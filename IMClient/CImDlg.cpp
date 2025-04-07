#include "stdafx.h"
#include "CImDlg.h"

CImDlg::CImDlg(IListen* pListen)
	: SHostWnd(_T("LAYOUT:XML_DLG_IM"))
	, m_pListen(pListen)
{
	m_pLvMessageAdapter = NULL;
	m_pTvContactAdapter = NULL;
	m_pLvNewFriendAdapter = NULL;
	m_pTileViewGZHAdapter = NULL;
	m_pTileViewDYHAdapter = NULL;
	m_pTileViewGrpMbrAdapter = NULL;
}

CImDlg::~CImDlg(void)
{
	//
}

BOOL CImDlg::OnInitDialog(HWND wndFocus, LPARAM lInitParam)
{
	SetMsgHandled(FALSE);

	if (!ImageProvider::IsExist(L"default_portrait"))
	{
		SAntialiasSkin* pSkin = new SAntialiasSkin();
		pSkin->SetRound(TRUE);

		if (pSkin->LoadFromFile(L"default_res\\default_portrait.png"))
			ImageProvider::Insert(L"default_portrait", pSkin);
		else
			delete pSkin;
	}

	::DragAcceptFiles(m_hWnd, TRUE);
	::RegisterDragDrop(m_hWnd, GetDropTarget());

	SImageButton* pBtnMessage = FindChildByName2<SImageButton>(L"btn_message");
	SImageButton* pBtnContact = FindChildByName2<SImageButton>(L"btn_contact");
	SImageButton* pBtnCollect = FindChildByName2<SImageButton>(L"btn_collect");
	SASSERT(pBtnMessage);
	SASSERT(pBtnContact);
	SASSERT(pBtnCollect);
	pBtnMessage->SetCheck(TRUE);
	pBtnContact->SetCheck(FALSE);
	pBtnCollect->SetCheck(FALSE);

	STabCtrl* pTabChatArea = FindChildByName2<STabCtrl>(L"tab_chat_area");
	SASSERT(pTabChatArea);

	//��������ػ��б��Լ�����������
	SListView* pLvMessage = FindChildByName2<SListView>(L"lv_message");
	SASSERT(pLvMessage);
	pLvMessage->EnableScrollBar(SSB_HORZ, FALSE);
	m_pLvMessageAdapter = new CLvMessageAdapter(pLvMessage, this);
	pLvMessage->SetAdapter(m_pLvMessageAdapter);
	m_pLvMessageAdapter->Release();

	//�˴���Ҫ��������Ự���б�ȥ���ģ�����������ʱĬ��Ϊû������Ự
	{
		//���һ���ļ�����
		m_pLvMessageAdapter->AddItem(0, L"file_helper");
		SStringW sstrPage;
		sstrPage.Format(L"<page title='%s'><include src='layout:XML_PAGECOMMON_FILEHELPER'/></page>", L"file_helper");
		pTabChatArea->InsertItem(sstrPage);
		SWindow* pPage = pTabChatArea->GetPage(L"file_helper", TRUE);
		SASSERT(pPage);
		SImRichEdit* pRecvRichedit = pPage->FindChildByName2<SImRichEdit>(L"recv_richedit");
		SImRichEdit* pSendRichedit = pPage->FindChildByName2<SImRichEdit>(L"send_richedit");
		SASSERT(pRecvRichedit);
		SASSERT(pSendRichedit);

		SUBSCRIBE(pRecvRichedit, EVT_RE_SCROLLBAR, CImDlg::OnRecvRichEditScrollEvent);
		SUBSCRIBE(pRecvRichedit, EVT_CTXMENU, CImDlg::OnRecvRichEditScrollEvent);
		SUBSCRIBE(pRecvRichedit, EVT_RE_OBJ, CImDlg::OnRecvRichEditObjEvent);

		SUBSCRIBE(pSendRichedit, EVT_RE_QUERY_ACCEPT, CImDlg::OnSendRichEditAcceptData);
		SUBSCRIBE(pSendRichedit, EVT_RE_NOTIFY, CImDlg::OnSendRichEditEditorChange);
		SUBSCRIBE(pSendRichedit, EVT_CTXMENU, CImDlg::OnSendRichEditMenu);
		SUBSCRIBE(pSendRichedit, EVT_RE_OBJ, CImDlg::OnRecvRichEditObjEvent);


 		AddFetchMoreBlock(pRecvRichedit);

		m_pLvMessageAdapter->SetCurSel(L"file_helper");
	}


	//���������б��Լ�����������

	//����ͨѶ¼�б��Լ�����������
	STreeView* pTreeView = FindChildByName2<STreeView>("tv_contact");
	pTreeView->EnableScrollBar(SSB_HORZ, FALSE);
	m_pTvContactAdapter = new CTvContactAdapter(this);
	pTreeView->SetAdapter(m_pTvContactAdapter);
	m_pTvContactAdapter->Release();
#pragma region ��������ϵ���б����������
	{//����µ�����
		TreeItemData team;
		team.bGroup = true;
		team.gid = 1;
		team.strName = L"�µ�����";
		m_pTvContactAdapter->AddItem(team);

		TreeItemData item;
		item.gid = team.gid;
		item.bGroup = FALSE;
		item.strID = "new_friends";
		item.strName = _T("�µ�����");
		item.strImg = _T("skin_default_personal32");
		m_pTvContactAdapter->AddItem(item);
	}

	{//��ӹ��ں�
		TreeItemData team;
		team.bGroup = true;
		team.gid = 2;
		team.strName = L"���ں�";
		m_pTvContactAdapter->AddItem(team);

		TreeItemData item;
		item.gid = team.gid;
		item.bGroup = FALSE;
		item.strID = "gzh";
		item.strName = _T("���ں�");
		item.strImg = _T("skin_default_gzh32");
		m_pTvContactAdapter->AddItem(item);
	}

	{//��Ӷ��ĺ�
		TreeItemData team;
		team.bGroup = true;
		team.gid = 3;
		team.strName = L"���ĺ�";
		m_pTvContactAdapter->AddItem(team);

		TreeItemData item;
		item.gid = team.gid;
		item.bGroup = FALSE;
		item.strID = "dyh";
		item.strName = _T("���ĺ�");
		item.strImg = _T("skin_default_dyh32");
		m_pTvContactAdapter->AddItem(item);
	}

	{//��Ӹ��˺���
		TreeItemData team;
		team.bGroup = true;
		team.gid = 4;
		team.strName = L"Ⱥ��";
		m_pTvContactAdapter->AddItem(team);
		auto iter = CGlobalUnits::GetInstance()->m_mapGroups.begin();
		for (; iter != CGlobalUnits::GetInstance()->m_mapGroups.end(); iter++)
		{
			TreeItemData item;
			item.gid = team.gid;
			item.bGroup = false;
			item.strID = iter->second.m_strGroupID;
			item.strName = S_CA2W(iter->second.m_strGroupName.c_str());
			item.strImg = _T("skin_default_group32");
			m_pTvContactAdapter->AddItem(item);
		}
	}

	{//���Ⱥ��
		TreeItemData data;
		data.bGroup = true;
		data.gid = 5;
		data.strName = L"����";
		m_pTvContactAdapter->AddItem(data);

		auto iter = CGlobalUnits::GetInstance()->m_mapPersonals.begin();
		for (; iter != CGlobalUnits::GetInstance()->m_mapPersonals.end(); iter++)
		{
			TreeItemData data1;
			data1.gid = data.gid;
			data1.bGroup = false;
			data1.strID = iter->second.m_strID;
			data1.strName = S_CA2W(iter->second.m_strName.c_str());
			data1.strImg = _T("skin_default_personal32");
			m_pTvContactAdapter->AddItem(data1);
		}
	}
#pragma endregion


	//�����������б��Լ�����������
	SListView* pLvNewFriend = FindChildByName2<SListView>(L"lv_newfrined");
	SASSERT(pLvNewFriend);
	m_pLvNewFriendAdapter = new CLvNewFriendAdapter(pLvNewFriend, this);
	pLvNewFriend->SetAdapter(m_pLvNewFriendAdapter);
	m_pLvNewFriendAdapter->Release();
#pragma region �����������������
	for (int i = 0; i < 20; i++)
	{
		SStringW sstrID;
		sstrID.Format(L"%d", i);
		SStringW sstrAvatar = L"";
		SStringW sstrName;
		sstrName.Format(L"������%d", i);
		SStringW sstrMessage;
		sstrMessage.Format(L"��� ����������%d ����һ����֤��Ϣ", i);
		int nState = 0;
		if (i == 0)
			nState = 2;
		else
			nState = i % 2;

		m_pLvNewFriendAdapter->AddItem(sstrID, sstrAvatar, sstrName, sstrMessage, nState);
	}
#pragma endregion

	//�����ں��б��Լ�����������
	STileView* pTileGZH = FindChildByName2<STileView>(L"tileview_gzh");
	SASSERT(pTileGZH);
	m_pTileViewGZHAdapter = new CGZHTileViewAdapter(this);
	pTileGZH->SetAdapter(m_pTileViewGZHAdapter);
	m_pTileViewGZHAdapter->Release();
#pragma region ������ӹ��ں�����
	for (int i = 0; i < 10; i++)
	{
		SStringW sstrID;
		sstrID.Format(L"%d", i);
		SStringW sstrName;
		sstrName.Format(L"���ں�%d", i);
		std::string strAvatar = "";
		std::string strID = S_CW2A(sstrID);
		std::string strName = S_CW2A(sstrName);
		m_pTileViewGZHAdapter->AddItem(strID, strName, strAvatar);
	}
#pragma endregion

	//�����ĺ��б��Լ�����������
	STileView* pTileDYH = FindChildByName2<STileView>(L"tileview_dyh");
	SASSERT(pTileDYH);
	m_pTileViewDYHAdapter = new CDYHTileViewAdapter(this);
	pTileDYH->SetAdapter(m_pTileViewDYHAdapter);
	m_pTileViewDYHAdapter->Release();
#pragma region ������Ӷ��ĺ�����
	for (int i = 0; i < 20; i++)
	{
		SStringW sstrID;
		sstrID.Format(L"%d", i);
		SStringW sstrName;
		sstrName.Format(L"���ĺ�%d", i);
		std::string strAvatar = "";
		std::string strID = S_CW2A(sstrID);
		std::string strName = S_CW2A(sstrName);
		m_pTileViewDYHAdapter->AddItem(strID, strName, strAvatar);
	}
#pragma endregion

	//����Ⱥ��Ա�б��Լ�����������
	STileView* pTileGrpMbr = FindChildByName2<STileView>(L"tileview_grpmbr");
	SASSERT(pTileGrpMbr);
	m_pTileViewGrpMbrAdapter = new CGrpMbrTileViewAdapter(this);
	pTileGrpMbr->SetAdapter(m_pTileViewGrpMbrAdapter);
	m_pTileViewGrpMbrAdapter->Release();
#pragma region �������Ⱥ��Ա����
	for (int i = 0; i < 40; i++)
	{
		SStringW sstrID;
		sstrID.Format(L"%d", i);
		SStringW sstrName;
		sstrName.Format(L"Ⱥ��Ա%d", i);
		std::string strAvatar = "";
		std::string strID = S_CW2A(sstrID);
		std::string strName = S_CW2A(sstrName);
		m_pTileViewGrpMbrAdapter->AddItem(strID, strName, strAvatar);
	}
#pragma endregion

	return TRUE;
}

