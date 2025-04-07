#pragma once

#include "extend.ctrls/imre/SImRichEdit.h"
#include "extend.ctrls/imre/ClipboardConverter.h"

#include "CLvMessageAdapter.h"
#include "CTvContactAdapter.h"
#include "CLvNewFriendAdapter.h"
#include "CGZHTileViewAdapter.h"
#include "CDYHTileViewAdapter.h"
#include "CGrpMbrTileViewAdapter.h"

class CImDlg : public SHostWnd
	, public CLvMessageAdapter::IListen
	, public CTvContactAdapter::IListen
	, public CLvNewFriendAdapter::IListen
	, public CGZHTileViewAdapter::IListen
	, public CDYHTileViewAdapter::IListen
	, public CGrpMbrTileViewAdapter::IListen
{
public:
	struct IListen
	{
	};

public:
	CImDlg(IListen* pListen);
	~CImDlg(void);

	void OnBnClickSend();

	void OnBnClickMessage();
	void OnBnClickContact();
	void OnBnClickCollect();
protected:
	BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);

	EVENT_MAP_BEGIN()
		EVENT_NAME_COMMAND(L"btn_send", OnBnClickSend)

		EVENT_NAME_COMMAND(L"btn_message", OnBnClickMessage)
		EVENT_NAME_COMMAND(L"btn_contact", OnBnClickContact)
		EVENT_NAME_COMMAND(L"btn_collect", OnBnClickCollect)
	EVENT_MAP_END()

	BEGIN_MSG_MAP_EX(CImDlg)
		MSG_WM_INITDIALOG(OnInitDialog)

		CHAIN_MSG_MAP(SHostWnd)
		REFLECT_NOTIFICATIONS_EX()
	END_MSG_MAP()

private:
	IListen* m_pListen;
	CLvMessageAdapter* m_pLvMessageAdapter;
	CTvContactAdapter* m_pTvContactAdapter;
	CLvNewFriendAdapter* m_pLvNewFriendAdapter;
	CGZHTileViewAdapter* m_pTileViewGZHAdapter;
	CDYHTileViewAdapter* m_pTileViewDYHAdapter;
	CGrpMbrTileViewAdapter* m_pTileViewGrpMbrAdapter;

	time_t	_lastWhellTime;
	int	_totalWhellDelta;
protected:
	bool OnSendRichEditAcceptData(EventArgs* pEvt);
	bool OnSendRichEditEditorChange(EventArgs* pEvt);
	bool OnSendRichEditMenu(EventArgs* pEvt);
	bool OnSendRichEditObjEvent(EventArgs* pEvt);

	bool OnRecvRichEditScrollEvent(EventArgs* pEvt);
	bool OnRecvRichEditMenu(EventArgs* pEvt);
	bool OnRecvRichEditObjEvent(EventArgs* pEvt);

	void DragDropFiles(RichFormatConv::DropFiles& files);

	void AddFetchMoreBlock(SImRichEdit* pRecvRichEdit);

public:
	virtual void OnMessageItemClick(int& nIndex);
	virtual void OnMessageItemRClick(int& nIndex);

	virtual void ContactTVItemClick(int nGID, const std::string& strID);
	virtual void ContactTVItemDBClick(int nGID, const std::string& strID);
	virtual void ContactTVItemRClick(int nGID, const std::string& strID);
};

