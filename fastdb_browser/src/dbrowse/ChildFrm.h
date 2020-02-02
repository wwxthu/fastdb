/***********************************************************
 * media transfer AG
 * 
 * Package:     FastDB Browser
 * Module:      $RCSfile: ChildFrm.h,v $
 * Type:        C++ Header
 * Version:     $Revision: 1.3 $
 * Last changed:
 *   Date: $Date: 2003/11/17 15:38:08 $
 *   By:   $Author: kzerbe $
 * 
 ***********************************************************/
/**
 * @file ChildFrm.h
 *
 * Database views (MDI child windows)
 */

#if !defined(AFX_CHILDFRM_H__109973F8_9A5F_4FCA_8BEB_F3180D13B164__INCLUDED_)
#define AFX_CHILDFRM_H__109973F8_9A5F_4FCA_8BEB_F3180D13B164__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <list>
#include <vector>

using namespace std;

#include "common/stdstring.h" // CStdString class


/// register/deregister views to related database view
#define WM_REGISTERVIEW WM_APP+1
#define WM_REFRESH_DATA WM_APP+2

#define PLAIN_SQL_SELECT  "select * from %s where %s"
#define FIELD_BUFFER_SIZE 4096


/** structural info for database field */

struct TFieldDef
{
  /// field name
  CStdString name;

  // table to which field belongs
  CStdString refTable;

  /// field type id
  int type;

  /// field size (bytes)
  int size;

  /// field offset in record buffer
  int ofs;
};

/** mode for this view is used */
typedef enum EViewType
{
  eNoView,      /**< new view defined */
  eDbView,      /**< database view */
  eSchemaView,  /**< table schema view */
  eDataView,    /**< table content view */
  eQbeView,     /**< query by example view */
  eArrayView,   /**< array field content view */
  eRecordView   /**< record detail view */
};

/** common database content/schema view window class
 *
 * implemented as a MDI child window
 */

class CChildFrame : public CMDIChildWindowImpl<CChildFrame>
{

public:
  /// declare as MDI child
  DECLARE_FRAME_WND_CLASS(NULL, IDR_MDICHILD)

  /** constructor used for database main view
   *
   * @param dbName database (internal) name
   * @param dbPath pathname of file where database persists
   */
  CChildFrame(CStdString dbName, CStdString dbPath);

  /** constructor used for related views
   *
   * related views depend on a database view and are closed if database is closed.
   * they show schemas, tables, record details and array-field content
   *
   * @param dbHandle handle for opened database
   * @param dbTable table which content or schema is showed
   * @param dbName database name
   * @param oid object id of record which content is showed (if only one record is showed)
   * @param field definition of field that is showed (for array field content)
   */
  CChildFrame(int dbHandle, CStdString dbTable, CStdString dbName,
              int oid=0, TFieldDef* field=NULL);

  /// enter edit mode
  static void StartEditing();

  /// terminate edit mode
  static void EndEdit(bool bCommit);

  /// refresh data in all views (call after any change)
  static void RefreshData();

  /// create/edit new record
  static void NewRecord();

  /// delete record from table
  static void DeleteRecord();

