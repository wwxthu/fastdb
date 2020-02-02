/***********************************************************
 * media transfer AG
 * 
 * Package:     FastDB Browser
 * Module:      $RCSfile: stdafx.h,v $
 * Type:        C++ Header
 * Version:     $Revision: 1.2 $
 * Last changed:
 *   Date: $Date: 2003/11/03 07:40:41 $
 *   By:   $Author: kzerbe $
 * 
 ***********************************************************/
/**
 * @file stdafx.h
 *
 * common header (precompiled)
 */

#if !defined(AFX_STDAFX_H__0E22057D_F52B_45CE_B985_88B6CEDE5DCB__INCLUDED_)
#define AFX_STDAFX_H__0E22057D_F52B_45CE_B985_88B6CEDE5DCB__INCLUDED_

// Change these values to use different versions
#define WINVER  0x0400
//#define _WIN32_WINNT 0x0400
#define _WIN32_IE 0x0500
#define _RICHEDIT_VER 0x0100
#define _WTL_USE_CSTRING

#define _ATL_DLL

#include <atlbase.h>
#include <atlapp.h>

extern CAppModule _Module;

#include <atlcom.h>
#include <atlhost.h>
#include <atlwin.h>
#include <atlmisc.h>
#include <atlctl.h>

#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>
#include <atlctrlw.h>
#include <atlddx.h>



//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__0E22057D_F52B_45CE_B985_88B6CEDE5DCB__INCLUDED_)
