/***********************************************************
 * media transfer AG
 * 
 * Package:     FastDB Browser
 * Module:      $RCSfile: ChildFrm.cpp,v $
 * Type:        C++ Source
 * Version:     $Revision: 1.5 $
 * Last changed:
 *   Date: $Date: 2003/11/28 10:25:31 $
 *   By:   $Author: kzerbe $
 * 
 ***********************************************************/
/**
 * @file ChildFrm.h
 *
 * Database views (MDI child windows)
 */

#include "stdafx.h"
#include "resource.h"

#include "dbrowseView.h"  // content view
#include "ChildFrm.h"
#include "fastdb/cli.h"   // FastDB call level interface
#include "fastdb/exception.h"

#include "queryprompter.h"
#include "messages.h"

#define PACK_OFFS 4

// init statics

bool CChildFrame::s_editing= false;

list<CChildFrame*> CChildFrame::m_allViews;

char CChildFrame::m_fieldBuffer[FIELD_BUFFER_SIZE];

CWindow CChildFrame::m_mdiClient;

int CChildFrame::m_currentDB= NULL;



CChildFrame::CChildFrame(CStdString dbName, CStdString dbPath)
{
  m_dbHandle= 0;
  m_dbName= dbName;
  m_dbFilePath= dbPath;
  m_dbTable="";
  m_vt= eDbView;
  m_buffer= NULL;
  m_bufferSize= 0;
  m_currentOid= 0;
  m_currentEdit= -1;
}


CChildFrame::CChildFrame(int dbHandle, CStdString dbTable, CStdString dbName,
                         int oid, TFieldDef* field)
{
  m_dbHandle= dbHandle;
  m_dbName= dbName;
  m_dbFilePath= "";
  m_dbTable= dbTable;
  m_vt= eArrayView;
  m_buffer= NULL;
  m_bufferSize= 0;
  m_currentOid= oid;

  if (field)
    m_currentField= *field;

  m_currentEdit= -1;
}



void CChildFrame::OnFinalMessage(HWND hWnd)
{
  if (m_vt== eDbView && m_dbHandle>=0) // database (main) view
  {
    // tell all related windows to close
    list<CWindow>::iterator it;

    for (it= m_chieldViews.begin(); it!= m_chieldViews.end(); ++it)
    {
      CWindow& wnd= *it;

      if (wnd.IsWindow())
      {
        wnd.PostMessage(WM_CLOSE, 0,0);
      }
    }

    m_chieldViews.empty();

    // close database
    cli_close(m_dbHandle);
    m_dbHandle= 0;
  }
  else // related (dependant) view
  {
    // deregister from database view

    if (m_dbWindow.IsWindow())
    {
      m_dbWindow.PostMessage(WM_REGISTERVIEW, 0, (LPARAM)hWnd);
    }
    if (m_dbHandle) cli_commit(m_dbHandle);
  }

  // if having record buffer, destroy it
  if (m_buffer) delete[] m_buffer;

  m_allViews.remove(this);

  // destroy object
  delete this;
}

void CChildFrame::CreateDbView()
{
  CStdString title;
  m_dbWindow= *this;

  // set database view caption
  title.Format("Database: %s", m_dbName.c_str());
  SetWindowText(title);

  // set single column caption
  m_view.AddColumn("Tablename", 0,0);
  m_view.SetColumnWidth(0,200);

  // open database

  try
  {
    m_dbHandle= cli_create(m_dbName, m_dbFilePath, 0,
                           cli_open_concurrent, 8192,8192,0,0);

    if (m_dbHandle>=0)
    {
      // get tables of database
      cli_table_descriptor *td;
      int i,tabs= cli_show_tables(m_dbHandle, &td);

      // add table names as list items

      for (i=0; i<tabs; i++)
      {
        m_view.AddItem(i,0,td[i].name);
      }

      // release cli_table_descriptor resources
      cli_free_memory(m_dbHandle,td);
    }
    else // on failure to open database close view
    {
      PostMessage(WM_CLOSE, 0,0);
    }
  }
  catch (dbException exc)
  {
    MessageBox(exc.getMsg(), APP_NAME, MB_OK|MB_ICONEXCLAMATION);
    PostMessage(WM_CLOSE, 0,0);
  }
}