void CImDlg::OnBnClickMessage()
{
	STabCtrl* pMainOptTab = FindChildByName2<STabCtrl>(L"tab_main_opt");
	SImageButton* pBtnMessage = FindChildByName2<SImageButton>(L"btn_message");
	SImageButton* pBtnContact = FindChildByName2<SImageButton>(L"btn_contact");
	SImageButton* pBtnCollect = FindChildByName2<SImageButton>(L"btn_collect");

	pBtnMessage->SetCheck(TRUE);
	pBtnContact->SetCheck(FALSE);
	pBtnCollect->SetCheck(FALSE);

	pMainOptTab->SetCurSel(0);
}

void CImDlg::OnBnClickContact()
{
	STabCtrl* pMainOptTab = FindChildByName2<STabCtrl>(L"tab_main_opt");

	SImageButton* pBtnMessage = FindChildByName2<SImageButton>(L"btn_message");
	SImageButton* pBtnContact = FindChildByName2<SImageButton>(L"btn_contact");
	SImageButton* pBtnCollect = FindChildByName2<SImageButton>(L"btn_collect");

	pBtnMessage->SetCheck(FALSE);
	pBtnContact->SetCheck(TRUE);
	pBtnCollect->SetCheck(FALSE);

	pMainOptTab->SetCurSel(1);
}

