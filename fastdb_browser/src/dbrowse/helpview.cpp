/***********************************************************
 * media transfer AG
 * 
 * Package:     FastDB Browser
 * Module:      $RCSfile: helpview.cpp,v $
 * Type:        C++ Source
 * Version:     $Revision: 1.2 $
 * Last changed:
 *   Date: $Date: 2003/11/03 07:40:40 $
 *   By:   $Author: kzerbe $
 * 
 ***********************************************************/
/**
 * @file helpview.cpp
 *
 * help dialog
 */

#include "stdafx.h"
#include "resource.h"

#include "helpview.h"
#include "common/stdstring.h"


LRESULT CHelpView::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
  CStdString appName, url;
  GetModuleFileName(_Module.GetModuleInstance(),appName.GetBufferSetLength(300),301);
  url.Format("res://%s/MANUAL", appName.c_str());

  m_hWndClient = m_view.Create(m_hWnd, rcDefault, url,
                               WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_HSCROLL | WS_VSCROLL,
                               WS_EX_CLIENTEDGE);
  return 0;
}


void CHelpView::OnFinalMessage(HWND hWnd)
{
  // destroy object
  delete this;
}

LRESULT CHelpView::OnForwardMsg(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
  LPMSG pMsg = (LPMSG)lParam;

  if(CMDIChildWindowImpl<CHelpView>::PreTranslateMessage(pMsg))
    return TRUE;

  return m_view.PreTranslateMessage(pMsg);
}

