/***********************************************************
 * media transfer AG
 * 
 * Package:     FastDB Browser
 * Module:      $RCSfile: aboutdlg.h,v $
 * Type:        C++ Header
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

#if !defined(AFX_ABOUTDLG_H__079E7A03_0E5D_4D17_A732_682970D91B2E__INCLUDED_)
#define AFX_ABOUTDLG_H__079E7A03_0E5D_4D17_A732_682970D91B2E__INCLUDED_

class CAboutDlg : public CDialogImpl<CAboutDlg>
{

public:
  enum { IDD = IDD_ABOUTBOX };

  BEGIN_MSG_MAP(CAboutDlg)
  MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
  COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
  COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
  END_MSG_MAP()

  // Handler prototypes (uncomment arguments if needed):
  // LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
  // LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
  // LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

  LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
  LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};

#endif // !defined(AFX_ABOUTDLG_H__079E7A03_0E5D_4D17_A732_682970D91B2E__INCLUDED_)