void CImDlg::OnBnClickCollect()
{
	STabCtrl* pMainOptTab = FindChildByName2<STabCtrl>(L"tab_main_opt");

	SImageButton* pBtnMessage = FindChildByName2<SImageButton>(L"btn_message");
	SImageButton* pBtnContact = FindChildByName2<SImageButton>(L"btn_contact");
	SImageButton* pBtnCollect = FindChildByName2<SImageButton>(L"btn_collect");

	pBtnMessage->SetCheck(FALSE);
	pBtnContact->SetCheck(FALSE);
	pBtnCollect->SetCheck(TRUE);

	pMainOptTab->SetCurSel(2);
}

void CImDlg::OnMessageItemClick(int& nIndex)
{
	STabCtrl* pTabChatArea = FindChildByName2<STabCtrl>(L"tab_chat_area");
	CLvMessageAdapter::ItemData* pData = m_pLvMessageAdapter->GetItemData(nIndex);
	int nPage = pTabChatArea->GetPageIndex(S_CA2W(pData->m_strID.c_str()), TRUE);
	pTabChatArea->SetCurSel(nPage);

	m_pLvMessageAdapter->GetItemData(nIndex);

	CGlobalUnits::GetInstance()->m_LvMessageCurSel.m_nType = pData->m_nType;
	CGlobalUnits::GetInstance()->m_LvMessageCurSel.m_strID = pData->m_strID;
}

