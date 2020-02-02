/***********************************************************
 * media transfer AG
 * 
 * Package:     FastDB Browser
 * Module:      $RCSfile: dbrowseview.h,v $
 * Type:        C++ Header
 * Version:     $Revision: 1.2 $
 * Last changed:
 *   Date: $Date: 2003/11/03 07:40:40 $
 *   By:   $Author: kzerbe $
 * 
 ***********************************************************/
/**
 * @file dbrowseview.h
 *
 * listview window
 */

#if !defined(AFX_DBROWSEVIEW_H__A9456857_E9BD_490F_B799_484AC8ACF201__INCLUDED_)
#define AFX_DBROWSEVIEW_H__A9456857_E9BD_490F_B799_484AC8ACF201__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CDbrowseView : public CWindowImpl<CDbrowseView, CListViewCtrl>
{

public:
  DECLARE_WND_SUPERCLASS(NULL, CListViewCtrl::GetWndClassName())

  BOOL PreTranslateMessage(MSG* pMsg);

  BEGIN_MSG_MAP(CDbrowseView)
  END_MSG_MAP()

  // Handler prototypes (uncomment arguments if needed):
  // LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
  // LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
  // LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DBROWSEVIEW_H__A9456857_E9BD_490F_B799_484AC8ACF201__INCLUDED_)