void CChildFrame::CreateTableView()
{
  CStdString title;
  cli_field_descriptor *fields;

  // database field types (relates to cli_var_type enumeration)
  static char* typeNames[]=  {"oid","bool","int1","int2","int4","int8","real4","real8",
                              "decimal","string","&string","cstring",
                              "oid[ ]","bool[ ]","int1[ ]","int2[ ]","int4[ ]","int8[ ]","real4[ ]","real8[ ]",
                              "decimal[ ]","string[ ]","any","datetime","auto","rect","unknown" };

  // get database field descriptions
  m_fieldCnt= cli_describe(m_dbHandle, m_dbTable, &fields);

  // if no fields or failure close view

  if (m_fieldCnt<=0)
  {
    PostMessage(WM_CLOSE, 0,0);
    return;
  }

  // set table schema view caption
  title.Format("Table schema: %s.%s", m_dbName.c_str(), m_dbTable.c_str());

  SetWindowText(title);

  // set column titles
  m_view.AddColumn("Fieldname", 0,0);

  m_view.SetColumnWidth(0,100);

  m_view.AddColumn("Type", 1);

  m_view.SetColumnWidth(1,50);

  m_view.AddColumn("RefTable", 2);

  m_view.SetColumnWidth(2,100);

  m_view.AddColumn("InvRefFieldname", 3);

  m_view.SetColumnWidth(3,100);

  m_view.AddColumn("Flags", 4);

  m_view.SetColumnWidth(4,50);

  // fill field descriptions into list
  for (int i=0; i< m_fieldCnt; i++)
  {
    m_view.InsertItem(i, fields[i].name);
    m_view.SetItemText(i, 1, typeNames[fields[i].type]);
    m_view.SetItemText(i, 2, fields[i].refTableName);
    m_view.SetItemText(i, 3, fields[i].inverseRefFieldName);
    CStdString flags;
    flags.Format("%04X", fields[i].flags);
    m_view.SetItemText(i, 4, flags);
  }

  // release cli_field_descriptor resources
  cli_free_memory(m_dbHandle, fields);
}

bool CChildFrame::GetRefItemValue(CStdString& value, TFieldDef* f)
{
  int fieldCnt, bufferSize;
  vector<TFieldDef> reff;
  cli_oid_t oid= *(cli_oid_t*)(m_buffer+ f->ofs);
  if (!oid || f->refTable.IsEmpty()) return false;
  if (!GetFieldDefs(f->refTable, fieldCnt, bufferSize, reff, false)) return false;
  if (!(reff[0].name.Compare("m_name")==0) && reff[0].type== cli_asciiz) return false;
  char* buf= new char[m_bufferSize];
  memset(buf, 0, m_bufferSize);
  
  CStdString query;
  query.Format(PLAIN_SQL_SELECT, f->refTable.c_str(), "current= %p");
  int statement;
  statement= cli_prepare_query(m_dbHandle, query);
  if (statement >=0 && buf && 
    cli_execute_query(statement, 0, buf, oid)>0)
  {
    cli_get_first(statement);
    GetItemValue(value, &reff[0],buf);
    cli_free(statement);
    return true;
  }

  return false;
}

void CChildFrame::GetItemValue(CStdString& value, TFieldDef* f, char* buffer)
{
  CStdString name;

  // get field content from buffer and convert to string
  switch(f->type)
  {

  case cli_oid:
#if 0
    if (GetRefItemValue(name, f))
    {
      value.Format("#%04X [%s]", *(cli_oid_t*)(buffer+ f->ofs), name.c_str());
    }
    else
#endif
    {
        value.Format("#%04X [%s]", *(cli_oid_t*)(buffer+ f->ofs), f->refTable.c_str());
    }
    break;

  case cli_int1:
    value.Format("%d", *(cli_int1_t*)(buffer+ f->ofs));
    break;

  case cli_int2:
    value.Format("%d", *(cli_int2_t*)(buffer+ f->ofs));
    break;

  case cli_autoincrement:

  case cli_int4:
    value.Format("%d", *(cli_int4_t*)(buffer+ f->ofs));
    break;

  case cli_int8:
    value.Format("%ld", *(cli_int8_t*)(buffer+ f->ofs));
    break;

  case cli_real4:
    {
      double x= *(cli_real4_t*)(buffer+ f->ofs);
      value.Format("%g", x);
    }

    break;

  case cli_real8:
    value.Format("%g", *(cli_real8_t*)(buffer+ f->ofs));
    break;

  case cli_asciiz:
    value= *(char**)(buffer+ f->ofs);
    break;

  case cli_pasciiz:
    value= **(char***)(buffer+ f->ofs);
    break;

  case cli_unknown:
    value="???";
    break;

  case cli_array_of_oid:
    value= "oid[]";
    break;

  case cli_array_of_bool:
    value= "bool[]";
    break;

  case cli_array_of_int1:
    value= "int1[]";
    break;

  case cli_array_of_int2:
    value= "int2[]";
    break;

  case cli_array_of_int4:
    value= "int4[]";
    break;

  case cli_array_of_int8:
    value= "int8[]";
    break;

  case cli_array_of_real4:
    value= "real4[]";
    break;

  case cli_array_of_real8:
    value= "real8[]";
    break;

  case cli_array_of_decimal:
    value= "decimal[]";
    break;

  case cli_array_of_string:
    value= "string[]";
    break;

  default:
    value= "*not supported*";
    break;
  }
}