void CImDlg::OnMessageItemRClick(int& nIndex)
{
	//��ʾ�Ҽ��˵�
	//TODO:
}

void CImDlg::ContactTVItemClick(int nGID, const std::string& strID)
{
	//����GID��ID������
	STabCtrl* pTabChatArea = FindChildByName2<STabCtrl>(L"tab_chat_area");
	if (nGID == 1) //new friend
	{
		pTabChatArea->SetCurSel(pTabChatArea->GetPageIndex(L"common_newfriend", TRUE));
	}
	else if (nGID == 2) //gzh
	{
		pTabChatArea->SetCurSel(pTabChatArea->GetPageIndex(L"common_gzh", TRUE));
	}
	else if (nGID == 3) //dyh
	{
		pTabChatArea->SetCurSel(pTabChatArea->GetPageIndex(L"common_dyh", TRUE));
	}
	else if (nGID == 4) //group
	{
		pTabChatArea->SetCurSel(pTabChatArea->GetPageIndex(L"common_group", TRUE));
	}
	else if (nGID == 5) //personal
	{
		int nIndex = pTabChatArea->GetPageIndex(L"common_personal", TRUE);
		pTabChatArea->SetCurSel(nIndex);
	}
}

void CImDlg::ContactTVItemDBClick(int nGID, const std::string& strID)
{
	if (nGID == 1 || nGID == 2 || nGID == 3) return;
	
	SStringW sstrID = S_CA2W(strID.c_str());
	if (4 == nGID)  //group
	{
		STabCtrl* pMainOptTab = FindChildByName2<STabCtrl>(L"tab_main_opt");

		SImageButton* pBtnMessage = FindChildByName2<SImageButton>(L"btn_message");
		SImageButton* pBtnContact = FindChildByName2<SImageButton>(L"btn_contact");
		SImageButton* pBtnCollect = FindChildByName2<SImageButton>(L"btn_collect");

		pBtnMessage->SetCheck(TRUE);
		pBtnContact->SetCheck(FALSE);
		pBtnCollect->SetCheck(FALSE);
		pMainOptTab->SetCurSel(0);

		m_pLvMessageAdapter->AddItem(2, sstrID);

		STabCtrl* pTabCharArea = FindChildByName2<STabCtrl>(L"tab_chat_area");
		SStringW sstrPage;
		sstrPage.Format(L"<page title='%s'><include src='layout:XML_PAGECOMMON_CHATGROUP'/></page>", sstrID);
		pTabCharArea->InsertItem(sstrPage);
		SWindow* pPage = pTabCharArea->GetPage(sstrID, TRUE);
		SASSERT(pPage);
		SImRichEdit* pRecvRichedit = pPage->FindChildByName2<SImRichEdit>(L"recv_richedit");
		SImRichEdit* pSendRichedit = pPage->FindChildByName2<SImRichEdit>(L"send_richedit");
		SASSERT(pRecvRichedit);
		SASSERT(pSendRichedit);

		SUBSCRIBE(pRecvRichedit, EVT_RE_SCROLLBAR, CImDlg::OnRecvRichEditScrollEvent);
		SUBSCRIBE(pRecvRichedit, EVT_CTXMENU, CImDlg::OnRecvRichEditScrollEvent);
		SUBSCRIBE(pRecvRichedit, EVT_RE_OBJ, CImDlg::OnRecvRichEditObjEvent);

		SUBSCRIBE(pSendRichedit, EVT_RE_QUERY_ACCEPT, CImDlg::OnSendRichEditAcceptData);
		SUBSCRIBE(pSendRichedit, EVT_RE_NOTIFY, CImDlg::OnSendRichEditEditorChange);
		SUBSCRIBE(pSendRichedit, EVT_CTXMENU, CImDlg::OnSendRichEditMenu);
		SUBSCRIBE(pSendRichedit, EVT_RE_OBJ, CImDlg::OnRecvRichEditObjEvent);

		AddFetchMoreBlock(pRecvRichedit);

		int nPage = pTabCharArea->GetPageIndex(sstrID, TRUE);
		pTabCharArea->SetCurSel(nPage);
		m_pLvMessageAdapter->SetCurSel(sstrID);

		CGlobalUnits::GetInstance()->m_LvMessageCurSel.m_nType = 2;
		CGlobalUnits::GetInstance()->m_LvMessageCurSel.m_strID = strID;
	}
	else if (5 == nGID) //Personal
	{
		STabCtrl* pMainOptTab = FindChildByName2<STabCtrl>(L"tab_main_opt");

		SImageButton* pBtnMessage = FindChildByName2<SImageButton>(L"btn_message");
		SImageButton* pBtnContact = FindChildByName2<SImageButton>(L"btn_contact");
		SImageButton* pBtnCollect = FindChildByName2<SImageButton>(L"btn_collect");

		pBtnMessage->SetCheck(TRUE);
		pBtnContact->SetCheck(FALSE);
		pBtnCollect->SetCheck(FALSE);
		pMainOptTab->SetCurSel(0);

		m_pLvMessageAdapter->AddItem(1, sstrID);

		STabCtrl* pTabCharArea = FindChildByName2<STabCtrl>(L"tab_chat_area");
		SStringW sstrPage;
		sstrPage.Format(L"<page title='%s'><include src='layout:XML_PAGECOMMON_CHATPERSONAL'/></page>", sstrID);
		pTabCharArea->InsertItem(sstrPage);
		SWindow* pPage = pTabCharArea->GetPage(sstrID, TRUE);
		SASSERT(pPage);
		SImRichEdit* pRecvRichedit = pPage->FindChildByName2<SImRichEdit>(L"recv_richedit");
		SImRichEdit* pSendRichedit = pPage->FindChildByName2<SImRichEdit>(L"send_richedit");
		SASSERT(pRecvRichedit);
		SASSERT(pSendRichedit);

		SUBSCRIBE(pRecvRichedit, EVT_RE_SCROLLBAR, CImDlg::OnRecvRichEditScrollEvent);
		SUBSCRIBE(pRecvRichedit, EVT_CTXMENU, CImDlg::OnRecvRichEditScrollEvent);
		SUBSCRIBE(pRecvRichedit, EVT_RE_OBJ, CImDlg::OnRecvRichEditObjEvent);

		SUBSCRIBE(pSendRichedit, EVT_RE_QUERY_ACCEPT, CImDlg::OnSendRichEditAcceptData);
		SUBSCRIBE(pSendRichedit, EVT_RE_NOTIFY, CImDlg::OnSendRichEditEditorChange);
		SUBSCRIBE(pSendRichedit, EVT_CTXMENU, CImDlg::OnSendRichEditMenu);
		SUBSCRIBE(pSendRichedit, EVT_RE_OBJ, CImDlg::OnRecvRichEditObjEvent);

		AddFetchMoreBlock(pRecvRichedit);

		int nPage = pTabCharArea->GetPageIndex(sstrID, TRUE);
		pTabCharArea->SetCurSel(nPage);
		m_pLvMessageAdapter->SetCurSel(sstrID);

		CGlobalUnits::GetInstance()->m_LvMessageCurSel.m_nType = 1;
		CGlobalUnits::GetInstance()->m_LvMessageCurSel.m_strID = strID;
	}
}

