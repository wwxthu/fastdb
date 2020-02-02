/***********************************************************
 * media transfer AG
 * 
 * Package:     FastDB Browser
 * Module:      $RCSfile: helpview.h,v $
 * Type:        C++ Header
 * Version:     $Revision: 1.2 $
 * Last changed:
 *   Date: $Date: 2003/11/03 07:40:40 $
 *   By:   $Author: kzerbe $
 * 
 ***********************************************************/
/**
 * @file helpview.h
 *
 * help dialog
 */

#if !defined(AFX_HELPVIEW_H__541D8028_4CED_49BD_8744_7117F3DC080A__INCLUDED_)
#define AFX_HELPVIEW_H__541D8028_4CED_49BD_8744_7117F3DC080A__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CHtmlView : public CWindowImpl<CHtmlView, CAxWindow>
{

public:
  DECLARE_WND_SUPERCLASS(NULL, CAxWindow::GetWndClassName())

  BOOL PreTranslateMessage(MSG* pMsg)
  {
    if((pMsg->message < WM_KEYFIRST || pMsg->message > WM_KEYLAST) &&
       (pMsg->message < WM_MOUSEFIRST || pMsg->message > WM_MOUSELAST))
      return FALSE;

    // give HTML page a chance to translate this message
    return (BOOL)SendMessage(WM_FORWARDMSG, 0, (LPARAM)pMsg);
  }

  BEGIN_MSG_MAP(CHtmlView)
  END_MSG_MAP()
};


class CHelpView : public CMDIChildWindowImpl<CHelpView>
{

public:
  DECLARE_FRAME_WND_CLASS(NULL, IDR_HELPVIEW)

  /// cleanup before window is closing
  virtual void OnFinalMessage(HWND hWnd);

protected:
  LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
  LRESULT OnForwardMsg(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/);

private:
  CHtmlView m_view;

public:
  BEGIN_MSG_MAP(CHelpView)
  MESSAGE_HANDLER(WM_CREATE, OnCreate)
  MESSAGE_HANDLER(WM_FORWARDMSG, OnForwardMsg)

  CHAIN_MSG_MAP(CMDIChildWindowImpl<CHelpView>)
  END_MSG_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HELPVIEW_H__541D8028_4CED_49BD_8744_7117F3DC080A__INCLUDED_)
