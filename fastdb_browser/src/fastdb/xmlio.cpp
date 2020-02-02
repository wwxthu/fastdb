
#include "StdAfx.h"
#include "fastdb.h"
#include "symtab.h"
#include "compiler.h"
#include "database.h"
#include "hashtab.h"
#include "ttree.h"
#include "xmlio.h"

inline int dbXmlScanner::get
  ()
{
  int ch = getc(in);

  if (ch == '\n')
  {
    pos = 0;
    line += 1;
  }
  else if (ch == '\t')
  {
    pos = DOALIGN(pos + 1, 8);
  }
  else
  {
    pos += 1;
  }

  return ch;
}

inline void dbXmlScanner::unget(int ch)
{
  if (ch != EOF)
  {
    if (ch != '\n')
    {
      pos -= 1;
    }
    else
    {
      line -= 1;
    }

    ungetc(ch, in);
  }
}


dbXmlScanner::token dbXmlScanner::scan()
{
  int ch, i, pos;
  bool floatingPoint;

retry:

  do
  {
    if ((ch = get
                ()) == EOF)
      {
        return xml_eof;
      }
  }
  while (ch <= ' ');

  switch (ch)
  {

  case '<':

    ch = get
           ();

    if (ch == '?')
    {
      while ((ch = get
                     ()) != '?')
        {
          if (ch == EOF)
          {
            return xml_error;
          }
        }

      if ((ch = get
                  ()) != '>')
        {
          return xml_error;
        }

      goto retry;
    }

    if (ch != '/')
    {
      unget(ch);
      return xml_lt;
    }

    return xml_lts;

  case '>':
    return xml_gt;

  case '/':

    ch = get
           ();

    if (ch != '>')
    {
      unget(ch);
      return xml_error;
    }

    return xml_gts;

  case '=':
    return xml_eq;

  case '"':
    i = 0;

    while (true)
    {

      ch = get
             ();

      switch (ch)
      {

      case EOF:
        return xml_error;

      case '&':

        switch (get
                ())
        {

        case 'a':

          if (get
              () != 'm' || get
                () != 'p' || get
                  () != ';')
              {
                return xml_error;
              }

          ch = '&';
          break;

        case 'l':

          if (get
              () != 't' || get
                () != ';')
            {
              return xml_error;
            }

          ch = '<';
          break;

        case 'g':

          if (get
              () != 't' || get
                () != ';')
            {
              return xml_error;
            }

          ch = '>';
          break;

        case 'q':

          if (get
              () != 'u' || get
                () != 'o' || get
                  () != 't' || get
                    () != ';')
                {
                  return xml_error;
                }

          ch = '"';
          break;

        default:
          return xml_error;
        }

        break;

      case '"':
        slen = i;
        sconst[i] = 0;
        return xml_sconst;
      }

      if ((size_t)i+1 >= size)
      {
        char* newBuf = new char[size *= 2];
        memcpy(newBuf, sconst, slen);
        sconst = newBuf;
      }

      sconst[i++] = (char)ch;
    }

  case '-':

  case '+':

  case '0':

  case '1':

  case '2':

  case '3':

  case '4':

  case '5':

  case '6':

  case '7':

  case '8':

  case '9':
    i = 0;
    floatingPoint = false;

    while (true)
    {
      if ((size_t)i == size)
      {
        return xml_error;
      }

      if (!isdigit(ch) && ch != '-' && ch != '+' && ch != '.' && ch != 'E')
      {
        unget(ch);
        sconst[i] = '\0';

        if (floatingPoint)
        {
          return sscanf(sconst, "%lf%n", &fconst, &pos) == 1 && pos == i
                 ? xml_fconst : xml_error;
        }
        else
        {
          return sscanf(sconst, INT8_FORMAT "%n", &iconst, &pos) == 1 && pos == i
                 ? xml_iconst : xml_error;
        }
      }

      sconst[i++] = (char)ch;

      if (ch == '.')
      {
        floatingPoint = true;
      }

      ch = get
             ();
    }

  default:
    i = 0;

    while (isalnum(ch) || ch == '-' || ch == ':' || ch == '_')
    {
      if (i == MaxIdentSize)
      {
        return xml_error;
      }

      ident[i++] = (char)ch;

      ch = get
             ();
    }

    unget(ch);

    if (i == MaxIdentSize || i == 0)
    {
      return xml_error;
    }

    ident[i] = '\0';
    return xml_ident;
  }
}

