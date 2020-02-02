
#ifndef __XMLIO_H__
#define __XMLIO_H__

#include "fastdb.h"

class dbXmlScanner
{

public:
  enum {
    MaxIdentSize = 256
  };
  enum token {
    xml_ident,
    xml_sconst,
    xml_iconst,
    xml_fconst,
    xml_lt,
    xml_gt,
    xml_lts,
    xml_gts,
    xml_eq,
    xml_eof,
    xml_error
  };

  dbXmlScanner(FILE* f)
  {
    in = f;
    sconst = new char[size = 1024];
    line = 1;
    pos = 0;
  }

  token scan();

  char* getString()
  {
    return sconst;
  }

  char* getIdentifier()
  {
    return ident;
  }

  size_t  getStringLength()
  {
    return slen;
  }

  db_int8 getInt()
  {
    return iconst;
  }

  double getReal()
  {
    return fconst;
  }

  bool expect(int sourcePos, token expected)
  {
    token tkn = scan();

    if (tkn != expected)
    {
      fprintf(stderr, "subsql.cpp:%d: line %d, column %d: Get token %d instead of expected token %d\n",
              sourcePos, line, pos, tkn, expected);
      return false;
    }

    return true;
  }

  bool expect(int sourcePos, char* expected)
  {
    token tkn = scan();

    if (tkn != xml_ident)
    {
      fprintf(stderr, "subsql.cpp:%d: line %d, column %d: Get token %d instead of expected identifier\n",
              sourcePos, line, pos, tkn);
      return false;
    }

    if (strcmp(ident, expected) != 0)
    {
      fprintf(stderr, "subsql.cpp:%d: line %d, column %d: Get tag '%s' instead of expected '%s'\n",
              sourcePos, line, pos, ident, expected);
      return false;
    }

    return true;
  }

private:

  int   get
    ();

  void  unget(int ch);

  int       line;

  int       pos;

  FILE*     in;

  char*     sconst;

  size_t    size;

  size_t    slen;

  db_int8   iconst;

  double    fconst;

  char      ident[MaxIdentSize];
};

class dbTmpAllocator
{
  enum {
    CHUNK_SIZE = 4096
  };

  struct Chunk
  {
    Chunk* next;
    Chunk* prev; // is not used, added for alignment
  };

  Chunk* curr;
  size_t used;

public:
  dbTmpAllocator()
  {
    curr = NULL;
    used = CHUNK_SIZE;
  }

  ~dbTmpAllocator()
  {
    reset();
  }

  void reset()
  {
    Chunk *c, *next;

    for (c = curr; c != NULL; c = next)
    {
      next = c->next;
      dbFree(c);
    }

    curr = NULL;
    used = CHUNK_SIZE;
  }


  void* alloc(size_t size)
  {
    size = DOALIGN(size, 8);

    if (size > CHUNK_SIZE/2)
    {
      Chunk* newChunk = (Chunk*)dbMalloc(size + sizeof(Chunk));

      if (curr != NULL)
      {
        newChunk->next = curr->next;
        curr->next = newChunk;
      }
      else
      {
        curr = newChunk;
        newChunk->next = NULL;
        used = CHUNK_SIZE;
      }

      return newChunk+1;
    }
    else if (size <= CHUNK_SIZE - used)
    {
      used += size;
      return (char*)(curr+1) + used - size;
    }
    else
    {
      Chunk* newChunk = (Chunk*)dbMalloc(CHUNK_SIZE);
      used = sizeof(Chunk) + size;
      newChunk->next = curr;
      curr = newChunk;
      return newChunk+1;
    }
  }
};


class dbXmlIo
{

public:
  dbXmlIo(dbDatabase* db);

  virtual ~dbXmlIo();

  bool importDatabase(FILE* in, bool append= false);
  void exportDatabase(FILE* out, bool schema= false);

private:
  bool importRecord(char* terminator, dbFieldDescriptor* fieldList, byte* rec, dbXmlScanner& scanner);
  bool importField(char* terminator, dbFieldDescriptor* fd, byte* rec, dbXmlScanner& scanner);
  oid_t mapId(long id);
  void insertRecord(dbTableDescriptor* desc, oid_t oid, void const* record);

  dbDatabase* m_db;
  dbTmpAllocator tmpAlloc;
  oid_t*  oidMap;
  oid_t   oidMapSize;
};

#endif
