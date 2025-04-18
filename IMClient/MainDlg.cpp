// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainDlg.h"	
	
CMainDlg::CMainDlg() : SHostWnd(_T("LAYOUT:XML_MAINWND"))
{
	m_bLayoutInited = FALSE;

	m_pLoginDlg = NULL;
	m_pImDlg = NULL;

	SNotifyCenter::getSingleton().addEvent(EVENTID(EventSnapshotFinish));
	SNotifyCenter::getSingleton().addEvent(EVENTID(EventSnapshotCancel));
}

CMainDlg::~CMainDlg()
{
}

int CMainDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	SetMsgHandled(FALSE);
	return 0;
}

BOOL CMainDlg::OnInitDialog(HWND hWnd, LPARAM lParam)
{
	m_bLayoutInited = TRUE;
// 	SShellNotifyIcon *notify = FindChildByID2<SShellNotifyIcon>(110);
// 	notify->ShowNotify(L"Hello SOUI",L"这可能是一个提示");

	if (m_pLoginDlg == NULL)
	{
		m_pLoginDlg = new CLoginDlg(this);
		m_pLoginDlg->Create(this->m_hWnd, 0, 0, 0, 0);
		m_pLoginDlg->SendMessage(WM_INITDIALOG);
	}
	m_pLoginDlg->ShowWindow(SW_SHOWNORMAL);

	return 0;
}

void CMainDlg::OnLanguageBtnCN()
{
	OnLanguage(1);
}
void CMainDlg::OnLanguageBtnJP()
{
	OnLanguage(0);
}
void CMainDlg::OnLanguage(int nID)
{
	ITranslatorMgr *pTransMgr = SApplication::getSingletonPtr()->GetTranslator();
	SASSERT(pTransMgr);
	bool bCnLang = nID == 1;

	pugi::xml_document xmlLang;
	if (SApplication::getSingletonPtr()->LoadXmlDocment(xmlLang, bCnLang ? _T("lang_cn") : _T("lang_jp"), _T("translator")))
	{
		CAutoRefPtr<ITranslator> lang;
		pTransMgr->CreateTranslator(&lang);
		lang->Load(&xmlLang.child(L"language"), 1);//1=LD_XML
		TCHAR lngName[TR_MAX_NAME_LEN] = {0};
		lang->GetName(lngName);
		pTransMgr->SetLanguage(lngName);
		pTransMgr->InstallTranslator(lang);
		SDispatchMessage(UM_SETLANGUAGE,0,0);
	}
}

//TODO:消息映射
void CMainDlg::OnClose()
{
	CSimpleWnd::DestroyWindow();
}

void CMainDlg::OnMaximize()
{
	SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE);
}
void CMainDlg::OnRestore()
{
	SendMessage(WM_SYSCOMMAND, SC_RESTORE);
}
void CMainDlg::OnMinimize()
{
	SendMessage(WM_SYSCOMMAND, SC_MINIMIZE);
}

void CMainDlg::OnSize(UINT nType, CSize size)
{
	SetMsgHandled(FALSE);
	if (!m_bLayoutInited) return;
	
	SWindow *pBtnMax = FindChildByName(L"btn_max");
	SWindow *pBtnRestore = FindChildByName(L"btn_restore");
	if(!pBtnMax || !pBtnRestore) return;
	
	if (nType == SIZE_MAXIMIZED)
	{
		pBtnRestore->SetVisible(TRUE);
		pBtnMax->SetVisible(FALSE);
	}
	else if (nType == SIZE_RESTORED)
	{
		pBtnRestore->SetVisible(FALSE);
		pBtnMax->SetVisible(TRUE);
	}
}


//演示如何响应菜单事件
void CMainDlg::OnCommand(UINT uNotifyCode, int nID, HWND wndCtl)
{
	if (uNotifyCode == 0)
	{
		switch (nID)
		{
		case 6:
			PostMessage(WM_CLOSE);
			break;
		default:
			break;
		}
	}
}

void CMainDlg::ILoginDlgLoginCB()
{
	int iii = 0;

	//登录窗口点击登录，切换到IM窗口
	m_pLoginDlg->ShowWindow(SW_HIDE);

	//创建IM主窗口
	if (m_pImDlg == NULL)
	{
		m_pImDlg = new CImDlg(this);
		m_pImDlg->Create(this->m_hWnd, 0, 0, 0, 0);
		m_pImDlg->SendMessage(WM_INITDIALOG);
	}
	m_pImDlg->ShowWindow(SW_SHOWNORMAL);

}

bool CMainDlg::OnEventSnapshotFinish(EventArgs* e)
{
	if (m_pImDlg)
		m_pImDlg->SnapshotFinished();

	return true;
}

bool CMainDlg::OnEventSnapshotCancel(EventArgs* e)
{
	if (m_pImDlg)
		m_pImDlg->SnapshotCancel();
	return true;
}