#define EXPECT(x) scanner.expect(__LINE__, x)

dbXmlIo::dbXmlIo(dbDatabase* db) : m_db(db)
{}

dbXmlIo::~dbXmlIo()
{}



bool skipElement(dbXmlScanner& scanner)
{
  int depth = 1;

  do
  {
    switch (scanner.scan())
    {

    case dbXmlScanner::xml_lt:
      depth += 1;
      continue;

    case dbXmlScanner::xml_lts:
      depth -= 1;

      if (depth < 0 || !EXPECT(dbXmlScanner::xml_ident) || !EXPECT(dbXmlScanner::xml_gt))
      {
        return false;
      }

      break;

    case dbXmlScanner::xml_gts:
      depth -= 1;
      break;

    default:
      continue;
    }
  }
  while (depth != 0);

  return true;
}

bool dbXmlIo::importRecord(char* terminator, dbFieldDescriptor* fieldList, byte* rec, dbXmlScanner& scanner)
{
  dbXmlScanner::token tkn;

  while ((tkn = scanner.scan()) != dbXmlScanner::xml_lts)
  {
    if (tkn != dbXmlScanner::xml_lt || !EXPECT(dbXmlScanner::xml_ident)
        || !EXPECT(dbXmlScanner::xml_gt))
    {
      return false;
    }

    char* fieldName = scanner.getIdentifier();

    dbSymbolTable::add
      (fieldName, tkn_ident, FASTDB_CLONE_ANY_IDENTIFIER);

    dbFieldDescriptor* fd = fieldList;

    while (true)
    {
      if (fd->name == fieldName)
      {
        if (!importField(fd->name, fd, rec, scanner))
        {
          return false;
        }

        break;
      }

      if ((fd = fd->next) == fieldList)
      {
        if (!skipElement(scanner))
        {
          return false;
        }

        break;
      }
    }
  }

  return EXPECT(terminator) && EXPECT(dbXmlScanner::xml_gt);
}

#define HEX_DIGIT(ch) ((ch) >= 'a' ? ((ch) - 'a' + 10) : (ch) >= 'A' ? (((ch) - 'A' + 10)) : ((ch) - '0'))