bool CChildFrame::SetItemValue(CStdString value, int fieldIdx)
{
  // get field description info for field with index fieldIdx
  TFieldDef* f= &m_fieldDefs[fieldIdx];

  switch(f->type)
  {

  case cli_oid:
    *(cli_oid_t*)m_fieldBuffer= FromHex(value.Mid(1).c_str());
    break;

  case cli_int1:
    *(cli_int1_t*)m_fieldBuffer= atoi(value);
    break;

  case cli_int2:
    *(cli_int2_t*)m_fieldBuffer= atoi(value);
    break;

  case cli_autoincrement:

  case cli_int4:
    *(cli_int4_t*)m_fieldBuffer= atol(value);
    break;

  case cli_int8:
    *(cli_int8_t*)m_fieldBuffer= atol(value);
    break;

  case cli_real4:
    *(cli_real4_t*)m_fieldBuffer= atof(value);
    break;

  case cli_real8:
    *(cli_real8_t*)m_fieldBuffer= atof(value);
    break;

  case cli_asciiz:
    strcpy(m_fieldBuffer, value);
    break;

  default:
    return false;
    break;
  }

  return true;
}


bool CChildFrame::GetFieldDefs(CStdString table, 
  int& fieldCnt, int& bufferSize, vector<TFieldDef>& fieldDefs, bool setColumns)
{
  cli_field_descriptor *fields;

  // get field descriptions from database

  if ((fieldCnt= cli_describe(m_dbHandle, table, &fields)) <=0)
  {
    return false;
  }

  // clear field descriptions
  fieldDefs.empty();

  fieldDefs.resize(fieldCnt);

  for (int i= 0; i< fieldCnt; i++)
  {
    // store relevant field info
    TFieldDef* f= &fieldDefs[i];
    f->name= fields[i].name;
    f->type= fields[i].type;
    f->size= fields[i].size; 
    f->refTable= fields[i].refTableName;

    // don't set columns for detail view

    if (setColumns)
    {
      m_view.AddColumn(f->name, i+1);
      m_view.SetColumnWidth(i+1, 100);
    }

    f->ofs= fields[i].offs; 
    bufferSize= f->ofs+ f->size;
  }

  bufferSize+=20;

  // release cli_field_descriptor resources
  cli_free_memory(m_dbHandle, fields);

  return true;
}


void CChildFrame::RefreshTableData()
{
  int i,j,r,statement;
  CStdString query, value;

  m_view.DeleteAllItems();

  // prepare database query for selecting whole table
  query.Format(PLAIN_SQL_SELECT, m_dbTable.c_str(), m_queryCond.c_str());

  try
  {
    statement= cli_prepare_query(m_dbHandle, query);

    // if query fails close view

    if (statement <=0)
    {
      PostMessage(WM_CLOSE, 0,0);
      return;
    }

    // execute query
    memset(m_buffer, 0, m_bufferSize);
    r= cli_execute_query(statement, 0, m_buffer);

    if (r>0) // if any rows in query result
    {
      j=0; // row counter

      // iterate query result rows
      cli_get_first(statement);

      do
      {
        // set first column to oid
        int id= cli_get_oid(statement);
        value.Format("#%04X", id);
        m_view.InsertItem(j, value);

        // also mark list row with oid for easier reference
        m_view.SetItemData(j, id);

        // fill all columns

        for (i= 0; i< m_fieldCnt; i++)
        {
          GetItemValue(value, &m_fieldDefs[i], m_buffer);
          m_view.SetItemText(j, i+1, value);
        }

        ++j;
      }
      while (cli_get_next(statement) >=0);
    }

    cli_free(statement);
    cli_commit(m_dbHandle);
  }
  catch (dbException exc)
  {
    MessageBox(exc.getMsg(), APP_NAME, MB_OK|MB_ICONEXCLAMATION);
    PostMessage(WM_CLOSE, 0,0);
  }
}


