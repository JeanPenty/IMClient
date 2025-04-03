#pragma once
class CImDlg : public SHostWnd
{
public:
	struct IListen
	{

	};

public:
	CImDlg(IListen* pListen);
	~CImDlg(void);

protected:
	BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);

	EVENT_MAP_BEGIN()
	EVENT_MAP_END()

	BEGIN_MSG_MAP_EX(CImDlg)
		MSG_WM_INITDIALOG(OnInitDialog)

		CHAIN_MSG_MAP(SHostWnd)
		REFLECT_NOTIFICATIONS_EX()
	END_MSG_MAP()

private:
	IListen* m_pListen;
};