  /// session handle for current database
  static int m_currentDB;

protected:
  /// handling view creation
  LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);

  /// process message forwarded from frame window
  LRESULT OnForwardMsg(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/);

  /// handler for clicking field content
  LRESULT OnItemActivated(int /*idCtrl*/, LPNMHDR pnmh, BOOL& bHandled);

  /// handler for right clicking field content
  LRESULT OnItemRClick(int /*idCtrl*/, LPNMHDR pnmh, BOOL& bHandled);

  /// register/deregister dependend views to database view
  LRESULT OnRegisterView(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

  /// refresh content after db change
  LRESULT OnRefreshData(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

  // window activated
  LRESULT OnActivate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);

  LRESULT OnSave(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

  LRESULT OnNewRecord(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

  LRESULT OnDeleteRecord(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

  /// cleanup before window is closing
  virtual void OnFinalMessage(HWND hWnd);

private:

  /// clone status of data view into another data view
  void Init(CChildFrame* pChild, EViewType vt);

  /// create database view
  void CreateDbView();

  /// create table schema view
  void CreateTableView();

  /// create table content view
  void CreateDataView();

  /// refresh table data content
  void RefreshTableData();

  /// create array field view
  void CreateArrayView();

  /// refresh array field content
  void RefreshArrayData();

  /// create record (detail) view
  void CreateRecordView();

  /// refresh record detail view
  void RefreshRecordData();

  /// add new record to table and edit in record view
  void AddNewRecord();

  /// delete record from table and update views
  void DeleteSelectedRecord();

  /// return value of hex encoded string (e.g. oid)
  int FromHex(CString str);

  /// return content of field with index fieldIdx as string
  void GetItemValue(CStdString& value, TFieldDef* f, char* buffer);
  bool GetRefItemValue(CStdString& value, TFieldDef* f);

  // convert string value into field format
  bool SetItemValue(CStdString value, int fieldIdx);

  /// store field descriptions into m_fieldDefs
  bool CChildFrame::GetFieldDefs(CStdString table, 
    int& fieldCnt, int& bufferSize, vector<TFieldDef>& fieldDefs, bool setColumns);

  /// check for field being edited and store if changed
  void UpdateField();

  /// check change flag
  bool HasChanged();

  void FillList(CStdString table);

  /// edit / transaction mode
  static bool s_editing;

  /// temporary field buffer
  static char m_fieldBuffer[FIELD_BUFFER_SIZE];

  /// parent window of all views
  static CWindow m_mdiClient;

  /// list view showing content
  CDbrowseView m_view;

  /// mode (what content is displayed) for view
  EViewType m_vt;

  /// database (internal) name
  CStdString m_dbName;

  /// database storage file path name
  CStdString m_dbFilePath;

  /// table from which schema or content is showed
  CStdString m_dbTable;

  /// table used for detail view of references
  CStdString m_refTable;

  /// query "where" condition
  CStdString m_queryCond;

  /// handle of used database
  int m_dbHandle;

  /// record buffer
  char* m_buffer;

  /// record buffer size
  int m_bufferSize;

  /// field definitions for all table fields
  vector<TFieldDef> m_fieldDefs;

  /// object id of showed record or array field
  int m_currentOid;

  /// number of columns for table
  int m_fieldCnt;

  /// current edited field (or -1)
  int m_currentEdit;

  /// type, size and record offset of current array-field
  TFieldDef m_currentField;

  /// window handle for view containing the database
  CWindow m_dbWindow;

  /// field editor
  CEdit m_fieldEdit;

  /// reference selector
  CComboBox m_refSelector;

  /// all views that are related to this database view
  list<CWindow> m_chieldViews;

  /// list of all views used to notify them e.g. about transaction state
  static list<CChildFrame*> m_allViews;

public:
  BEGIN_MSG_MAP(CChildFrame)
  MESSAGE_HANDLER(WM_CREATE, OnCreate)
  MESSAGE_HANDLER(WM_FORWARDMSG, OnForwardMsg)
  MESSAGE_HANDLER(WM_REGISTERVIEW, OnRegisterView)
  MESSAGE_HANDLER(WM_REFRESH_DATA, OnRefreshData)
  MESSAGE_HANDLER(WM_SETFOCUS, OnActivate)
  COMMAND_ID_HANDLER(ID_SAVE, OnSave)
  COMMAND_ID_HANDLER(ID_NEWRECORD, OnNewRecord)
  COMMAND_ID_HANDLER(ID_DELETERECORD, OnDeleteRecord)

  CHAIN_MSG_MAP(CMDIChildWindowImpl<CChildFrame>)
  NOTIFY_CODE_HANDLER(LVN_ITEMACTIVATE, OnItemActivated)
  NOTIFY_CODE_HANDLER(NM_RCLICK, OnItemRClick)
  END_MSG_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDFRM_H__109973F8_9A5F_4FCA_8BEB_F3180D13B164__INCLUDED_)
