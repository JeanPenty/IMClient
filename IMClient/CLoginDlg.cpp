#include "stdafx.h"
#include "CLoginDlg.h"

CLoginDlg::CLoginDlg(IListen* pListen)
	: SHostWnd(_T("LAYOUT:XML_DLG_LOGIN"))
	, m_pListen(pListen)
{
	//
}

CLoginDlg::~CLoginDlg()
{
	//
}

BOOL CLoginDlg::OnInitDialog(HWND wndFocus, LPARAM lInitParam)
{
	return TRUE;
}

void CLoginDlg::OnCommand(UINT uNotifyCode, int nID, HWND wndCtl)
{
	//
}

void CLoginDlg::OnTimer(UINT_PTR idEvent)
{
	//
}

void CLoginDlg::OnKeyDown(TCHAR nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_RETURN)
	{
		BOOL ctrlDown = !(GetKeyState(nChar) & 0x8000);

		if (!ctrlDown)
		{
			OnBnClickLogin();
			return;
		}
	}

	SetMsgHandled(FALSE);
}

void CLoginDlg::OnBnClickMin()
{
	ShowWindow(SW_MINIMIZE);
}

void CLoginDlg::OnBnClickClose()
{
	AnimateHostWindow(500, AW_BLEND | AW_HIDE);
	DestroyWindow();
}

void CLoginDlg::OnBnClickTool()
{
	//
}

void CLoginDlg::OnBnClickToolDot()
{
	//
}

void CLoginDlg::OnBnClickQQLogin()
{
	STabCtrl* pTabAccQRLogin = FindChildByName2<STabCtrl>(L"tab_accqr_login");
	SASSERT(pTabAccQRLogin);
	pTabAccQRLogin->SetCurSel(1);
}

void CLoginDlg::OnBnClickAccLogin()
{
	STabCtrl* pTabAccQRLogin = FindChildByName2<STabCtrl>(L"tab_accqr_login");
	SASSERT(pTabAccQRLogin);
	pTabAccQRLogin->SetCurSel(0);
}

void CLoginDlg::OnBnClickChkSMSLogin()
{
	SCheckBox* pChkSMSLogin = FindChildByName2<SCheckBox>(L"check_sms_login");
	SCheckBox* pChkACCLogin = FindChildByName2<SCheckBox>(L"check_acc_login");

	pChkSMSLogin->SetAttribute(L"show", L"0");
	pChkACCLogin->SetAttribute(L"show", L"1");

	//短信登录时隐藏记住密码、自动登录、忘记密码
	SCheckBox* pChkRememberPwd = FindChildByName2<SCheckBox>(L"chk_remember_pwd");
	SCheckBox* pChkAutoLogin = FindChildByName2<SCheckBox>(L"chk_auto_login");
	SButton* pBtnForgetPwd = FindChildByName2<SButton>(L"btn_forget_pwd");
	pChkRememberPwd->SetAttribute(L"show", L"0");
	pChkAutoLogin->SetAttribute(L"show", L"0");
	pBtnForgetPwd->SetAttribute(L"show", L"0");

	STabCtrl* pTabAccSMSLogin = FindChildByName2<STabCtrl>(L"tab_accsms_login");
	SASSERT(pTabAccSMSLogin);
	pTabAccSMSLogin->SetCurSel(1);
}

void CLoginDlg::OnBnClickChkACCLogin()
{
	SCheckBox* pChkSMSLogin = FindChildByName2<SCheckBox>(L"check_sms_login");
	SCheckBox* pChkACCLogin = FindChildByName2<SCheckBox>(L"check_acc_login");

	pChkSMSLogin->SetAttribute(L"show", L"1");
	pChkACCLogin->SetAttribute(L"show", L"0");

	SCheckBox* pChkRememberPwd = FindChildByName2<SCheckBox>(L"chk_remember_pwd");
	SCheckBox* pChkAutoLogin = FindChildByName2<SCheckBox>(L"chk_auto_login");
	SButton* pBtnForgetPwd = FindChildByName2<SButton>(L"btn_forget_pwd");
	pChkRememberPwd->SetAttribute(L"show", L"1");
	pChkAutoLogin->SetAttribute(L"show", L"1");
	pBtnForgetPwd->SetAttribute(L"show", L"1");

	STabCtrl* pTabAccSMSLogin = FindChildByName2<STabCtrl>(L"tab_accsms_login");
	SASSERT(pTabAccSMSLogin);
	pTabAccSMSLogin->SetCurSel(0);
}