void CImDlg::ContactTVItemRClick(int nGID, const std::string& strID)
{
	if (nGID == 1 || nGID == 2 || nGID == 3) return;
	SStringW sstrID = S_CA2W(strID.c_str());

	SStringW sstrFont = L"face:΢���ź�,size:12";
	if (4 ==  nGID)
	{
		MenuWrapper menu(L"menu_contactlist", L"SMENU");
		menu.AddMenu(_T("������Ϣ"), 1, TRUE, FALSE, sstrFont);
		menu.AddMenu(_T("���ñ�ע"), 2, TRUE, FALSE, sstrFont);
		menu.AddMenu(_T("��ͨѶ¼��ɾ��"), 3, TRUE, FALSE, sstrFont);
		menu.AddSeperator();
		menu.AddMenu(_T("�˳�Ⱥ��"), 4, TRUE, FALSE, sstrFont);

		int ret = 0;
		POINT pt;
		::GetCursorPos(&pt);

		ret = menu.ShowMenu(TPM_RETURNCMD, pt.x, pt.y, m_hWnd);
		switch (ret)
		{
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		default:
			break;
		}
	}
	else if (5 == nGID)
	{
		MenuWrapper menu(L"menu_contactlist", L"SMENU");
		menu.AddMenu(_T("������Ϣ"), 1, TRUE, FALSE, sstrFont);
		menu.AddMenu(_T("��Ϊ�Ǳ�����"), 2, TRUE, FALSE, sstrFont);
		menu.AddMenu(_T("������Ƭ"), 3, TRUE, FALSE, sstrFont);
		menu.AddSeperator();
		menu.AddMenu(_T("ɾ������"), 4, TRUE, FALSE, sstrFont);

		int ret = 0;
		POINT pt;
		::GetCursorPos(&pt);

		ret = menu.ShowMenu(TPM_RETURNCMD, pt.x, pt.y, m_hWnd);
		switch (ret)
		{
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		default:
			break;
		}
	}
}

