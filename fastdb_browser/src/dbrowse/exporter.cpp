/***********************************************************
 * media transfer AG
 * 
 * Package:     FastDB Browser
 * Module:      $RCSfile: exporter.cpp,v $
 * Type:        C++ Header
 * Version:     $Revision: 1.2 $
 * Last changed:
 *   Date: $Date: 2003/11/03 07:40:40 $
 *   By:   $Author: kzerbe $
 * 
 ***********************************************************/
/**
 * @file exporter.h
 *
 * schema exporter for case tools
 */

#include "stdafx.h"

#include "exporter.h"
#include "fastdb/cli.h"   // FastDB call level interface
#include "fastdb/exception.h"

#include <fstream>

using namespace std;

void CCaseExporter::Export(int dbHandle, CStdString cppName)
{
  static char* typeNames[]=
    {
      "dbReference<%s>","bool","int1","int2","int4","int8","real4","real8",
      "decimal","string","string*","cstring",
      "dbArray< dbReference<%s> >","dbArray<bool>",
      "dbArray<int1>","dbArray<int2>","dbArray<int4>","dbArray<int8>",
      "dbArray<real4>","dbArray<real8>", "dbArray<decimal>","dbArray<string>",
      "any","datetime","auto","rect","unknown" };

  if (!dbHandle)
    return;

  ofstream outf(cppName);

  if (!outf.is_open())
  {
    return;
  }

  cli_table_descriptor *td;
  int i, tableCnt= cli_show_tables(dbHandle, &td);

  for (i= 0; i< tableCnt; ++i)
  {
    outf << "class " << td[i].name  << " {" << endl << "public:" << endl;

    cli_field_descriptor *fd;
    int j, fieldCnt= cli_describe(dbHandle, td[i].name, &fd);

    for (j= 0; j< fieldCnt; ++j)
    {
      CStdString type;

      if (fd[j].type == cli_oid)
      {
        type.Format("%s*", fd[j].refTableName);
        outf << "   " << type << " " << fd[j].name << ";" << endl;
      }
      else if (fd[j].type > cli_array_of_oid && fd[j].type <= cli_array_of_string)
      {
        type= typeNames[fd[j].type];
        outf << "   " << type << " " << fd[j].name << "[];" << endl;
      }
      else if (fd[j].type == cli_array_of_oid)
      {
        type.Format("%s*", fd[j].refTableName);
        outf << "   " << type << " " << fd[j].name << "[];" << endl;
      }
      else
      {
        type= typeNames[fd[j].type];
        outf << "   " << type << " " << fd[j].name << ";" << endl;
      }

    }

    outf << "};" << endl << endl;
  }

  outf.close();
}