void CChildFrame::CreateDataView()
{
  // set table content view caption
  CStdString title;
  title.Format("Table data: %s.%s", m_dbName.c_str(), m_dbTable.c_str());
  SetWindowText(title);

  // first column always shows object id
  m_view.AddColumn("Oid", 0);
  m_view.SetColumnWidth(0, 50);

  // get record structure and create record buffer

  if (!GetFieldDefs(m_dbTable, m_fieldCnt, m_bufferSize, m_fieldDefs, true))
  {
    PostMessage(WM_CLOSE, 0,0);
    return;
  }

  m_buffer= new char[m_bufferSize];

  // fill table content
  RefreshTableData();
}

void CChildFrame::RefreshArrayData()
{
  CStdString query, value, handle;

  m_view.DeleteAllItems();

  try
  {
    query.Format(PLAIN_SQL_SELECT, m_dbTable.c_str(), "current= %p");

    int i, statement;
    statement= cli_prepare_query(m_dbHandle, query);

    memset(m_buffer, 0, m_bufferSize);
    // query successfull?
    if (statement >=0 && m_buffer && cli_execute_query(statement, 0, m_buffer, m_currentOid)>0)
    {
      // seek current record
      int r= cli_seek(statement, m_currentOid);

      if (r>=0) // record found?
      {
        // get array field content
        cli_array_t arr= *(cli_array_t*)(m_buffer+ m_currentField.ofs);

        // iterate array

        for (i=0; i< arr.size; ++i)
        {
          // convert element content to string

          switch (m_currentField.type)
          {

          case cli_array_of_oid:
            value.Format("#%04X", ((cli_oid_t*)arr.data)[i]);
            break;

          case cli_array_of_bool:
            value.Format("%s", ((cli_bool_t*)arr.data)[i] ? "true":"false");
            break;

          case cli_array_of_int1:
            value.Format("%d", ((cli_int1_t*)arr.data)[i]);
            break;

          case cli_array_of_int2:
            value.Format("%d", ((cli_int2_t*)arr.data)[i]);
            break;

          case cli_array_of_int4:
            value.Format("%d", ((cli_int4_t*)arr.data)[i]);
            break;

          case cli_array_of_int8:
            value.Format("%d", ((cli_int8_t*)arr.data)[i]);
            break;

          case cli_array_of_real4:
            {
              float x= ((cli_real4_t*)arr.data)[i];
              value.Format("%g", x);
            }

            break;

          case cli_array_of_real8:
            value.Format("%g", ((cli_real8_t*)arr.data)[i]);
            break;

          case cli_array_of_string:
            {
              char *s= ((char**)arr.data)[i];
              value.Format("%s", s);
            }

            break;
          }

          // insert element into list
          m_view.InsertItem(i, value);
        }

      }
    }

    cli_free(statement);
    cli_commit(m_dbHandle);
  }
  catch (dbException exc)
  {
    MessageBox(exc.getMsg(), APP_NAME, MB_OK|MB_ICONEXCLAMATION);
    PostMessage(WM_CLOSE, 0,0);
  }
}

void CChildFrame::CreateArrayView()
{
  CStdString title;

  // set array content view caption
  title.Format("Array data: %s.%s[#%04X]", m_dbName.c_str(), m_dbTable.c_str(), m_currentOid);
  SetWindowText(title);

  // set field name as sole column title
  m_view.AddColumn(m_currentField.name, 0,0);
  m_view.SetColumnWidth(0,200);

  // create record buffer;
  m_buffer= new char[m_bufferSize];

  RefreshArrayData();
}


void CChildFrame::RefreshRecordData()
{
  CStdString query, value;
  int statement;

  try
  {
    query.Format(PLAIN_SQL_SELECT, m_dbTable.c_str(), "current= %p");
    statement= cli_prepare_query(m_dbHandle, query);

    memset(m_buffer, 0, m_bufferSize);

    // query successfull?
    if (statement >=0 && m_buffer && cli_execute_query(statement, 0, m_buffer, m_currentOid)>0)
    {
      // seek current record
      int r= cli_seek(statement, m_currentOid);

      if (r>=0)
      {
        m_view.DeleteAllItems();

        // iterate record fields

        for (int i=0; i<m_fieldCnt; ++i)
        {
          m_view.InsertItem(i, m_fieldDefs[i].name);
          GetItemValue(value, &m_fieldDefs[i], m_buffer);
          m_view.SetItemText(i, 1, value);
        }
      }
    }

    cli_free(statement);
    cli_commit(m_dbHandle);
  }
  catch (dbException exc)
  {
    MessageBox(exc.getMsg(), APP_NAME, MB_OK|MB_ICONEXCLAMATION);
    PostMessage(WM_CLOSE, 0,0);
  }
}