void CLoginDlg::OnBnClickPwdVisible()
{
	SImageButton* pBtnPwdVisible = FindChildByName2<SImageButton>(L"btn_pwdvisible");
	SImageButton* pBtnPwdInVisible = FindChildByName2<SImageButton>(L"btn_pwdinvisible");
	pBtnPwdVisible->SetAttribute(L"show", L"0");
	pBtnPwdInVisible->SetAttribute(L"show", L"1");

	SEdit* pEditPwd = FindChildByName2<SEdit>(L"edit_password");
	pEditPwd->SetAttribute(L"password", L"1");
}

void CLoginDlg::OnBnClickPwdInVisible()
{
	SImageButton* pBtnPwdVisible = FindChildByName2<SImageButton>(L"btn_pwdvisible");
	SImageButton* pBtnPwdInVisible = FindChildByName2<SImageButton>(L"btn_pwdinvisible");
	pBtnPwdVisible->SetAttribute(L"show", L"1");
	pBtnPwdInVisible->SetAttribute(L"show", L"0");

	SEdit* pEditPwd = FindChildByName2<SEdit>(L"edit_password");
	pEditPwd->SetAttribute(L"password", L"0");
}

void CLoginDlg::OnBnClickOnline()
{
	MenuWrapper menu(L"menu_template", L"SMENU");
	menu.AddMenu(_T("在线"), 1001, TRUE, FALSE);
	menu.AddMenu(_T("离线"), 1002, TRUE, FALSE);

	SImageButton* pBtnOnline = FindChildByName2<SImageButton>(L"btn_online");
	CRect rtBtn = pBtnOnline->GetWindowRect();
	ClientToScreen(&rtBtn);
	POINT pt;
	::GetCursorPos(&pt);
	int ret = menu.ShowMenu(TPM_RETURNCMD, rtBtn.left, rtBtn.bottom, m_hWnd);
	switch (ret)
	{
	case 1001:
	{
		SImageButton* pBtnOnline = FindChildByName2<SImageButton>(L"btn_online");
		SImageButton* pBtnLeave = FindChildByName2<SImageButton>(L"btn_leave");
		pBtnOnline->SetAttribute(L"show", L"1", FALSE);
		pBtnLeave->SetAttribute(L"show", L"0", FALSE);
	}
	break;
	case 1002:
	{
		SImageButton* pBtnOnline = FindChildByName2<SImageButton>(L"btn_online");
		SImageButton* pBtnLeave = FindChildByName2<SImageButton>(L"btn_leave");
		pBtnOnline->SetAttribute(L"show", L"0", FALSE);
		pBtnLeave->SetAttribute(L"show", L"1", FALSE);
	}
	break;
	default:
		break;
	}
}

void CLoginDlg::OnBnClickLeave()
{
	MenuWrapper menu(L"menu_template", L"SMENU");
	menu.AddMenu(_T("在线"), 1001, TRUE, FALSE);
	menu.AddMenu(_T("离线"), 1002, TRUE, FALSE);

	SImageButton* pBtnOnline = FindChildByName2<SImageButton>(L"btn_online");
	CRect rtBtn = pBtnOnline->GetWindowRect();
	ClientToScreen(&rtBtn);
	POINT pt;
	::GetCursorPos(&pt);
	int ret = menu.ShowMenu(TPM_RETURNCMD, rtBtn.left, rtBtn.bottom, m_hWnd);
	switch (ret)
	{
	case 1001:
	{
		SImageButton* pBtnOnline = FindChildByName2<SImageButton>(L"btn_online");
		SImageButton* pBtnLeave = FindChildByName2<SImageButton>(L"btn_leave");
		pBtnOnline->SetAttribute(L"show", L"1", FALSE);
		pBtnLeave->SetAttribute(L"show", L"0", FALSE);
	}
	break;
	case 1002:
	{
		SImageButton* pBtnOnline = FindChildByName2<SImageButton>(L"btn_online");
		SImageButton* pBtnLeave = FindChildByName2<SImageButton>(L"btn_leave");
		pBtnOnline->SetAttribute(L"show", L"0", FALSE);
		pBtnLeave->SetAttribute(L"show", L"1", FALSE);
	}
	break;
	default:
		break;
	}
}

void CLoginDlg::OnBnClickGetSMS()
{
	//点击获取验证码
}

void CLoginDlg::OnBnClickLogin()
{
	//点击登录

	m_pListen->ILoginDlgLoginCB();
}

HIMC g_hIMC = NULL;
bool CLoginDlg::OnEditPasswordSetFocus(EventArgs* pEvt)		//edit密码框获取到焦点时事件
{
	g_hIMC = ImmAssociateContext(this->m_hWnd, NULL);
	return true;
}
bool CLoginDlg::OnEditPasswordKillFocus(EventArgs* pEvt)	//edit密码框失去焦点时的事件
{
	ImmAssociateContext(this->m_hWnd, g_hIMC);
	return true;
}