#pragma once

#include "extend.ctrls/imre/SImRichEdit.h"
#include "extend.ctrls/imre/ClipboardConverter.h"

class CImDlg : public SHostWnd
{
public:
	struct IListen
	{

	};

public:
	CImDlg(IListen* pListen);
	~CImDlg(void);

	void OnBnClickSend();

protected:
	BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);

	EVENT_MAP_BEGIN()
		EVENT_NAME_COMMAND(L"btn_send", OnBnClickSend)
	EVENT_MAP_END()

	BEGIN_MSG_MAP_EX(CImDlg)
		MSG_WM_INITDIALOG(OnInitDialog)

		CHAIN_MSG_MAP(SHostWnd)
		REFLECT_NOTIFICATIONS_EX()
	END_MSG_MAP()

private:
	IListen* m_pListen;

protected:
	bool OnSendRichEditAcceptData(EventArgs* pEvt);

	void DragDropFiles(RichFormatConv::DropFiles& files);
};