void CChildFrame::CreateRecordView()
{
  CStdString title;

  // set detail view caption
  title.Format("Record data: %s.%s[#%04X]", m_dbName.c_str(), m_dbTable.c_str(), m_currentOid);
  SetWindowText(title);

  // set column titles
  m_view.AddColumn("Field", 0);
  m_view.SetColumnWidth(0,100);
  m_view.AddColumn("Value", 1);
  m_view.SetColumnWidth(1,300);

  GetFieldDefs(m_dbTable, m_fieldCnt, m_bufferSize, m_fieldDefs, false);

  // create record buffer;
  m_buffer= new char[m_bufferSize];

  RefreshRecordData();
}


LRESULT CChildFrame::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
  m_allViews.push_back(this);
  m_mdiClient= m_hWndMDIClient;

  // create list view for showing content
  m_hWndClient = m_view.Create(m_hWnd, rcDefault, NULL,
                               WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN
                               | LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL,
                               WS_EX_CLIENTEDGE);

  // extend list view with grid lines, moveable columns and one click activation
  m_view.SetExtendedListViewStyle(LVS_EX_FULLROWSELECT|LVS_EX_HEADERDRAGDROP
                                  |LVS_EX_GRIDLINES|LVS_EX_ONECLICKACTIVATE);

  // for detail view create edit field and combobox

  if (m_vt== eRecordView)
  {
    CRect rc(CPoint(0,0), CSize(100,10));
    m_fieldEdit.Create(m_view, rc, NULL,
                       WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN |WS_BORDER);
    m_fieldEdit.SetFont(m_view.GetFont());

    m_refSelector.Create(m_view, rc, NULL,
                         WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN |WS_BORDER
                         | CBS_DROPDOWNLIST | CBS_SORT);
    m_refSelector.SetFont(m_view.GetFont());

  }

  // remove (potential) file extension from database name
  int pos= m_dbName.Find('.');

  if (pos!= -1)
  {
    m_dbName= m_dbName.Left(pos);
  }

  // create view
  switch(m_vt)
  {

  case eDbView:
    CreateDbView();
    break;

  case eSchemaView:
    CreateTableView();
    break;

  case eDataView:
    CreateDataView();
    break;

  case eArrayView:
    CreateArrayView();
    break;

  case eRecordView:
    CreateRecordView();
    break;

  default:
    PostMessage(WM_CLOSE, 0,0);
    break;
  }

  bHandled = FALSE;
  return 1;
}


LRESULT CChildFrame::OnForwardMsg(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
  LPMSG pMsg = (LPMSG)lParam;

  if(CMDIChildWindowImpl<CChildFrame>::PreTranslateMessage(pMsg))
    return TRUE;

  return m_view.PreTranslateMessage(pMsg);
}


LRESULT CChildFrame::OnActivate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
  m_currentDB= m_dbHandle;
  return 0;
}

void CChildFrame::Init(CChildFrame* pChild, EViewType vt)
{
  pChild->m_bufferSize= m_bufferSize;
  pChild->m_vt= vt;
  pChild->m_fieldDefs= m_fieldDefs;
  pChild->m_fieldCnt= m_fieldCnt;
  pChild->m_queryCond= m_queryCond;
}

LRESULT CChildFrame::OnItemRClick(int /*idCtrl*/, LPNMHDR pnmh, BOOL& bHandled)
{
  CMenu menu;
  CMenu menuTrackPopup;  // shortcut menu
  NMITEMACTIVATE& ia= *(NMITEMACTIVATE*)pnmh;

  if (!s_editing)
    return 0;

  if (!menu.LoadMenu(IDR_CONTEXT))
    return 0;

  switch (m_vt)
  {

  case eDataView:
    menuTrackPopup = menu.GetSubMenu(0);
    break;

  case eRecordView:
    menuTrackPopup = menu.GetSubMenu(1);
    break;

  case eArrayView:
    menuTrackPopup = menu.GetSubMenu(2);
    break;

  default:
    return 0;
  }

  CPoint pt(ia.ptAction);
  ClientToScreen(&pt);
  menuTrackPopup.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,
                                pt.x, pt.y, m_hWnd);
  return 0;
}


