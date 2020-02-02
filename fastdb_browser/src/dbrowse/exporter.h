/***********************************************************
 * media transfer AG
 * 
 * Package:     FastDB Browser
 * Module:      $RCSfile: exporter.h,v $
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

#if !defined(__EXPORTER_H__)
#define __EXPORTER_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "common/stdstring.h"

class CCaseExporter
{

public:
  CCaseExporter()
  { }

  void Export(int dbHandle, CStdString cppName);
};


#endif
