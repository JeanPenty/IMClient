#include "stdafx.h"
#include "CLoginDlg.h"

CLoginDlg::CLoginDlg()
	: SHostWnd(_T("LAYOUT:XML_DLG_LOGIN"))
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
	//
}

void CLoginDlg::OnBnClickLeave()
{
	//
}

void CLoginDlg::OnBnClickGetSMS()
{
	//点击获取验证码
}

void CLoginDlg::OnBnClickLogin()
{
	//点击登录
}