void CImDlg::OnBnClickSend()
{

// 	CHARRANGE chr = { 0, -1 };
// 	SStringW strContent = pSendRichedit->GetSelectedContent(&chr);
// 	pugi::xml_document doc;
// 	if (!doc.load_buffer(strContent, strContent.GetLength() * sizeof(WCHAR)))
// 		return;
// 
// 	int nIncrement = 0;
// 	pugi::xml_node node = doc.child(L"RichEditContent").first_child();
// 	for (; node; node = node.next_sibling())
// 	{
// 	}

	STabCtrl* pTabChatArea = FindChildByName2<STabCtrl>(L"tab_chat_area");
	SWindow* pPage = pTabChatArea->GetPage(S_CA2W(CGlobalUnits::GetInstance()->m_LvMessageCurSel.m_strID.c_str()));

	SImRichEdit* pRecvRichedit = pPage->FindChildByName2<SImRichEdit>(L"recv_richedit");
	SImRichEdit* pSendRichedit = pPage->FindChildByName2<SImRichEdit>(L"send_richedit");
	SStringW sstrTmp = pSendRichedit->GetWindowTextW();

	//���Է�����Ϣ����ʱ��
	SYSTEMTIME lpsystime;
	GetLocalTime(&lpsystime);
	time_t tt = time(NULL);
	tm* t = localtime(&tt);
	SStringT sstrTempTime;
	if (t->tm_hour > 12)
		sstrTempTime.Format(L"%02d��%02d��  ����%02d:%02d", t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min);
	else
		sstrTempTime.Format(L"%02d��%02d��  ����%02d:%02d", t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min);
	SStringT sstrTime;
	sstrTime.Format(L"<text font-size=\"8\" color=\"#808080\">%s</text>", sstrTempTime);
	SStringW content;
	content.Format(
		L"<RichEditContent type=\"ContentCenter\" >"
		L"<para margin=\"100,15,100,0\" align=\"center\" break=\"1\" >"
		L"%s"
		L"</para>"
		L"</RichEditContent>", sstrTime);
	pRecvRichedit->InsertContent(content, RECONTENT_LAST);

	//���Է�����Ϣ
	SStringW sstrTemp = sstrTmp;
	SStringW sstrText, sstrRevoke;
	sstrText.Format(L"<text font-size=\"10\" font-face=\"΢���ź�\" color=\"#000000\"><![CDATA[%s]]></text>", sstrTemp);
	sstrRevoke.Format(L"<text font-size=\"10\" font-face=\"΢���ź�\" color=\"#333333\"><![CDATA[%s]]></text>", sstrTemp);

	LPCWSTR pEmpty;
	pEmpty = L"<para id=\"msgbody\" margin=\"0,0,0,0\" break=\"1\" simulate-align=\"1\">"
		L""
		L"</para>";

	SStringW sstrContent;
	sstrContent.Format(
		L"<RichEditContent msgtype=\"text\" talk_type=\"ac_chatroom\" type=\"ContentRight\" align=\"right\" auto-layout=\"1\">"
		L"<para break=\"1\" align=\"center\" />"
		L"<bkele data=\"avatar\" right-pos=\"-50,]-10,@40,@40\" cursor=\"hand\" interactive=\"1\"/>"
		L"<para id=\"msgbody\" margin=\"65,0,45,0\" break=\"1\" simulate-align=\"1\">"
		L"%s"
		L"</para>"
		L"<bkele data=\"bubble\" right-skin=\"skin_right_bubble\" right-pos=\"{-10,{-9,-55,[10\" />"
		L"%s"
		L"%s"
		L"</RichEditContent>", sstrText, pEmpty, pEmpty);
	pRecvRichedit->InsertContent(sstrContent, RECONTENT_LAST);

	pSendRichedit->Clear();

	//��Ϣ����֮����Ҫ��������Ự�б��е���Ϣ���ݡ���Ϣʱ�����Ϣ
	//TODO:
}