LRESULT CChildFrame::OnSave(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  EndEdit(true);
  return 0;
}


LRESULT CChildFrame::OnNewRecord(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  AddNewRecord();
  return 0;
}


LRESULT CChildFrame::OnDeleteRecord(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  DeleteSelectedRecord();
  return 0;
}


LRESULT CChildFrame::OnItemActivated(int /*idCtrl*/, LPNMHDR pnmh, BOOL& bHandled)
{
  NMITEMACTIVATE& ia= *(NMITEMACTIVATE*)pnmh;
  CChildFrame* pChild=0;
  TFieldDef* field;

  // get caption of selected item/subitem
  CString item;

  m_view.GetItemText(ia.iItem, ia.iSubItem, item);

  if (s_editing && m_vt== eRecordView && ia.iSubItem==1)
  {
    CRect rc;

    field=  &m_fieldDefs[ia.iItem];

    if (ia.iItem!= m_currentEdit && field->type >=cli_oid && field->type < cli_pasciiz)
    {
      if (HasChanged())
      {
        //int r= (MessageBox("Item data has changed. Save data?", APP_NAME, MB_YESNO|MB_ICONQUESTION)== IDYES) ? 1:0;
        //if (r)
        UpdateField();
        m_dbWindow.SendMessage(WM_REFRESH_DATA, 1, 0);
      }

      m_currentEdit= ia.iItem;
      m_view.GetItemRect(ia.iItem, rc, LVIR_BOUNDS);
      rc.left= m_view.GetColumnWidth(0);

      if (field->type==cli_oid)
      {
        FillList(field->refTable);
        m_fieldEdit.ShowWindow(SW_HIDE);
        m_refSelector.SetWindowPos(HWND_TOP, rc.left, rc.top, rc.Width(), rc.Height(), SWP_SHOWWINDOW);
        m_refSelector.SelectString(0,item);
      }
      else
      {
        m_refSelector.ShowWindow(SW_HIDE);
        m_fieldEdit.SetWindowPos(HWND_TOP, rc.left, rc.top, rc.Width(), rc.Height(), SWP_SHOWWINDOW);
        m_fieldEdit.SetWindowText(item);
      }
    }
  }
  else if (!item.IsEmpty() && item[0]=='#') // assume object id was clicked
  {
    CStdString table= m_dbTable;

    // if another than the oid column was clicked find reference table

    if (ia.iSubItem>0)
    {
      // for detail view use row index else column index

      if (m_vt== eRecordView)
      {
        field= &m_fieldDefs[ia.iItem];
      }
      else
      {
        field= &m_fieldDefs[ia.iSubItem-1];
      }

      // get reference table name for field
      if (!field->refTable.IsEmpty())
        table= field->refTable;
    }
    else if (m_vt== eArrayView && !m_refTable.IsEmpty())
    {
      // for array view ref table name is stored separately
      table= m_refTable;
    }

    // create new record detail view
    int oid= FromHex(item.Mid(1));
    if (oid !=0)
    {
      pChild = new CChildFrame(m_dbHandle, table, m_dbName, oid);
      Init(pChild, eRecordView);
    }

  }
  else if (m_vt == eDataView || m_vt == eRecordView) // table content view active
  {
    // for detail view use row index else column index

    if (m_vt == eRecordView)
    {
      field= &m_fieldDefs[ia.iItem];
    }
    else
    {
      field= &m_fieldDefs[ia.iSubItem-1];
    }

    // was an array field clicked?
    if (field->type >= cli_array_of_oid && field->type <= cli_array_of_string)
    {
      // create new array field view
      pChild = new CChildFrame(m_dbHandle, m_dbTable, m_dbName,
                               m_vt== eRecordView? m_currentOid : m_view.GetItemData(ia.iItem),
                               field);

      Init(pChild, eArrayView);
      pChild->m_refTable= field->refTable;
    }

  }
  else // any other view: assume tablename clicked and open new view for that
  {
    pChild = new CChildFrame(m_dbHandle, (LPCTSTR)item, m_dbName);

    // if Ctrl-Key pressed show schema, if shift do query else full content

    if (ia.uKeyFlags & LVKF_CONTROL)
    {
      pChild->m_vt= eSchemaView;
    }
    else
    {
      pChild->m_vt= eDataView;

      if (ia.uKeyFlags & LVKF_SHIFT)
      {
        CQueryPrompter dlg;

        if (dlg.DoModal()== IDOK)
        {
          pChild->m_queryCond= dlg.m_query;
        }
      }
    }
  }

  // create window if any view was created
  if (pChild)
  {
    pChild->CreateEx(GetParent());

    // register with database view
    pChild->m_dbWindow= m_dbWindow;
    m_dbWindow.PostMessage(WM_REGISTERVIEW, 1, (LPARAM)pChild->m_hWnd);
  }

  return 0;
}