bool dbXmlIo::importField(char* terminator, dbFieldDescriptor* fd, byte* rec, dbXmlScanner& scanner)
{
  dbXmlScanner::token tkn;
  int i;
  long id;
  byte* dst = rec + fd->appOffs;

  switch (fd->type)
  {

  case dbField::tpStructure:
    return importRecord(terminator, fd->components, rec, scanner);

  case dbField::tpArray:
    {
      int arrSize = 8;
      int elemSize = fd->components->appSize;
      byte* arr = (byte*)tmpAlloc.alloc(elemSize*arrSize);
      memset(arr, 0, elemSize*arrSize);

      for (i = 0; (tkn = scanner.scan()) == dbXmlScanner::xml_lt; i++)
      {
        if (!EXPECT("array-element")
            || !EXPECT(dbXmlScanner::xml_gt))
        {
          return false;
        }

        if (i == arrSize)
        {
          arrSize *= 2;
          byte* newArr = (byte*)tmpAlloc.alloc(elemSize*arrSize);
          memcpy(newArr, arr, i*elemSize);
          memset(newArr + i*elemSize, 0, i*elemSize);
          arr = newArr;
        }

        importField("array-element", fd->components, arr + i*elemSize, scanner);
      }

      dbAnyArray::arrayAllocator((dbAnyArray*)dst, arr, i);
      return tkn == dbXmlScanner::xml_lts
             && EXPECT(terminator)
             && EXPECT(dbXmlScanner::xml_gt);
    }

  case dbField::tpReference:

    if (!EXPECT(dbXmlScanner::xml_lt)
        || !EXPECT("ref")
        || !EXPECT("id")
        || !EXPECT(dbXmlScanner::xml_eq)
        || !EXPECT(dbXmlScanner::xml_sconst)
        || sscanf(scanner.getString(), "%ld", &id) != 1
        || !EXPECT(dbXmlScanner::xml_gts))
    {
      return false;
    }

    *(oid_t*)dst = mapId(id);
    break;

  case dbField::tpBool:

    switch (scanner.scan())
    {

    case dbXmlScanner::xml_iconst:
      *(bool*)dst = scanner.getInt() != 0;
      break;

    case dbXmlScanner::xml_fconst:
      *(bool*)dst = scanner.getReal() != 0.0;
      break;

    default:
      fprintf(stderr, "Failed to convert field '%s'\n", fd->name);
    }

    break;

  case dbField::tpInt1:

    switch (scanner.scan())
    {

    case dbXmlScanner::xml_iconst:
      *(int1*)dst = (int1)scanner.getInt();
      break;

    case dbXmlScanner::xml_fconst:
      *(int1*)dst = (int1)scanner.getReal();
      break;

    default:
      fprintf(stderr, "Failed to convert field '%s'\n", fd->name);
    }

    break;

  case dbField::tpInt2:

    switch (scanner.scan())
    {

    case dbXmlScanner::xml_iconst:
      *(int2*)dst = (int2)scanner.getInt();
      break;

    case dbXmlScanner::xml_fconst:
      *(int2*)dst = (int2)scanner.getReal();
      break;

    default:
      fprintf(stderr, "Failed to convert field '%s'\n", fd->name);
    }

    break;

  case dbField::tpInt4:

    switch (scanner.scan())
    {

    case dbXmlScanner::xml_iconst:
      *(int4*)dst = (int4)scanner.getInt();
      break;

    case dbXmlScanner::xml_fconst:
      *(int4*)dst = (int4)scanner.getReal();
      break;

    default:
      fprintf(stderr, "Failed to convert field '%s'\n", fd->name);
    }

    break;

  case dbField::tpInt8:

    switch (scanner.scan())
    {

    case dbXmlScanner::xml_iconst:
      *(db_int8*)dst = scanner.getInt();
      break;

    case dbXmlScanner::xml_fconst:
      *(db_int8*)dst = (int8)scanner.getReal();
      break;

    default:
      fprintf(stderr, "Failed to convert field '%s'\n", fd->name);
    }

    break;

  case dbField::tpReal4:

    switch (scanner.scan())
    {

    case dbXmlScanner::xml_iconst:
      *(real4*)dst = (real4)scanner.getInt();
      break;

    case dbXmlScanner::xml_fconst:
      *(real4*)dst = (real4)scanner.getReal();
      break;

    default:
      fprintf(stderr, "Failed to convert field '%s'\n", fd->name);
    }

    break;

  case dbField::tpReal8:

    switch (scanner.scan())
    {

    case dbXmlScanner::xml_iconst:
      *(real8*)dst = (real8)scanner.getInt();
      break;

    case dbXmlScanner::xml_fconst:
      *(real8*)dst = scanner.getReal();
      break;

    default:
      fprintf(stderr, "Failed to convert field '%s'\n", fd->name);
    }

    break;

  case dbField::tpString:

    switch (scanner.scan())
    {

    case dbXmlScanner::xml_iconst:

    case dbXmlScanner::xml_sconst:

    case dbXmlScanner::xml_fconst:
      {
        char* str = (char*)tmpAlloc.alloc((scanner.getStringLength()+1));
        memcpy(str, scanner.getString(), scanner.getStringLength());
        str[scanner.getStringLength()] = '\0';
        *(char**)dst = str;
        break;
      }

    default:
      fprintf(stderr, "Failed to convert field '%s'\n", fd->name);
    }

    break;

  case dbField::tpRawBinary:
    {
      char* src = scanner.getString();
      int len = scanner.getStringLength() >> 1;

      if (fd->appSize != (size_t)len)
      {
        fprintf(stderr, "Length of raw binary field '%s' was changed\n", fd->name);
      }
      else
      {
        while (--len >= 0)
        {
          *dst++ = (HEX_DIGIT(src[0]) << 4) | HEX_DIGIT(src[1]);
          src += 2;
        }
      }

      break;
    }

#ifdef USE_STD_STRING

  case dbField::tpStdString:
    ((std::string*)dst)->assign(scanner.getString(), scanner.getStringLength());
    break;
#endif

  }

  return EXPECT(dbXmlScanner::xml_lts)
         && EXPECT(terminator)
         && EXPECT(dbXmlScanner::xml_gt);
}

oid_t dbXmlIo::mapId(long id)
{
  oid_t oid;

  if (id == 0)
  {
    return 0;
  }

  if ((oid_t)id >= oidMapSize)
  {
    oid_t* newOidMap = new oid_t[id*2];
    memcpy(newOidMap, oidMap, oidMapSize*sizeof(oid_t));
    memset(newOidMap + oidMapSize, 0, (id*2-oidMapSize)*sizeof(oid_t));
    oidMapSize = id*2;
    oidMap = newOidMap;
  }

  oid = oidMap[id];

  if (oid == 0)
  {
    oid = m_db->allocateId();
    oidMap[id] = oid;
  }

  return oid;
}