bool CImDlg::OnSendRichEditAcceptData(EventArgs* pEvt)
{
	EventQueryAccept* e = (EventQueryAccept*)pEvt;
	if (e->Conv->GetAvtiveFormat() == CF_HDROP)
	{
		::SetForegroundWindow(m_hWnd);
		RichFormatConv::DropFiles files = e->Conv->GetDropFiles();
		DragDropFiles(files);
		return true;
	}

	return true;
}

bool CImDlg::OnSendRichEditEditorChange(EventArgs* pEvt)
{
	EventRENotify* pReNotify = (EventRENotify*)pEvt;
	if (pReNotify->iNotify == EN_CHANGE)
	{
		//�˴���Ҫ�������������@ʱչʾȺ��Ա�б�
	}

	return true;
}

bool CImDlg::OnSendRichEditMenu(EventArgs* pEvt)
{
	//�����Ϳ���Ҽ��˵�
// 	SImRichEdit* pSendRichEdit = FindChildByName2<SImRichEdit>(L"send_richedit");
// 	SASSERT(pSendRichEdit);
// 
// 	CHARRANGE chrSel;
// 	pSendRichEdit->GetSel(&chrSel.cpMin, &chrSel.cpMax);
// 	RichEditOleBase* pOle = pSendRichEdit->GetSelectedOle();

	return true;
}

bool CImDlg::OnSendRichEditObjEvent(EventArgs* pEvt)
{
	EventRichEditObj* pev = (EventRichEditObj*)pEvt;
	//�����Ϳ���oleԪ�ص��¼����������ͼƬole˫������ʹ��ͼƬԤ��չʾ��ǰͼƬ
	switch (pev->SubEventId)
	{
// 	case DBLCLICK_IMAGEOLE:
// 	{
// 		RichEditImageOle* pImageOle = sobj_cast<RichEditImageOle>(pev->RichObj);
// 		//ShellExecute(NULL, _T("open"), pImageOle->GetImagePath(), NULL, NULL, SW_SHOW);
// 	}
// 	break;
	default:
		break;
	}
	return true;
}

