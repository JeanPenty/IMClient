#pragma once

#include "extend.ctrls/imre/SImRichEdit.h"
#include "extend.ctrls/imre/ClipboardConverter.h"

#include "CLvMessageAdapter.h"
#include "CTvContactAdapter.h"
#include "CLvNewFriendAdapter.h"
#include "CGZHTileViewAdapter.h"
#include "CDYHTileViewAdapter.h"
#include "CGrpMbrTileViewAdapter.h"

#include "CEmotionTileViewAdapter.h"

class CImDlg : public SHostWnd
	, public CLvMessageAdapter::IListen
	, public CTvContactAdapter::IListen
	, public CLvNewFriendAdapter::IListen
	, public CGZHTileViewAdapter::IListen
	, public CDYHTileViewAdapter::IListen
	, public CGrpMbrTileViewAdapter::IListen
	, public CEmotionTileViewAdapter::IListener
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

	void OnBnClickEmotion(EventArgs* pEvt);
	void OnBnClickImage();
	void OnBnClickFile();
	void OnBnClickSnapshot();
	void OnBnClickSnapshotArrow();
	void OnBnClickAudio();
	void OnBnClickVideo();
	void OnBnClickLive();
	void OnBnClickAudioVideo();
protected:
	BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);

	EVENT_MAP_BEGIN()
		EVENT_NAME_COMMAND(L"btn_send", OnBnClickSend)

		EVENT_NAME_COMMAND(L"btn_message", OnBnClickMessage)
		EVENT_NAME_COMMAND(L"btn_contact", OnBnClickContact)
		EVENT_NAME_COMMAND(L"btn_collect", OnBnClickCollect)

		EVENT_NAME_HANDLER(L"btn_emotion", EventCmd::EventID, OnBnClickEmotion)
		EVENT_NAME_COMMAND(L"btn_image", OnBnClickImage)
		EVENT_NAME_COMMAND(L"btn_file", OnBnClickFile)
		EVENT_NAME_COMMAND(L"btn_snapshot", OnBnClickSnapshot)
		EVENT_NAME_COMMAND(L"btn_snapshot_arrow", OnBnClickSnapshotArrow)
		EVENT_NAME_COMMAND(L"btn_audio", OnBnClickAudio)
		EVENT_NAME_COMMAND(L"btn_video", OnBnClickVideo)
		EVENT_NAME_COMMAND(L"btn_live", OnBnClickLive)
		EVENT_NAME_COMMAND(L"btn_audio_video", OnBnClickAudioVideo)
	EVENT_MAP_END()

	LRESULT OnMenuEvent(UINT msg, WPARAM wp, LPARAM lp);
	void OnInitEmojiMenu(SMenuEx* menuPopup, UINT nIndex);

	BEGIN_MSG_MAP_EX(CImDlg)
		MSG_WM_INITDIALOG(OnInitDialog)
		MESSAGE_HANDLER_EX(UM_MENUEVENT, OnMenuEvent)
		MSG_WM_INITMENUPOPUP_EX(OnInitEmojiMenu)
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

	SMenuEx* m_pEmojiMenu;
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

	virtual void OnEmotionItemClick(const std::string& strID);
};

