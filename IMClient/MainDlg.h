// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////
#pragma once

#include "CLoginDlg.h"
#include "CImDlg.h"

class CMainDlg : public SHostWnd
	, public TAutoEventMapReg<CMainDlg>  //通知中心事件自动注册
	, public CLoginDlg::IListen
	, public CImDlg::IListen
{
public:
	CMainDlg();
	~CMainDlg();

	void OnClose();
	void OnMaximize();
	void OnRestore();
	void OnMinimize();
	void OnSize(UINT nType, CSize size);
	//托盘通知消息处理函数
	//LRESULT OnIconNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL/* bHandled*/);
	//演示如何响应菜单事件
	void OnCommand(UINT uNotifyCode, int nID, HWND wndCtl);
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);

protected:
	void OnLanguage(int nID);
	void OnLanguageBtnCN();
	void OnLanguageBtnJP();
	//soui消息
	EVENT_MAP_BEGIN()
		EVENT_NAME_COMMAND(L"btn_close", OnClose)
		EVENT_NAME_COMMAND(L"btn_min", OnMinimize)
		EVENT_NAME_COMMAND(L"btn_max", OnMaximize)
		EVENT_NAME_COMMAND(L"btn_restore", OnRestore)
		EVENT_NAME_COMMAND(L"zh_cn", OnLanguageBtnCN)
		EVENT_NAME_COMMAND(L"jp", OnLanguageBtnJP)

		EVENT_HANDLER(EVT_SNAPSHOTFINISH, OnEventSnapshotFinish)
		EVENT_HANDLER(EVT_SNAPSHOTCANCEL, OnEventSnapshotCancel)
	EVENT_MAP_END()
		
	//HostWnd真实窗口消息处理
	BEGIN_MSG_MAP_EX(CMainDlg)
		MSG_WM_CREATE(OnCreate)
		MSG_WM_INITDIALOG(OnInitDialog)
		MSG_WM_CLOSE(OnClose)
		MSG_WM_SIZE(OnSize)
	//托盘消息处理
		//MESSAGE_HANDLER(WM_ICONNOTIFY, OnIconNotify)
		MSG_WM_COMMAND(OnCommand)
		CHAIN_MSG_MAP(SHostWnd)
		REFLECT_NOTIFICATIONS_EX()
	END_MSG_MAP()
private:
	BOOL			m_bLayoutInited;	

	CLoginDlg* m_pLoginDlg;
	CImDlg* m_pImDlg;
public:
	virtual void ILoginDlgLoginCB();

protected:
	bool OnEventSnapshotFinish(EventArgs* e);
	bool OnEventSnapshotCancel(EventArgs* e);
};
