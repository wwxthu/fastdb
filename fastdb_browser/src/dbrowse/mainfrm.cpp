/***********************************************************
 * media transfer AG
 * 
 * Package:     FastDB Browser
 * Module:      $RCSfile: mainfrm.cpp,v $
 * Type:        C++ Source
 * Version:     $Revision: 1.5 $
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

#include "stdafx.h"
#include "resource.h"

#include "aboutdlg.h"
#include "dbrowseView.h"
#include "ChildFrm.h"
#include "MainFrm.h"
#include "messages.h"
#include "exporter.h"
#include "fastdb/xmlio.h"
#include "fastdb/localcli.h"

UINT CMainFrame::m_stateTriggered= 0;

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
  if(CMDIFrameWindowImpl<CMainFrame>::PreTranslateMessage(pMsg))
    return TRUE;

  HWND hWnd = MDIGetActive();

  if(hWnd != NULL)
    return (BOOL)::SendMessage(hWnd, WM_FORWARDMSG, 0, (LPARAM)pMsg);

  return FALSE;
}

BOOL CMainFrame::OnIdle()
{
  UIUpdateToolBar();
  return FALSE;
}

LRESULT CMainFrame::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
  // create command bar window
  HWND hWndCmdBar = m_CmdBar.Create(m_hWnd, rcDefault, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE);
  // attach menu
  m_CmdBar.AttachMenu(GetMenu());
  // load command bar images
  m_CmdBar.LoadImages(IDR_MAINFRAME);
  // remove old menu
  SetMenu(NULL);

  HWND hWndToolBar = CreateSimpleToolBarCtrl(m_hWnd, IDR_MAINFRAME, FALSE, ATL_SIMPLE_TOOLBAR_PANE_STYLE);

  CreateSimpleReBar(ATL_SIMPLE_REBAR_NOBORDER_STYLE);
  AddSimpleReBarBand(hWndCmdBar);
  AddSimpleReBarBand(hWndToolBar, NULL, TRUE);

  CreateSimpleStatusBar();

  CreateMDIClient();
  m_CmdBar.SetMDIClient(m_hWndMDIClient);


  UIAddToolBar(hWndToolBar);
  UISetCheck(ID_VIEW_TOOLBAR, 1);
  UISetCheck(ID_VIEW_STATUS_BAR, 1);

  // register object for message filtering and idle updates
  CMessageLoop* pLoop = _Module.GetMessageLoop();
  ATLASSERT(pLoop != NULL);
  pLoop->AddMessageFilter(this);
  pLoop->AddIdleHandler(this);

  SetWindowText(APP_NAME);
  m_stateTriggered= RegisterWindowMessage(WM_TRIGGERSTATE);

  return 0;
}


LRESULT CMainFrame::OnStateTriggered(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
  CChildFrame::RefreshData();
  return 0;
}

LRESULT CMainFrame::OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  PostMessage(WM_CLOSE);
  return 0;
}

LRESULT CMainFrame::OnHelp(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  CHelpView* pChild = new CHelpView();
  pChild->CreateEx(m_hWndClient);
  return 0;
}

LRESULT CMainFrame::OnFileOpen(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  CFileDialog dlg(TRUE, NULL, "*.fdb",
                  OFN_FILEMUSTEXIST|OFN_LONGNAMES,
                  "FastDB Datenbank (*.fdb)\0*.fdb\0");

  if (dlg.DoModal()== IDOK)
  {
    CChildFrame* pChild = new CChildFrame(dlg.m_szFileTitle, dlg.m_szFileName);
    pChild->CreateEx(m_hWndClient);
  }

  return 0;
}


LRESULT CMainFrame::OnFileExport(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  CFileDialog dlg(FALSE, NULL, "*.h",
                  OFN_OVERWRITEPROMPT|OFN_PATHMUSTEXIST|OFN_LONGNAMES,
                  "C++ Headerfile (*.h)\0*.h\0");

  if (dlg.DoModal()== IDOK)
  {
    CCaseExporter exporter;
    exporter.Export(CChildFrame::m_currentDB, dlg.m_szFileName);
  }

  return 0;
}


LRESULT CMainFrame::OnXmlImport(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  CFileDialog dlg(TRUE, NULL, "*.xml",
                  OFN_FILEMUSTEXIST|OFN_LONGNAMES,
                  "XML Datenbank (*.xml)\0*.xml\0");

  if (dlg.DoModal()== IDOK)
  {
    session_desc* desc= dbCLI::instance.get_session(CChildFrame::m_currentDB);
    FILE *file= fopen(dlg.m_szFileName, "rt");

    if (file && desc)
    {
      dbXmlIo db(desc->db);
      db.importDatabase(file);
      fclose(file);
    }
  }

  return 0;
}

LRESULT CMainFrame::OnXmlExport(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  CFileDialog dlg(FALSE, NULL, "*.xml",
                  OFN_OVERWRITEPROMPT|OFN_PATHMUSTEXIST|OFN_LONGNAMES,
                  "XML database (*.xml)\0*.xml\0");

  if (dlg.DoModal()== IDOK)
  {
    session_desc* desc= dbCLI::instance.get_session(CChildFrame::m_currentDB);
    FILE *file= fopen(dlg.m_szFileName, "wt");

    if (file && desc)
    {
      dbXmlIo db(desc->db);
      db.exportDatabase(file);
      fclose(file);
    }
  }

  return 0;
}


LRESULT CMainFrame::OnBeginTransact(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  CChildFrame::StartEditing();

  UISetCheck(ID_EDIT, 1);
  return 0;
}


LRESULT CMainFrame::OnNewRecord(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  CChildFrame::NewRecord();
  return 0;
}


LRESULT CMainFrame::OnDeleteRecord(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  CChildFrame::DeleteRecord();
  return 0;
}


LRESULT CMainFrame::OnCommit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  CChildFrame::EndEdit(true);
  UISetCheck(ID_EDIT, 0);
  return 0;
}


LRESULT CMainFrame::OnRollback(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  CChildFrame::EndEdit(false);
  UISetCheck(ID_EDIT, 0);
  return 0;
}


LRESULT CMainFrame::OnViewToolBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  static BOOL bVisible = TRUE; // initially visible
  bVisible = !bVisible;
  CReBarCtrl rebar = m_hWndToolBar;
  int nBandIndex = rebar.IdToIndex(ATL_IDW_BAND_FIRST + 1); // toolbar is 2nd added band
  rebar.ShowBand(nBandIndex, bVisible);
  UISetCheck(ID_VIEW_TOOLBAR, bVisible);
  UpdateLayout();
  return 0;
}

LRESULT CMainFrame::OnViewStatusBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  BOOL bVisible = !::IsWindowVisible(m_hWndStatusBar);
  ::ShowWindow(m_hWndStatusBar, bVisible ? SW_SHOWNOACTIVATE : SW_HIDE);
  UISetCheck(ID_VIEW_STATUS_BAR, bVisible);
  UpdateLayout();
  return 0;
}


LRESULT CMainFrame::OnRefresh(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  CChildFrame::RefreshData();
  return 0;
}

LRESULT CMainFrame::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  CAboutDlg dlg;
  dlg.DoModal();
  return 0;
}

LRESULT CMainFrame::OnWindowCascade(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  MDICascade();
  return 0;
}

LRESULT CMainFrame::OnWindowTile(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  MDITile();
  return 0;
}

LRESULT CMainFrame::OnWindowArrangeIcons(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  MDIIconArrange();
  return 0;
}
