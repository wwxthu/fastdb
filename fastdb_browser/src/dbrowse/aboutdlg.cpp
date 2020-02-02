/***********************************************************
 * media transfer AG
 * 
 * Package:     FastDB Browser
 * Module:      $RCSfile: aboutdlg.cpp,v $
 * Type:        C++ Source
 * Version:     $Revision: 1.2 $
 * Last changed:
 *   Date: $Date: 2003/11/03 07:40:40 $
 *   By:   $Author: kzerbe $
 * 
 ***********************************************************/
/**
 * @file aboutdlg.h
 *
 * About dialog
 */


#include "stdafx.h"
#include "resource.h"

#include "aboutdlg.h"

LRESULT CAboutDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
  CenterWindow(GetParent());
  return TRUE;
}

LRESULT CAboutDlg::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  EndDialog(wID);
  return 0;
}
