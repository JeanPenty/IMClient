#pragma once

class CLoginDlg : public SHostWnd
{
public:
	struct IListen 
	{
		virtual void ILoginDlgLoginCB() = 0;
	};
public:
	CLoginDlg(IListen* pListen);
	~CLoginDlg();

	//定义一个登录方式
	enum LoginType
	{
		ENUM_LOGINTYPE_ACC = 0,  //账号登录
		ENUM_LOGINTYPE_QR,		//扫码登录
		ENUM_LOGINTYPE_SMS,		//短信验证码登录
	};

protected:
	BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);
	void OnCommand(UINT uNotifyCode, int nID, HWND wndCtl);
	void OnTimer(UINT_PTR idEvent);
	void OnKeyDown(TCHAR nChar, UINT nRepCnt, UINT nFlags);

	void OnBnClickMin();
	void OnBnClickClose();
	void OnBnClickTool();
	void OnBnClickToolDot();

	void OnBnClickQQLogin();
	void OnBnClickAccLogin();

	void OnBnClickChkSMSLogin();
	void OnBnClickChkACCLogin();

	void OnBnClickPwdVisible();
	void OnBnClickPwdInVisible();

	void OnBnClickOnline();
	void OnBnClickLeave();

	void OnBnClickGetSMS();
	void OnBnClickLogin();

	EVENT_MAP_BEGIN()
		EVENT_NAME_COMMAND(L"btn_min", OnBnClickMin)
		EVENT_NAME_COMMAND(L"btn_close", OnBnClickClose)

		EVENT_NAME_COMMAND(L"btn_qrlogin", OnBnClickQQLogin)
		EVENT_NAME_COMMAND(L"btn_acclogin", OnBnClickAccLogin)
		EVENT_NAME_COMMAND(L"check_sms_login", OnBnClickChkSMSLogin)
		EVENT_NAME_COMMAND(L"check_acc_login", OnBnClickChkACCLogin)
		EVENT_NAME_COMMAND(L"btn_pwdvisible", OnBnClickPwdVisible)
		EVENT_NAME_COMMAND(L"btn_pwdinvisible", OnBnClickPwdInVisible)
		EVENT_NAME_COMMAND(L"btn_online", OnBnClickOnline)
		EVENT_NAME_COMMAND(L"btn_leave", OnBnClickLeave)
		EVENT_NAME_COMMAND(L"btn_getsms", OnBnClickGetSMS)
		EVENT_NAME_COMMAND(L"btn_login", OnBnClickLogin)
	EVENT_MAP_END()

	BEGIN_MSG_MAP_EX(CLoginDlg)
		MSG_WM_INITDIALOG(OnInitDialog)
		MSG_WM_COMMAND(OnCommand)
		MSG_WM_TIMER(OnTimer)
		MSG_WM_KEYDOWN(OnKeyDown)

	CHAIN_MSG_MAP(SHostWnd)
	REFLECT_NOTIFICATIONS_EX()
	END_MSG_MAP()

private:
	LoginType m_enLoginType;

	IListen* m_pListen;
};

