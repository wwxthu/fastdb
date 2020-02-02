/***********************************************************
 * media transfer AG
 * 
 * Package:     FastDB Browser
 * Module:      $RCSfile: queryprompter.h,v $
 * Type:        C++ Header
 * Version:     $Revision: 1.2 $
 * Last changed:
 *   Date: $Date: 2003/11/03 07:40:41 $
 *   By:   $Author: kzerbe $
 * 
 ***********************************************************/
/**
 * @file queryprompter.h
 *
 * string prompter for query clauses
 */


#ifndef __QUERYPROMPTER_H_
#define __QUERYPROMPTER_H_

#include "resource.h"

class CQueryPrompter :
      public CDialogImpl<CQueryPrompter>,
      public CWinDataExchange<CQueryPrompter>
{

public:
  enum { IDD = IDD_PROMPT_QUERY };

  LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
  {
    DoDataExchange(FALSE);
    return TRUE;
  }

  LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
  {
    DoDataExchange(TRUE);
    EndDialog(IDOK);
    return 0;
  }

  LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
  {
    EndDialog(IDCANCEL);
    return 0;
  }

  CString m_query;

  BEGIN_MSG_MAP(CQueryPrompter)
  MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
  COMMAND_ID_HANDLER(IDOK, OnOK)
  COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
  END_MSG_MAP()

  BEGIN_DDX_MAP(CQueryPrompter)
  DDX_TEXT(IDC_QUERY, m_query)
  END_DDX_MAP()
};

#endif