void dbXmlIo::insertRecord(dbTableDescriptor* desc, oid_t oid, void const* record)
{
  dbFieldDescriptor* fd;
  byte* src = (byte*)record;
  size_t size = desc->columns->calculateRecordSize(src, desc->fixedSize);
  m_db->allocateRow(desc->tableId, oid, size);
  byte* dst = (byte*)m_db->getRow(oid);
  desc->columns->storeRecordFields(dst, src, desc->fixedSize, true);

  dbTable* table = (dbTable*)m_db->getRow(desc->tableId);
#ifdef AUTOINCREMENT_SUPPORT

  desc->autoincrementCount = table->count;
#endif

  int nRows = table->nRows;

  for (fd = desc->hashedFields; fd != NULL; fd = fd->nextHashedField)
  {
    dbHashTable::insert(m_db, fd->hashTable, oid, fd->type, fd->dbsSize, fd->dbsOffs, nRows);
  }

  for (fd = desc->indexedFields; fd != NULL; fd = fd->nextIndexedField)
  {
    dbTtree::insert(m_db, fd->tTree, oid, fd->type, fd->dbsSize, fd->comparator, fd->dbsOffs);
  }
}

bool dbXmlIo::importDatabase(FILE* in, bool append)
{
  dbXmlScanner scanner(in);
  dbXmlScanner::token tkn;

  if (!EXPECT(dbXmlScanner::xml_lt) ||
      !EXPECT("database") ||
      !EXPECT(dbXmlScanner::xml_gt))
  {
    return false;
  }

  oidMapSize = dbDefaultInitIndexSize;
  oidMap = new oid_t[oidMapSize];
  memset(oidMap, 0, oidMapSize*sizeof(oid_t));

  m_db->beginTransaction(dbDatabase::dbExclusiveLock);

  dbTableDescriptor* desc;

  if (!append)
  {
    for (desc=m_db->tables; desc != NULL; desc=desc->nextDbTable)
    {
      if (desc->tableId != dbMetaTableId)
      {
        m_db->deleteTable(desc);
      }
    }
  }

  while ((tkn = scanner.scan()) != dbXmlScanner::xml_lts)
  {
    if (tkn != dbXmlScanner::xml_lt || !EXPECT(dbXmlScanner::xml_ident))
    {
      return false;
    }

    desc = m_db->findTableByName(scanner.getIdentifier());

    if (desc == NULL)
    {
      fprintf(stderr, "Table '%s' not found\n", scanner.getIdentifier());
    }

    if (!EXPECT("id")
        || !EXPECT(dbXmlScanner::xml_eq)
        || !EXPECT(dbXmlScanner::xml_sconst)
        || !EXPECT(dbXmlScanner::xml_gt))
    {
      return false;
    }

    if (desc != NULL)
    {
      long id;

      if (sscanf(scanner.getString(), "%ld", &id) != 1)
      {
        return false;
      }

      oid_t oid = mapId(id);
      byte *record = (byte*)tmpAlloc.alloc(desc->appSize);
      memset(record, 0, desc->appSize);

      if (!importRecord(desc->name, desc->columns, record, scanner))
      {
        tmpAlloc.reset();
        return false;
      }

      insertRecord(desc, oid, record);
      tmpAlloc.reset();
    }
    else
    { // skip record

      if (!skipElement(scanner))
      {
        return false;
      }
    }
  }

  return EXPECT("database") && EXPECT(dbXmlScanner::xml_gt);
}


void exportString(FILE* out, char* src, int len)
{
  fprintf(out, "\"");

  while (--len > 0)
  {
    byte b = (byte)*src++;

    switch (b)
    {

    case '&':
      fprintf(out, "&amp;");
      break;

    case '<':
      fprintf(out, "&lt;");
      break;

    case '>':
      fprintf(out, "&gt;");
      break;

    case '"':
      fprintf(out, "&quot;");
      break;

    default:
      fprintf(out, "%c", b);
    }
  }

  fprintf(out, "\"");
}

void exportBinary(FILE* out, byte* src, int len)
{
  fprintf(out, "\"");

  while (--len >= 0)
  {
    fprintf(out, "%02X", *src++);
  }

  fprintf(out, "\"");
}