LRESULT CChildFrame::OnRegisterView(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  if (wParam != 0)
  {
    m_chieldViews.push_back((HWND)lParam);
  }
  else
  {
    m_chieldViews.remove((HWND)lParam);
  }

  return 0;
}


LRESULT CChildFrame::OnRefreshData(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  list<CWindow>::iterator it;

  switch (wParam)
  {

  case 0:
    cli_abort(m_dbHandle);
    break;

  case 1:

    if (HasChanged())
      UpdateField();

    cli_commit(m_dbHandle);

    break;

  case 2:
    switch (m_vt)
    {

    case eDataView:
      RefreshTableData();
      break;

    case eArrayView:
      RefreshArrayData();
      break;

    case eRecordView:
      RefreshRecordData();
      break;
    }

    return 0;

  case 3:
    break;
  }

  for (it= m_chieldViews.begin(); it!=m_chieldViews.end(); ++it)
  {
    (*it).PostMessage(WM_REFRESH_DATA, 2, 0);
  }

  return 0;
}


int CChildFrame::FromHex(CString str)
{
  cli_int8_t dw=0;

  for (int i=0; i< str.GetLength(); ++i, dw *=16)
  {
    char c= str[i];
    if (c==' ') break;
    dw+= c & 0xf;

    if (c >'9')
      dw+= 9;
  }

  return (int)(dw/16);
}


void CChildFrame::StartEditing()
{
  list<CChildFrame*>::iterator it;

  for (it= m_allViews.begin(); it!= m_allViews.end(); ++it)
  {
    CChildFrame* view= *it;
    view->m_currentEdit=-1;
  }

  s_editing= true;
}


void CChildFrame::EndEdit(bool bCommit)
{
  list<CChildFrame*>::iterator it;

  for (it= m_allViews.begin(); it!= m_allViews.end(); ++it)
  {
    CChildFrame* view= *it;

    if (view->m_fieldEdit.IsWindow())
      view->m_fieldEdit.ShowWindow(SW_HIDE);

    if (view->m_refSelector.IsWindow())
      view->m_refSelector.ShowWindow(SW_HIDE);

    if (view->HasChanged())
      view->UpdateField();

    view->m_currentEdit=-1;

    if (view->m_vt== eDbView)
    {
      view->PostMessage(WM_REFRESH_DATA, bCommit? 1:0, 0);
    }
  }

  s_editing= false;
}


void CChildFrame::RefreshData()
{
  list<CChildFrame*>::iterator it;

  for (it= m_allViews.begin(); it!= m_allViews.end(); ++it)
  {
    CChildFrame* view= *it;

    if (view->m_vt== eDbView)
    {
      view->PostMessage(WM_REFRESH_DATA, 3, 0);
    }
  }
}


void CChildFrame::NewRecord()
{
  if (!s_editing)
    return;

  list<CChildFrame*>::iterator it;

  CChildFrame* view=NULL;

  CWindow parent= m_mdiClient.GetTopWindow();

  for (it= m_allViews.begin(); it!= m_allViews.end(); ++it)
  {
    view= *it;

    if (view->m_hWnd== parent.m_hWnd)
      break;
    else
      view=NULL;
  }

  if (view && view->m_vt== eDataView)
  {
    view->AddNewRecord();
  }
}

void CChildFrame::AddNewRecord()
{
  cli_oid_t oid;
  memset(m_buffer, '\0', m_bufferSize);
  int r= cli_insert_struct(m_dbHandle, m_dbTable, m_buffer, &oid);

  if (r== cli_ok)
  {
    // create new record detail view
    CChildFrame* pChild = new CChildFrame(m_dbHandle, m_dbTable, m_dbName, oid);

    if (pChild)
    {
      Init(pChild, eRecordView);
      pChild->CreateEx(GetParent());

      // register with database view
      pChild->m_dbWindow= m_dbWindow;
      m_dbWindow.PostMessage(WM_REGISTERVIEW, 1, (LPARAM)pChild->m_hWnd);
    }
  }
}

