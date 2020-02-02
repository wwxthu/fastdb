/***********************************************************
 * media transfer AG
 * 
 * Package:     FastDB Browser
 * Module:      $RCSfile: mainfrm.h,v $
 * Type:        C++ Header
 * Version:     $Revision: 1.3 $
 * Last changed:
 *   Date: $Date: 2003/11/17 15:38:08 $
 *   By:   $Author: kzerbe $
 * 
 ***********************************************************/
/**
 * @file mainfrm.h
 *
 * main window (MDI frame) window
 */

#if !defined(AFX_MAINFRM_H__310802F5_0897_4BC9_B359_6B4CBDC9CFE3__INCLUDED_)
#define AFX_MAINFRM_H__310802F5_0897_4BC9_B359_6B4CBDC9CFE3__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#include "helpview.h"

#define WM_TRIGGERSTATE "StateTriggered"


class CMainFrame : public CMDIFrameWindowImpl<CMainFrame>, public CUpdateUI<CMainFrame>,
      public CMessageFilter, public CIdleHandler
{

public:
  DECLARE_FRAME_WND_CLASS(NULL, IDR_MAINFRAME)

  CMDICommandBarCtrl m_CmdBar;

  virtual BOOL PreTranslateMessage(MSG* pMsg);
  virtual BOOL OnIdle();

  static UINT m_stateTriggered;

  // Handler prototypes (uncomment arguments if needed):
  // LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
  // LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
  // LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

protected:
  LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
  LRESULT OnStateTriggered(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
  LRESULT OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnFileOpen(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnFileExport(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnXmlImport(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnXmlExport(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

  LRESULT OnBeginTransact(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnNewRecord(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnDeleteRecord(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnCommit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnRollback(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnRefresh(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

  LRESULT OnViewToolBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnViewStatusBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnHelp(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

  LRESULT OnWindowCascade(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnWindowTile(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnWindowArrangeIcons(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

public:
  BEGIN_UPDATE_UI_MAP(CMainFrame)
  UPDATE_ELEMENT(ID_VIEW_TOOLBAR, UPDUI_MENUPOPUP)
  UPDATE_ELEMENT(ID_VIEW_STATUS_BAR, UPDUI_MENUPOPUP)
  UPDATE_ELEMENT(ID_EDIT, UPDUI_MENUPOPUP|UPDUI_TOOLBAR)
  UPDATE_ELEMENT(ID_NEWRECORD, UPDUI_MENUPOPUP|UPDUI_TOOLBAR)
  UPDATE_ELEMENT(ID_DELETERECORD, UPDUI_MENUPOPUP|UPDUI_TOOLBAR)
  END_UPDATE_UI_MAP()

  BEGIN_MSG_MAP(CMainFrame)
  MESSAGE_HANDLER(WM_CREATE, OnCreate)
  MESSAGE_HANDLER(CMainFrame::m_stateTriggered, OnStateTriggered)
  COMMAND_ID_HANDLER(ID_APP_EXIT, OnFileExit)
  COMMAND_ID_HANDLER(ID_FILE_OPEN, OnFileOpen)
  COMMAND_ID_HANDLER(ID_EXPORT, OnFileExport)
  COMMAND_ID_HANDLER(ID_XML_IMPORT, OnXmlImport)
  COMMAND_ID_HANDLER(ID_XML_EXPORT, OnXmlExport)

  COMMAND_ID_HANDLER(ID_VIEW_TOOLBAR, OnViewToolBar)
  COMMAND_ID_HANDLER(ID_VIEW_STATUS_BAR, OnViewStatusBar)

  COMMAND_ID_HANDLER(ID_EDIT, OnBeginTransact)
  COMMAND_ID_HANDLER(ID_NEWRECORD, OnNewRecord)
  COMMAND_ID_HANDLER(ID_DELETERECORD, OnDeleteRecord)
  COMMAND_ID_HANDLER(ID_COMMIT, OnCommit)
  COMMAND_ID_HANDLER(ID_ROLLBACK, OnRollback)
  COMMAND_ID_HANDLER(ID_REFRESH, OnRefresh)

  COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
  COMMAND_ID_HANDLER(ID_HELP, OnHelp)

  COMMAND_ID_HANDLER(ID_WINDOW_CASCADE, OnWindowCascade)
  COMMAND_ID_HANDLER(ID_WINDOW_TILE_HORZ, OnWindowTile)
  COMMAND_ID_HANDLER(ID_WINDOW_ARRANGE, OnWindowArrangeIcons)
  CHAIN_MSG_MAP(CUpdateUI<CMainFrame>)
  CHAIN_MSG_MAP(CMDIFrameWindowImpl<CMainFrame>)
  END_MSG_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__310802F5_0897_4BC9_B359_6B4CBDC9CFE3__INCLUDED_)