void exportRecord(dbFieldDescriptor* fieldList, FILE* out, byte* src, int indent)
{
  int i;
  dbFieldDescriptor* fd = fieldList;

  do
  {
    byte* ptr = src + fd->dbsOffs;

    for (i = indent; --i >= 0;)
    {
      fprintf(out, " ");
    }

    char* fieldName = fd->name;

    if (strcmp(fieldName, "[]") == 0)
    {
      fieldName = "array-element";
    }

    fprintf(out, "<%s>", fieldName);

    switch (fd->type)
    {

    case dbField::tpBool:
      fprintf(out, "%d", *(bool*)ptr);
      break;

    case dbField::tpInt1:
      fprintf(out, "%d", *(int1*)ptr);
      break;

    case dbField::tpInt2:
      fprintf(out, "%d", *(int2*)ptr);
      break;

    case dbField::tpInt4:
      fprintf(out, "%d", *(int4*)ptr);
      break;

    case dbField::tpInt8:
      fprintf(out, INT8_FORMAT, *(db_int8*)ptr);
      break;

    case dbField::tpReal4:
      fprintf(out, "%.8G", *(real4*)ptr);
      break;

    case dbField::tpReal8:
      fprintf(out, "%.16G", *(real8*)ptr);
      break;

    case dbField::tpRawBinary:
      exportBinary(out, src+fd->dbsOffs, fd->dbsSize);
      break;

    case dbField::tpString:
      exportString(out, (char*)(src + ((dbVarying*)ptr)->offs), ((dbVarying*)ptr)->size);
      break;

    case dbField::tpArray:
      {
        int nElems = ((dbVarying*)ptr)->size;
        byte* srcElem = src + ((dbVarying*)ptr)->offs;
        dbFieldDescriptor* element = fd->components;
        fprintf(out, "\n");

        while (--nElems >= 0)
        {
          exportRecord(element, out, srcElem, indent+1);
          srcElem += element->dbsSize;
        }

        for (i = indent; --i >= 0;)
        {
          fprintf(out, " ");
        }

        break;
      }

    case dbField::tpReference:
      fprintf(out, "<ref id=\"%ld\"/>", (long)*(oid_t*)ptr);
      break;

    case dbField::tpStructure:
      fprintf(out, "\n");
      exportRecord(fd->components, out, src, indent+1);

      for (i = indent; --i >= 0;)
      {
        fprintf(out, " ");
      }

      break;
    }

    fprintf(out, "</%s>\n", fieldName);
  }
  while ((fd = fd->next) != fieldList);
}

void dbXmlIo::exportDatabase(FILE* out, bool schema)
{
  fprintf(out, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<database>\n");

  for (dbTableDescriptor* desc=m_db->tables; desc != NULL; desc=desc->nextDbTable)
  {
    if (desc->tableId != dbMetaTableId)
    {
      dbTable* table = (dbTable*)m_db->getRow(desc->tableId);
      oid_t oid = table->firstRow;
      int n = table->nRows;
      int percent = 0;

      if (schema)
      {
        int i;
        fprintf(out, " <%s id=\"x\">\n", desc->name);
        dbFieldDescriptor* fd = desc->columns;

        do
        {
          for (i = 2; --i >= 0;)
            fprintf(out, " ");

          char* fieldName = fd->name;

          fprintf(out, "<%s>", fieldName);

          if (fd->type== dbField::tpArray)
          {
            fprintf(out, "\n");

            for (i = 4; --i >= 0;)
              fprintf(out, " ");

            fprintf(out, "<array-element></array-element>\n");
          }

          fprintf(out, "</%s>\n", fieldName);
        }
        while ((fd = fd->next) != desc->columns);

        fprintf(out, " </%s>\n", desc->name);
      }
      else
      {
        for (int i = 0; oid != 0; i++)
        {
          dbRecord* rec = m_db->getRow(oid);
          fprintf(out, " <%s id=\"%d\">\n", desc->name, oid);
          exportRecord(desc->columns, out, (byte*)rec, 2);
          fprintf(out, " </%s>\n", desc->name);
          oid = rec->next;
          int p = (i+1)*100/n;

          if (p != percent)
          {
            fprintf(stderr, "Exporting table %s: %d%%\r", desc->name, p);
            fflush(stderr);
            percent = p;
          }
        }
      }

      fputc('\n', stderr);
    }
  }

  fprintf(out, "</database>\n");
}