void CChildFrame::DeleteRecord()
{
  if (!s_editing)
    return;

  list<CChildFrame*>::iterator it;

  CChildFrame* view=NULL;

  CWindow parent= m_mdiClient.GetTopWindow();

  for (it= m_allViews.begin(); it!= m_allViews.end(); ++it)
  {
    view= *it;

    if (view->m_hWnd== parent.m_hWnd)
      break;
    else
      view=NULL;
  }

  if (view && view->m_vt== eDataView)
  {
    view->DeleteSelectedRecord();
  }
}


void CChildFrame::DeleteSelectedRecord()
{
  CString item;
  int n= m_view.GetSelectedIndex();

  if (n==-1)
    return;

  m_view.GetItemText(n,0, item);

  CStdString query;

  int objid= FromHex(item.Mid(1));

  query.Format("select * from %s where current=%d", m_dbTable.c_str(), objid);

  try
  {
    int statement= cli_prepare_query(m_dbHandle, query);

    if (statement)
    {
      int r= cli_fetch(statement, cli_for_update);
      r= cli_remove(statement);

      if (r== cli_ok)
      {
        RefreshData();
      }
    }
  }
  catch (dbException exc)
  {
    MessageBox(exc.getMsg(), APP_NAME, MB_OK|MB_ICONEXCLAMATION);
  }
}

bool CChildFrame::HasChanged()
{
  if (m_currentEdit == -1)
    return false;

  CString oldVal,newVal;

  TFieldDef& field= m_fieldDefs[m_currentEdit];

  if (field.type >cli_oid && field.type < cli_pasciiz)
  {
    int len= m_fieldEdit.GetWindowTextLength();
    m_fieldEdit.GetWindowText(newVal.GetBufferSetLength(len),len+1);
  }
  else if (field.type == cli_oid)
  {
    int len, n= m_refSelector.GetCurSel();
    len= m_refSelector.GetLBTextLen(n);

    if (len!=CB_ERR)
    {
      m_refSelector.GetLBText(n, newVal.GetBufferSetLength(len));
    }
  }

  m_view.GetItemText(m_currentEdit, 1, oldVal);
  return (oldVal!=newVal);
}


void CChildFrame::UpdateField()
{
  CStdString newVal;

  TFieldDef& field= m_fieldDefs[m_currentEdit];

  if (!(field.type >=cli_oid && field.type < cli_pasciiz))
    return;

  bool isReference= field.type==cli_oid;

  if (isReference)
  {
    int len, n= m_refSelector.GetCurSel();
    len= m_refSelector.GetLBTextLen(n);
    m_refSelector.GetLBText(n, newVal.GetBufferSetLength(len));
  }
  else
  {
    int len= m_fieldEdit.GetWindowTextLength();
    m_fieldEdit.GetWindowText(newVal.GetBufferSetLength(len),len+1);
  }


  try
  {
    CStdString query;
    query.Format(PLAIN_SQL_SELECT, m_dbTable.c_str(), "current=%oid");
    int statement= cli_statement(m_dbHandle, query);

    if (statement >0)
    {
      int size=FIELD_BUFFER_SIZE;
      cli_column(statement, field.name, field.type, &size, &m_fieldBuffer);
      cli_parameter(statement, "%oid", cli_oid, &m_currentOid);
      cli_fetch(statement, cli_for_update);
      cli_seek(statement, m_currentOid);
      SetItemValue((LPCSTR) newVal, m_currentEdit);
      cli_update(statement);
      cli_free(statement);
      cli_commit(m_dbHandle);
    }

    m_currentEdit= -1;
  }
  catch (dbException exc)
  {
    MessageBox(exc.getMsg(), APP_NAME, MB_OK|MB_ICONEXCLAMATION);
  }
}

void CChildFrame::FillList(CStdString table)
{
  try
  {
    CStdString query, value;
    query.Format(PLAIN_SQL_SELECT, table.c_str(), "");
    int statement= cli_statement(m_dbHandle, query);

    if (statement >0)
    {
      m_refSelector.ResetContent();
      int j=0,r= cli_fetch(statement, cli_view_only);
      cli_get_first(statement);

      do
      {
        int id= cli_get_oid(statement);
        value.Format("#%04X", id);
        m_refSelector.InsertString(j, value);
        m_refSelector.SetItemData(j, id);
        ++j;
      }
      while (cli_get_next(statement) >=0);
    }
  }
  catch (dbException exc)
  {
    MessageBox(exc.getMsg(), APP_NAME, MB_OK|MB_ICONEXCLAMATION);
  }
}