bool CImDlg::OnRecvRichEditScrollEvent(EventArgs* pEvt)
{
	EventRichEditScroll* pSBEvt = (EventRichEditScroll*)pEvt;
	SASSERT(pSBEvt);

	SImRichEdit* pRecvRichedit = FindChildByName2<SImRichEdit>(L"recv_richedit");
	SASSERT(pRecvRichedit);
	time_t now = time(NULL);
	if (pSBEvt->ScrollAtTop && (now - _lastWhellTime) < 200)
	{
		_totalWhellDelta += pSBEvt->WheelDelta;
	}
	else
	{
		_totalWhellDelta = 0;
		if (pSBEvt->ScrollAtBottom)
		{
			//չʾ�·��Ƿ�������Ϣ
// 			SWindow* pShowNewMessageWnd = FindChildByName2<SWindow>(L"show_new_message");
// 			if (pShowNewMessageWnd)
// 				pShowNewMessageWnd->SetVisible(FALSE, TRUE);
		}
	}

	_lastWhellTime = now;

	if (_totalWhellDelta > 3)
	{
		RichEditContent* pContent = pRecvRichedit->GetContent(UINT(0));
		if (pContent)
		{
			RichEditFetchMoreOle* pOle = NULL;
			pOle = sobj_cast<RichEditFetchMoreOle>(pContent->GetByName(L"FetchMoreOle"));

			if (!pOle || pOle->GetCurrentState() != RichEditFetchMoreOle::REFM_STATE_NORMAL)
				return true;

			pOle->ShowLoadingState();
			
			//���������ʷ��Ϣ���������ݿ��ȡ���������������
			//TODO:
		}
	}
	return true;
}

bool CImDlg::OnRecvRichEditMenu(EventArgs* pEvt)
{
	//���տ��Ҽ��˵�
	EventCtxMenu* pev = static_cast<EventCtxMenu*>(pEvt);

	return true;
}

bool CImDlg::OnRecvRichEditObjEvent(EventArgs* pEvt)
{
	EventRichEditObj* pev = (EventRichEditObj*)pEvt;
	//������տ���oleԪ�ص��¼����������ͼƬole˫������ʹ��ͼƬԤ��չʾ��ǰͼƬ
	switch (pev->SubEventId)
	{
		// 	case DBLCLICK_IMAGEOLE:
		// 	{
		// 		RichEditImageOle* pImageOle = sobj_cast<RichEditImageOle>(pev->RichObj);
		// 		//ShellExecute(NULL, _T("open"), pImageOle->GetImagePath(), NULL, NULL, SW_SHOW);
		// 	}
		// 	break;
	default:
		break;
	}
	return true;
}

void CImDlg::DragDropFiles(RichFormatConv::DropFiles& files)
{
	SImRichEdit* pSendRichEdit = FindChildByName2<SImRichEdit>(L"send_richedit");

	RichFormatConv::DropFiles::iterator iter = files.begin();
	for (; iter != files.end(); ++iter)
	{
		SStringW strFile = *iter;
		int nFileSize;
		FILE* fp = _wfopen(strFile, L"rb");
		if (fp)
		{
			fseek(fp, 0L, SEEK_END);
			nFileSize = ftell(fp);
			rewind(fp);
			fclose(fp);
		}
		else
			return;

		SStringW sstrContent;
		sstrContent.Format(L"<RichEditContent><metafile file=\"%s\" /></RichEditContent>", *iter);
		pSendRichEdit->InsertContent(sstrContent, RECONTENT_CARET);
	}

	::SetFocus(m_hWnd);
	pSendRichEdit->SetFocus();
	pSendRichEdit->GetContainer()->OnShowCaret(TRUE);
}

void CImDlg::AddFetchMoreBlock(SImRichEdit* pRecvRichEdit)
{
	SStringW content = L"<RichEditContent type=\"ContentFetchMore\" align=\"center\">"
		L"<para name=\"para\" margin=\"0,5,0,0\" break=\"1\">"
		L"<fetchmore name=\"FetchMoreOle\" selectable=\"0\" />"
		L"</para>"
		L"</RichEditContent>";

	pRecvRichEdit->InsertContent(content);
}