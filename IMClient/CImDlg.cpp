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
		SUBSCRIBE(pSendRichedit, EVT_RE_OBJ, CImDlg::OnSendRichEditObjEvent);


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

	{//���Ⱥ��
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

	{//��Ӹ��˺���
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

			//����ͷ��
			std::string strFaceID = CGlobalUnits::GetInstance()->GenerateUUID();
			SStringW sstrFaceID = S_CA2W(strFaceID.c_str());
			auto iterFace = CGlobalUnits::GetInstance()->m_mapFaceIndex.find(iter->second.m_strID);
			if (iterFace != CGlobalUnits::GetInstance()->m_mapFaceIndex.end())
				CGlobalUnits::GetInstance()->m_mapFaceIndex.erase(iter->second.m_strID);
			CGlobalUnits::GetInstance()->m_mapFaceIndex.insert(std::make_pair(iter->second.m_strID, sstrFaceID));
			if (!ImageProvider::IsExist(sstrFaceID))
			{
				SAntialiasSkin* pSkin = new SAntialiasSkin();
				pSkin->SetRound(TRUE);

				SStringW sstrFacePath = L"";
				bool bDefault = true;
				if (bDefault)
				{
					//ʹ��Ĭ��ͷ��
					sstrFacePath = L"default_res\\default_portrait.png";
				}
				else
				{
					//ʹ���Զ���ͷ��
					//TODO:
				}
				if (pSkin->LoadFromFile(sstrFacePath))
					ImageProvider::Insert(sstrFaceID, pSkin);
				else
					delete pSkin;
			}
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

LRESULT CImDlg::OnMenuEvent(UINT msg, WPARAM wp, LPARAM lp) {
	EventArgs* e = (EventArgs*)lp;
	_HandleEvent(e);
	return 0;
}

void CImDlg::OnInitEmojiMenu(SMenuEx* menuPopup, UINT nIndex)
{
 	SHostWnd* pMenuHost = static_cast<SHostWnd*>(menuPopup->GetHost());
  	STileView* pTileView = pMenuHost->FindChildByName2<STileView>(L"emoji_titleview");
	if (pTileView)
	{
		CEmotionTileViewAdapter* pAdapter = new CEmotionTileViewAdapter(this);
		pTileView->SetAdapter(pAdapter);
		std::map<std::string, std::string>::iterator iter = CGlobalUnits::GetInstance()->m_mapEmojisIndex.begin();
		for (; iter != CGlobalUnits::GetInstance()->m_mapEmojisIndex.end(); iter++)
		{
			pAdapter->AddItem(iter->first.c_str());
		}
		pAdapter->Release();
		pTileView->SetSel(-1);
	}
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

	SStringW sstrID = S_CA2W(CGlobalUnits::GetInstance()->m_LvMessageCurSel.m_strID.c_str());
	STabCtrl* pTabChatArea = FindChildByName2<STabCtrl>(L"tab_chat_area");
	int nPage = pTabChatArea->GetPageIndex(sstrID, TRUE);
	pTabChatArea->SetCurSel(nPage);
	m_pLvMessageAdapter->SetCurSel(sstrID);
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

void CImDlg::OnBnClickEmotion(EventArgs* pEvt)
{
	CRect rc2(0, 0, 250, 250);
	SWindow* btn = sobj_cast<SWindow>(pEvt->sender);
	CRect rc = btn->GetWindowRect();
	ClientToScreen(&rc);

	SMenuEx menuEmoji;
	menuEmoji.LoadMenu(L"smenuex:emoji");
	menuEmoji.TrackPopupMenu(TPM_BOTTOMALIGN, rc.left, rc.top, m_hWnd, GetScale());

	m_pEmojiMenu = &menuEmoji;
}

void CImDlg::OnBnClickImage()
{
	SStringW strFile;
	CFileDialogEx openDlg(TRUE, _T("ͼƬ"), 0, 6,
		_T("ͼƬ�ļ�\0*.gif;*.bmp;*.jpg;*.png\0\0"));
	if (openDlg.DoModal() == IDOK)
	{
		strFile = openDlg.m_szFileName;
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

		SStringW str;
		str.Format(L"<RichEditContent>"
			L"<para break=\"0\" disable-layout=\"1\">"
			L"<img type=\"normal_img\" path=\"%s\" id=\"zzz\" max-size=\"\" minsize=\"\" scaring=\"1\" cursor=\"hand\" />"
			L"</para>"
			L"</RichEditContent>", strFile);

		//����currSel����page
		STabCtrl* pTabChatArea = FindChildByName2<STabCtrl>(L"tab_chat_area");
		int nPage = pTabChatArea->GetPageIndex(S_CA2W(CGlobalUnits::GetInstance()->m_LvMessageCurSel.m_strID.c_str()), TRUE);
		SWindow* pPage = pTabChatArea->GetPage(nPage);
		SImRichEdit* pSendRichedit = pPage->FindChildByName2<SImRichEdit>(L"send_richedit");
		SASSERT(pSendRichedit);
		pSendRichedit->InsertContent(str, RECONTENT_CARET);
	}
}

void CImDlg::OnBnClickFile()
{
	CFileDialogEx openDlg(TRUE, _T("�ļ�"), 0, 6,
		_T("�ļ�\0*.*\0\0"));
	if (openDlg.DoModal() == IDOK)
	{
		SStringW sstrFile = openDlg.m_szFileName;
		int nFileSize;
		FILE* fp = _wfopen(sstrFile, L"rb");
		if (fp)
		{
			fseek(fp, 0L, SEEK_END);
			nFileSize = ftell(fp);
			rewind(fp);
			fclose(fp);
		}
		else
			return;
		
		SStringW str;
		str.Format(L"<RichEditContent>"
			L"<metafile file=\"%s\" />"
			L"</RichEditContent>", sstrFile);

		//����currSel����page
		STabCtrl* pTabChatArea = FindChildByName2<STabCtrl>(L"tab_chat_area");
		int nPage = pTabChatArea->GetPageIndex(S_CA2W(CGlobalUnits::GetInstance()->m_LvMessageCurSel.m_strID.c_str()), TRUE);
		SWindow* pPage = pTabChatArea->GetPage(nPage);
		SImRichEdit* pSendRichedit = pPage->FindChildByName2<SImRichEdit>(L"send_richedit");
		SASSERT(pSendRichedit);
		pSendRichedit->InsertContent(str, RECONTENT_CARET);
	}
}

void CImDlg::OnBnClickSnapshot()
{
	//
}

void CImDlg::OnBnClickSnapshotArrow()
{
	//
}

void CImDlg::OnBnClickAudio()
{
	//
}

void CImDlg::OnBnClickVideo()
{
	//
}

void CImDlg::OnBnClickLive()
{
	//
}

void CImDlg::OnBnClickAudioVideo()
{
	//
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

	SStatic* pObjName = FindChildByName2<SStatic>(L"obj_name");
	SStringW sstrName = L"";
	switch (pData->m_nType)
	{
	case 0: //filehelper
	{
		sstrName = L"�ļ���������";
	}
	break;
	case 1: //personal
	{
		auto iter = CGlobalUnits::GetInstance()->m_mapPersonals.find(pData->m_strID);
		if (iter != CGlobalUnits::GetInstance()->m_mapPersonals.end())
		{
			sstrName = S_CA2W(iter->second.m_strName.c_str());
		}
	}
	break;
	case 2: //group
	{
		auto iter = CGlobalUnits::GetInstance()->m_mapGroups.find(pData->m_strID);
		if (iter != CGlobalUnits::GetInstance()->m_mapGroups.end())
		{
			sstrName = S_CA2W(iter->second.m_strGroupName.c_str());
		}
	}
	break;
	case 3: //dyh
	{
		sstrName = L"���ĺ�";
	}
	break;
	case 4: //����
	{
		sstrName = L"����";
	}
	break;
	case 5:
	{
		//
	}
	break;
	default:
		break;
	}
	pObjName->SetWindowTextW(sstrName);
}

void CImDlg::OnMessageItemRClick(int& nIndex)
{
	//��ʾ�Ҽ��˵�
	//Ⱥ�����ˡ����ںš����ĺš����ŵĲ˵�������ͬ����Ҫ�ֱ���
	CLvMessageAdapter::ItemData* pData = m_pLvMessageAdapter->GetItemData(nIndex);

	SStringW sstrFont = L"face:΢���ź�,size:12";
	MenuWrapper menu(L"menu_messagelist", L"SMENU");
	switch (pData->m_nType)
	{
	case 0:
	{
		//�ļ���������
		if (CGlobalUnits::GetInstance()->m_bFileHelperTopmost)
			menu.AddMenu(_T("ȡ���ö�"), 1, TRUE, FALSE, sstrFont);
		else
			menu.AddMenu(_T("�ö�"), 1, TRUE, FALSE, sstrFont);
		
		if (CGlobalUnits::GetInstance()->m_bFileHelperRead)
			menu.AddMenu(_T("��Ϊδ��"), 2, TRUE, FALSE, sstrFont);
		else
			menu.AddMenu(_T("��Ϊ�Ѷ�"), 2, TRUE, FALSE, sstrFont);
		
		if (CGlobalUnits::GetInstance()->m_bFileHelperDisturb)
			menu.AddMenu(_T("��Ϣ�����"), 3, TRUE, FALSE, sstrFont);
		else
			menu.AddMenu(_T("������Ϣ����"), 3, TRUE, FALSE, sstrFont);

		menu.AddMenu(_T("�ڶ��������д�"), 4, TRUE, FALSE, sstrFont);
		menu.AddSeperator();
		menu.AddMenu(_T("����ʾ����"), 5, TRUE, FALSE, sstrFont);
		menu.AddMenu(_T("ɾ������"), 6, TRUE, FALSE, sstrFont);

		int ret = 0;
		POINT pt;
		::GetCursorPos(&pt);

		ret = menu.ShowMenu(TPM_RETURNCMD, pt.x, pt.y, m_hWnd);
		switch (ret)
		{
		case 1:
		{
			CGlobalUnits::GetInstance()->m_bFileHelperTopmost = !CGlobalUnits::GetInstance()->m_bFileHelperTopmost;
			//��Ҫ��lvMessage�е���Ϣ���ƶ����ö�����֮��
			//TODO:
		}
		break;
		case 2:
		{
			CGlobalUnits::GetInstance()->m_bFileHelperRead = !CGlobalUnits::GetInstance()->m_bFileHelperRead;
			//��Ҫ����lvMessage�е���Ϣ��ʹ����ͷ�����Ͻǵĺ���ѱ�Ǹ���Ϣ���Ѷ�����δ��
			//TODO:
		}
		break;
		case 3:
		{
			CGlobalUnits::GetInstance()->m_bFileHelperDisturb = !CGlobalUnits::GetInstance()->m_bFileHelperDisturb;
			//��Ҫ����lvMessage�е���Ϣ���й�����Ϣ����״̬
			//TODO:
		}
		break;
		case 4:
		{
			//���´�������������չʾ������Ϣ���������չʾ��ɾ���й������������ص����ݡ�
		}
		break;
		case 5:
		{
			//����ʾ����
		}
		break;
		case 6:
		{
			//ɾ������
		}
		break;
		default:
			break;
		}
	}
	break;
	case 1:
	{
		//��������
		auto iterTopmost = CGlobalUnits::GetInstance()->m_mapPersonalTopmost.find(pData->m_strID);
		if (iterTopmost == CGlobalUnits::GetInstance()->m_mapPersonalTopmost.end())
		{
			menu.AddMenu(_T("�ö�"), 1, TRUE, FALSE, sstrFont);
			CGlobalUnits::GetInstance()->m_mapPersonalTopmost[pData->m_strID] = false;
		}
		else
		{
			if (iterTopmost->second)
				menu.AddMenu(_T("ȡ���ö�"), 1, TRUE, FALSE, sstrFont);
			else
				menu.AddMenu(_T("�ö�"), 1, TRUE, FALSE, sstrFont);
		}

		auto iterRead = CGlobalUnits::GetInstance()->m_mapPersonalRead.find(pData->m_strID);
		if (iterRead == CGlobalUnits::GetInstance()->m_mapPersonalRead.end())
		{
			menu.AddMenu(_T("��Ϊ�Ѷ�"), 2, TRUE, FALSE, sstrFont);
			CGlobalUnits::GetInstance()->m_mapPersonalRead[pData->m_strID] = false;
		}
		else
		{
			if (iterRead->second)
				menu.AddMenu(_T("��Ϊδ��"), 2, TRUE, FALSE, sstrFont);
			else
				menu.AddMenu(_T("��Ϊ�Ѷ�"), 2, TRUE, FALSE, sstrFont);
		}

		auto iterDisturb = CGlobalUnits::GetInstance()->m_mapPersonalDisturb.find(pData->m_strID);
		if (iterDisturb == CGlobalUnits::GetInstance()->m_mapPersonalDisturb.end())
		{
			menu.AddMenu(_T("������Ϣ����"), 3, TRUE, FALSE, sstrFont);
			CGlobalUnits::GetInstance()->m_mapPersonalDisturb[pData->m_strID] = false;
		}
		else
		{
			if (iterDisturb->second)
				menu.AddMenu(_T("��Ϣ�����"), 3, TRUE, FALSE, sstrFont);
			else
				menu.AddMenu(_T("������Ϣ����"), 3, TRUE, FALSE, sstrFont);
		}

		menu.AddMenu(_T("�ڶ��������д�"), 4, TRUE, FALSE, sstrFont);
		menu.AddSeperator();
		menu.AddMenu(_T("����ʾ����"), 5, TRUE, FALSE, sstrFont);
		menu.AddMenu(_T("ɾ������"), 6, TRUE, FALSE, sstrFont);

		int ret = 0;
		POINT pt;
		::GetCursorPos(&pt);

		ret = menu.ShowMenu(TPM_RETURNCMD, pt.x, pt.y, m_hWnd);
		switch (ret)
		{
		case 1:
		{
			CGlobalUnits::GetInstance()->m_mapPersonalTopmost[pData->m_strID] = !CGlobalUnits::GetInstance()->m_mapPersonalTopmost[pData->m_strID];
			//��Ҫ��lvMessage�е���Ϣ���ƶ����ö�����֮��
			//TODO:
		}
		break;
		case 2:
		{
			CGlobalUnits::GetInstance()->m_mapPersonalRead[pData->m_strID] = !CGlobalUnits::GetInstance()->m_mapPersonalRead[pData->m_strID];
			//��Ҫ����lvMessage�е���Ϣ��ʹ����ͷ�����Ͻǵĺ���ѱ�Ǹ���Ϣ���Ѷ�����δ��
			//TODO:
		}
		break;
		case 3:
		{
			CGlobalUnits::GetInstance()->m_mapPersonalDisturb[pData->m_strID] = !CGlobalUnits::GetInstance()->m_mapPersonalDisturb[pData->m_strID];
			//��Ҫ����lvMessage�е���Ϣ���й�����Ϣ����״̬
			//TODO:
		}
		break;
		case 4:
		{
			//���´�������������չʾ������Ϣ���������չʾ��ɾ���й������������ص����ݡ�
		}
		break;
		case 5:
		{
			//����ʾ����
		}
		break;
		case 6:
		{
			//ɾ������
		}
		break;
		default:
			break;
		}
	}
	break;
	case 2:
	{
		//��������
		auto iterTopmost = CGlobalUnits::GetInstance()->m_mapGroupTopmost.find(pData->m_strID);
		if (iterTopmost == CGlobalUnits::GetInstance()->m_mapGroupTopmost.end())
		{
			menu.AddMenu(_T("�ö�"), 1, TRUE, FALSE, sstrFont);
			CGlobalUnits::GetInstance()->m_mapGroupTopmost[pData->m_strID] = false;
		}
		else
		{
			if (iterTopmost->second)
				menu.AddMenu(_T("ȡ���ö�"), 1, TRUE, FALSE, sstrFont);
			else
				menu.AddMenu(_T("�ö�"), 1, TRUE, FALSE, sstrFont);
		}

		auto iterRead = CGlobalUnits::GetInstance()->m_mapGroupRead.find(pData->m_strID);
		if (iterRead == CGlobalUnits::GetInstance()->m_mapGroupRead.end())
		{
			menu.AddMenu(_T("��Ϊ�Ѷ�"), 2, TRUE, FALSE, sstrFont);
			CGlobalUnits::GetInstance()->m_mapGroupRead[pData->m_strID] = false;
		}
		else
		{
			if (iterRead->second)
				menu.AddMenu(_T("��Ϊδ��"), 2, TRUE, FALSE, sstrFont);
			else
				menu.AddMenu(_T("��Ϊ�Ѷ�"), 2, TRUE, FALSE, sstrFont);
		}

		auto iterDisturb = CGlobalUnits::GetInstance()->m_mapGroupDisturb.find(pData->m_strID);
		if (iterDisturb == CGlobalUnits::GetInstance()->m_mapGroupDisturb.end())
		{
			menu.AddMenu(_T("������Ϣ����"), 3, TRUE, FALSE, sstrFont);
			CGlobalUnits::GetInstance()->m_mapGroupDisturb[pData->m_strID] = false;
		}
		else
		{
			if (iterDisturb->second)
				menu.AddMenu(_T("��Ϣ�����"), 3, TRUE, FALSE, sstrFont);
			else
				menu.AddMenu(_T("������Ϣ����"), 3, TRUE, FALSE, sstrFont);
		}

		menu.AddMenu(_T("�޸�Ⱥ������"), 4, TRUE, FALSE, sstrFont);
		menu.AddMenu(_T("���ñ�ע"), 5, TRUE, FALSE, sstrFont);
		menu.AddMenu(_T("�ڶ��������д�"), 6, TRUE, FALSE, sstrFont);
		menu.AddMenu(_T("���浽ͨѶ¼"), 7, TRUE, FALSE, sstrFont);   //��Ҫ�ж��Ƿ��Ѿ����浽ͨѶ¼��

		menu.AddSeperator();
		menu.AddMenu(_T("����ʾ����"), 8, TRUE, FALSE, sstrFont);
		menu.AddMenu(_T("ɾ������"), 9, TRUE, FALSE, sstrFont);

		int ret = 0;
		POINT pt;
		::GetCursorPos(&pt);

		ret = menu.ShowMenu(TPM_RETURNCMD, pt.x, pt.y, m_hWnd);
		switch (ret)
		{
		case 1:
		{
			CGlobalUnits::GetInstance()->m_mapGroupTopmost[pData->m_strID] = !CGlobalUnits::GetInstance()->m_mapGroupTopmost[pData->m_strID];
			//��Ҫ��lvMessage�е���Ϣ���ƶ����ö�����֮��
			//TODO:
		}
		break;
		case 2:
		{
			CGlobalUnits::GetInstance()->m_mapGroupRead[pData->m_strID] = !CGlobalUnits::GetInstance()->m_mapGroupRead[pData->m_strID];
			//��Ҫ����lvMessage�е���Ϣ��ʹ����ͷ�����Ͻǵĺ���ѱ�Ǹ���Ϣ���Ѷ�����δ��
			//TODO:
		}
		break;
		case 3:
		{
			CGlobalUnits::GetInstance()->m_mapGroupDisturb[pData->m_strID] = !CGlobalUnits::GetInstance()->m_mapGroupDisturb[pData->m_strID];
			//��Ҫ����lvMessage�е���Ϣ���й�����Ϣ����״̬
			//TODO:
		}
		break;
		default:
			break;
		}
	}
	break;
	case 3:
	{
		//���ĺ�
	}
	break;
	case 4:
	{
		//����
	}
	break;
	case 5:
	{
		//���ں�
	}
	break;
	default:
		break;
	}

	m_pLvMessageAdapter->Update();
}

void CImDlg::ContactTVItemClick(int nGID, const std::string& strID)
{
	//����GID��ID������
	SStatic* pObjName = FindChildByName2<SStatic>(L"obj_name");
	SStringW sstrName = L"";
	STabCtrl* pTabChatArea = FindChildByName2<STabCtrl>(L"tab_chat_area");
	if (nGID == 1) //new friend
	{
		pTabChatArea->SetCurSel(pTabChatArea->GetPageIndex(L"common_newfriend", TRUE));
		sstrName = L"�µ�����";
	}
	else if (nGID == 2) //gzh
	{
		pTabChatArea->SetCurSel(pTabChatArea->GetPageIndex(L"common_gzh", TRUE));
		sstrName = L"���ں�";
	}
	else if (nGID == 3) //dyh
	{
		pTabChatArea->SetCurSel(pTabChatArea->GetPageIndex(L"common_dyh", TRUE));
		sstrName = L"���ĺ�";
	}
	else if (nGID == 4) //group
	{
		pTabChatArea->SetCurSel(pTabChatArea->GetPageIndex(L"common_group", TRUE));

		std::string strName = CGlobalUnits::GetInstance()->m_mapGroups[strID].m_strGroupName;
		sstrName = S_CA2W(strName.c_str());
	}
	else if (nGID == 5) //personal
	{
		int nIndex = pTabChatArea->GetPageIndex(L"common_personal", TRUE);
		pTabChatArea->SetCurSel(nIndex);

		std::string strName = CGlobalUnits::GetInstance()->m_mapPersonals[strID].m_strName;
		sstrName = S_CA2W(strName.c_str());

		SWindow* pPage = pTabChatArea->GetPage(nIndex);
		//����avatar
		SImageWnd* pAvatar = pPage->FindChildByName2<SImageWnd>(L"item_avatar");

		//����name
		SStatic* pName = pPage->FindChildByName2<SStatic>(L"contact_userinfo_name");
		pName->SetWindowTextW(sstrName);

		//����΢�ź�
		SStatic* pWxNumber = pPage->FindChildByName2<SStatic>(L"contact_userinfo_wxnumber");
		SStringW sstrWxNumber;
		SStringW sstrNumber = L"0xFFFFFF";
		sstrWxNumber.Format(L"΢�źţ�%s", sstrNumber);
		pWxNumber->SetWindowTextW(sstrWxNumber);

		//���õ���
		SStatic* pArea = pPage->FindChildByName2<SStatic>(L"contact_userinfo_area");
		SStringW sstrArea = L"�������㽭 ����";
		pArea->SetWindowTextW(sstrArea);

		//���ñ�ע
		SStatic* pMark = pPage->FindChildByName2<SStatic>(L"contact_userinfo_mark");

		//���ø���ǩ��

		//������Դ
	}
	pObjName->SetWindowTextW(sstrName);
}

void CImDlg::ContactTVItemDBClick(int nGID, const std::string& strID)
{
	if (nGID == 1 || nGID == 2 || nGID == 3) return;

	SStatic* pObjName = FindChildByName2<SStatic>(L"obj_name");
	SStringW sstrName = L"";
	
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

		STabCtrl* pTabChatArea = FindChildByName2<STabCtrl>(L"tab_chat_area");
		SStringW sstrPage;
		sstrPage.Format(L"<page title='%s'><include src='layout:XML_PAGECOMMON_CHATGROUP'/></page>", sstrID);
		pTabChatArea->InsertItem(sstrPage);
		SWindow* pPage = pTabChatArea->GetPage(sstrID, TRUE);
		SASSERT(pPage);
		SImRichEdit* pRecvRichedit = pPage->FindChildByName2<SImRichEdit>(L"recv_richedit");
		SImRichEdit* pSendRichedit = pPage->FindChildByName2<SImRichEdit>(L"send_richedit");
		SASSERT(pRecvRichedit);
		SASSERT(pSendRichedit);

		pSendRichedit->SetFocus();
		DWORD dwEvtMask = pSendRichedit->SSendMessage(EM_GETEVENTMASK);
		pSendRichedit->SSendMessage(EM_SETEVENTMASK, 0, dwEvtMask | ENM_CHANGE);
		SUBSCRIBE(pRecvRichedit, EVT_RE_SCROLLBAR, CImDlg::OnRecvRichEditScrollEvent);
		SUBSCRIBE(pRecvRichedit, EVT_CTXMENU, CImDlg::OnRecvRichEditScrollEvent);
		SUBSCRIBE(pRecvRichedit, EVT_RE_OBJ, CImDlg::OnRecvRichEditObjEvent);

		SUBSCRIBE(pSendRichedit, EVT_RE_QUERY_ACCEPT, CImDlg::OnSendRichEditAcceptData);
		SUBSCRIBE(pSendRichedit, EVT_RE_NOTIFY, CImDlg::OnSendRichEditEditorChange);
		SUBSCRIBE(pSendRichedit, EVT_CTXMENU, CImDlg::OnSendRichEditMenu);
		SUBSCRIBE(pSendRichedit, EVT_RE_OBJ, CImDlg::OnRecvRichEditObjEvent);

		AddFetchMoreBlock(pRecvRichedit);

		int nPage = pTabChatArea->GetPageIndex(sstrID, TRUE);
		pTabChatArea->SetCurSel(nPage);
		m_pLvMessageAdapter->SetCurSel(sstrID);

		CGlobalUnits::GetInstance()->m_LvMessageCurSel.m_nType = 2;
		CGlobalUnits::GetInstance()->m_LvMessageCurSel.m_strID = strID;

		std::string strName = CGlobalUnits::GetInstance()->m_mapGroups[strID].m_strGroupName;
		sstrName = S_CA2W(strName.c_str());
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

		STabCtrl* pTabChatArea = FindChildByName2<STabCtrl>(L"tab_chat_area");
		SStringW sstrPage;
		sstrPage.Format(L"<page title='%s'><include src='layout:XML_PAGECOMMON_CHATPERSONAL'/></page>", sstrID);
		pTabChatArea->InsertItem(sstrPage);
		SWindow* pPage = pTabChatArea->GetPage(sstrID, TRUE);
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

		int nPage = pTabChatArea->GetPageIndex(sstrID, TRUE);
		pTabChatArea->SetCurSel(nPage);
		m_pLvMessageAdapter->SetCurSel(sstrID);

		CGlobalUnits::GetInstance()->m_LvMessageCurSel.m_nType = 1;
		CGlobalUnits::GetInstance()->m_LvMessageCurSel.m_strID = strID;

		std::string strName = CGlobalUnits::GetInstance()->m_mapPersonals[strID].m_strName;
		sstrName = S_CA2W(strName.c_str());
	}
	pObjName->SetWindowTextW(sstrName);
}

void CImDlg::ContactTVItemRClick(int nGID, const std::string& strID)
{
	if (nGID == 1 || nGID == 2 || nGID == 3) return;
	SStringW sstrID = S_CA2W(strID.c_str());

	SStringW sstrFont = L"face:΢���ź�,size:12";
	MenuWrapper menu(L"menu_contactlist", L"SMENU");
	if (4 ==  nGID)
	{
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
			ContactTVItemDBClick(nGID, strID);
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
			ContactTVItemDBClick(nGID, strID);
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

void CImDlg::OnEmotionItemClick(const std::string& strID)
{
	//TODO:
	m_pEmojiMenu->ExitPopupMenu(-1);
	
	auto& tmp = CGlobalUnits::GetInstance()->m_mapEmojisIndex;
	//��ȡAppPath
	TCHAR szFilePath[MAX_PATH + 1];
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0;
	SStringW sstrExePath = szFilePath;
	SStringW sstrEmojiPath = sstrExePath + L"emojis\\";
	std::string strFileName = CGlobalUnits::GetInstance()->m_mapEmojisIndex[strID];
	SStringW sstrFileName = S_CA2W(strFileName.c_str());
	sstrEmojiPath += sstrFileName;

	SStringW str;
	str.Format(L"<RichEditContent>"
		L"<para break=\"0\" disable-layout=\"1\">"
		L"<img type=\"smiley_img\" path=\"%s\" id=\"zzz\" max-size=\"\" minsize=\"\" scaring=\"1\" cursor=\"hand\" />"
		L"</para>"
		L"</RichEditContent>", sstrEmojiPath);

	//����currSel����page
	STabCtrl* pTabChatArea = FindChildByName2<STabCtrl>(L"tab_chat_area");
	int nPage = pTabChatArea->GetPageIndex(S_CA2W(CGlobalUnits::GetInstance()->m_LvMessageCurSel.m_strID.c_str()), TRUE);
	SWindow* pPage = pTabChatArea->GetPage(nPage);
	SImRichEdit* pSendRichedit = pPage->FindChildByName2<SImRichEdit>(L"send_richedit");
	SASSERT(pSendRichedit);
	pSendRichedit->InsertContent(str, RECONTENT_CARET);
}

void CImDlg::OnBnClickSend()
{
	STabCtrl* pTabChatArea = FindChildByName2<STabCtrl>(L"tab_chat_area");
	SWindow* pPage = pTabChatArea->GetPage(S_CA2W(CGlobalUnits::GetInstance()->m_LvMessageCurSel.m_strID.c_str()));
	SImRichEdit* pRecvRichedit = pPage->FindChildByName2<SImRichEdit>(L"recv_richedit");
	SImRichEdit* pSendRichedit = pPage->FindChildByName2<SImRichEdit>(L"send_richedit");

	CHARRANGE chr = { 0, -1 };
	SStringW strContent = pSendRichedit->GetSelectedContent(&chr);
	pugi::xml_document doc;
	if (!doc.load_buffer(strContent, strContent.GetLength() * sizeof(WCHAR)))
		return;

	//�����е���Ϣ���һ��json���ݣ�Ȼ�������ʱ�ٰ�˳��������ڽ���ʱ������ͼƬ���ļ���Ϣ������ӵ�recv_richedit���ı���emoji��������ӵ�recv_richedit
	//TODO:

	//����Ϣ����һ����ϢID
	std::string strUUID = CGlobalUnits::GetInstance()->GenerateUUID();

	SStringW sstrLasttalkContent = L"";
	std::vector<std::string> vecMessages;
	pugi::xml_node node = doc.child(L"RichEditContent").first_child();
	for (; node; node = node.next_sibling())
	{
		const wchar_t* pNodeName = node.name();
		if (wcscmp(RichEditText::GetClassName(), pNodeName) == 0)//�ı���Ϣ
		{
			//���ı�����Ҫ�����ַ���emoji�����
			SStringW sstrText = node.text().get();
			std::string strText = S_CW2A(sstrText);
			auto encode = CBase64::GetInstance()->base64_encode((unsigned char const*)strText.c_str(), strText.length());

			Json::Value msg;
			msg["type"] = "text";
			msg["content"] = encode;

			Json::FastWriter writer;
			std::string strJson = writer.write(msg);
			vecMessages.push_back(strJson);

			sstrLasttalkContent += sstrText;
		}
		else if (wcscmp(RichEditImageOle::GetClassName(), pNodeName) == 0) //ͼƬ��Ϣ
		{
			//��ͼƬ����Ҫ��emoji����ֿ�����
			SStringW sstrPath = node.attribute(L"path").as_string();
			SStringW sstrType = node.attribute(L"type").as_string();
			std::string strPath = S_CW2A(sstrPath);
			std::string strType = S_CW2A(sstrType);

			Json::Value msg;
			msg["type"] = "image";
			msg["img_type"] = strType;
			msg["img_path"] = strPath;

			Json::FastWriter writer;
			std::string strJson = writer.write(msg);
			vecMessages.push_back(strJson);

			sstrLasttalkContent += L"[ͼƬ]";
		}
		else if (wcscmp(RichEditMetaFileOle::GetClassName(), pNodeName) == 0)//�ļ���Ϣ
		{
			SStringW sstrPath = node.attribute(L"file").as_string();
			std::string strPath = S_CW2A(sstrPath);

			Json::Value msg;
			msg["type"] = "file";
			msg["file_path"] = strPath;

			Json::FastWriter writer;
			std::string strJson = writer.write(msg);
			vecMessages.push_back(strJson);

			sstrLasttalkContent += L"[�ļ�]";
		}
		else
		{
			//��������Ϣ���ɼ��ں���������@��Ϣ���������Զ�����Ϣ
		}
	}

	
	std::vector<SStringW> vecMessagePara;
	std::string strOldMessageType = "";
	SStringW sstrMessageFormat = L"";

	bool bAppend = false;

	SStringW sstrTempMessage;
	for (int i = 0; i < vecMessages.size(); i++)
	{
		std::string strJson = vecMessages[i];
		Json::Reader reader;
		Json::Value  root;
		if (reader.parse(strJson.c_str(), root))
		{
			std::string strType = root["type"].asString();
// 			if (bAppend)
// 			{
// 				//
// 			}
// 			else
// 			{
// 				if (strType == "text")
// 				{
// 					std::string strContent = root["content"].asString();
// 					auto decode = CBase64::GetInstance()->base64_decode(strContent);
// 					SStringW sstrContent = S_CA2W(decode.c_str());
// 					//�˴��Ȳ��Դ��ı������
// 					SStringW sstrFormat;
// 					sstrFormat.Format(L"<text font-size=\"10\" font-face=\"΢���ź�\" color=\"#000000\"><![CDATA[%s]]></text>", sstrContent);
// 
// 					sstrMessageFormat += sstrFormat;
// 					bAppend = true;
// 				}
// 				else if (strType == "image")
// 				{
// 					std::string strImageType = root["img_type"].asString();
// 					std::string strImagePath = root["img_path"].asString();
// 					SStringW sstrImageType = S_CA2W(strImageType.c_str());
// 					SStringW sstrImagePath = S_CA2W(strImagePath.c_str());
// 					//������ͼƬ������ͼƬ�ֿ�
// 					if (strImageType == "smiley_img")   //����
// 					{
// 						//����ͼƬ���ı�һ����
// 						bAppend = true;
// 					}
// 					else if (strImageType == "mormal_img") //����ͼƬ
// 					{
// 						//����ͼƬ�����һ���������µ���Ϣ
// 						//��Ҫ��֮ǰ����Ϣ��������
// 
// 						bAppend = false;
// 					}
// 
// 					SStringW sstrOleID = S_CA2W(strUUID.c_str()) + L"_imgole";
// 					SStringW sstrFormatImage;
// 					sstrFormatImage.Format(L"<img subid=\"%s\" id=\"%s\" type=\"%s\" encoding=\"\" show-magnifier=\"1\" path=\"%s\" image_original_url=\"\"/>",
// 						sstrOleID, sstrOleID, sstrImageType, sstrImagePath);
// 					vecMessagePara.push_back(sstrFormatImage);
// 				}
// 			}


			if (strType == "text")
			{
				std::string strContent = root["content"].asString();
				auto decode = CBase64::GetInstance()->base64_decode(strContent);
				SStringW sstrContent = S_CA2W(decode.c_str());
				//�˴���Ҫ�������Ĵ�����Ҫ�������ı�������@�����ӵȷָ��
				//�˴��Ȳ��Դ��ı������
				SStringW sstrFormatText;
				sstrFormatText.Format(L"<text font-size=\"10\" font-face=\"΢���ź�\" color=\"#000000\"><![CDATA[%s]]></text>", sstrContent);
				vecMessagePara.push_back(sstrFormatText);
			}
			else if (strType == "image")
			{
				std::string strImageType = root["img_type"].asString();
				std::string strImagePath = root["img_path"].asString();
				SStringW sstrImageType = S_CA2W(strImageType.c_str());
				SStringW sstrImagePath = S_CA2W(strImagePath.c_str());

				SStringW sstrOleID = S_CA2W(strUUID.c_str()) + L"_imgole";
				SStringW sstrFormatImage;
				sstrFormatImage.Format(L"<img subid=\"%s\" id=\"%s\" type=\"%s\" encoding=\"\" show-magnifier=\"1\" path=\"%s\" image_original_url=\"\"/>", 
					sstrOleID, sstrOleID, sstrImageType, sstrImagePath);
				vecMessagePara.push_back(sstrFormatImage);
			}
		}
	}

	SYSTEMTIME lpsystime;
	GetLocalTime(&lpsystime);
	time_t tt = time(NULL);
	tm* t = localtime(&tt);
	SStringT sstrTempTime;
	if (t->tm_hour > 12)
		sstrTempTime.Format(L"%02d��%02d��  ����%02d:%02d", t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min);
	else
		sstrTempTime.Format(L"%02d��%02d��  ����%02d:%02d", t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min);

	switch (CGlobalUnits::GetInstance()->m_LvMessageCurSel.m_nType)
	{
	case 0: //filehelper
	{
		CGlobalUnits::GetInstance()->m_strFileHelperLasttalkContent = S_CW2A(sstrLasttalkContent);
		CGlobalUnits::GetInstance()->m_ttFileHelperLasttalkTime = tt;
	}
	break;
	case 1://personal
	{
		CGlobalUnits::GetInstance()->m_mapPersonalLasttalkContent[CGlobalUnits::GetInstance()->m_LvMessageCurSel.m_strID] = S_CW2A(sstrLasttalkContent);
		CGlobalUnits::GetInstance()->m_mapPersonalLasttalkTime[CGlobalUnits::GetInstance()->m_LvMessageCurSel.m_strID] = tt;
	}
	break;
	case 2://group
	{
		CGlobalUnits::GetInstance()->m_mapGroupLasttalkContent[CGlobalUnits::GetInstance()->m_LvMessageCurSel.m_strID] = S_CW2A(sstrLasttalkContent);
		CGlobalUnits::GetInstance()->m_mapGroupLasttalkTime[CGlobalUnits::GetInstance()->m_LvMessageCurSel.m_strID] = tt;
	}
	break;
	default:
		break;
	}

	//����һ���յĶ���
	LPCWSTR pEmpty;
	pEmpty = L"<para id=\"msgbody\" margin=\"0,0,0,0\" break=\"1\" simulate-align=\"1\">"
		L""
		L"</para>";

	//���Է�����Ϣ����ʱ��
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

	SStringW sstrID = S_CA2W(CGlobalUnits::GetInstance()->m_LvMessageCurSel.m_strID.c_str());
	SStringW sstrFaceID;
	auto iterFace = CGlobalUnits::GetInstance()->m_mapFaceIndex.find(CGlobalUnits::GetInstance()->m_LvMessageCurSel.m_strID);
	if (iterFace != CGlobalUnits::GetInstance()->m_mapFaceIndex.end())
		sstrFaceID = iterFace->second.c_str();

	//����ͷ��
	SStringW sstrAvatar;
	sstrAvatar.Format(L"<bkele id=\"%s\" skin=\"%s\" right-pos=\"-50,]-8,@32,@32\" cursor=\"hand\" interactive=\"1\"/>", sstrID, sstrFaceID);

	//������Ϣ
	SStringW sstrMessage;
	for (int i = 0; i < vecMessagePara.size(); i++)
	{
		sstrMessage += vecMessagePara[i];
	}

	SStringW sstrTextPara;
	sstrTextPara.Format(
		L"<para id=\"msgbody\" margin=\"65,0,45,0\" break=\"1\" simulate-align=\"1\">"
		L"%s"
		L"</para>", sstrMessage);


	//��������
	SStringW sstrBubble = L"<bkele data=\"bubble\" right-skin=\"skin_right_bubble\" right-pos=\"{-10,{-9,-55,[10\" />";

	SStringW sstrContent;
	sstrContent.Format(
		L"<RichEditContent msgtype=\"text\" talk_type=\"personal\" type=\"ContentRight\" align=\"right\" auto-layout=\"1\">"
		L"<para break=\"1\" align=\"left\" />"
		L"%s"
		L"%s"
		L"%s"
		L"%s" //���������ն���
		L"</RichEditContent>", sstrAvatar, sstrTextPara, sstrBubble, pEmpty);
	pRecvRichedit->InsertContent(sstrContent, RECONTENT_LAST);
	pRecvRichedit->ScrollToBottom();

	pSendRichedit->Clear();

	//��Ϣ����֮����Ҫ��������Ự�б��е���Ϣ���ݡ���Ϣʱ�����Ϣ
	m_pLvMessageAdapter->Update();
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
	case DBLCLICK_RICH_METAFILE:
	{
		//���Ϳ��ļ�˫���¼�
		//TODO:

		int kk = 0;
	}
	break;
	case DBLCLICK_IMAGEOLE:
	{
		RichEditImageOle* pImageOle = sobj_cast<RichEditImageOle>(pev->RichObj);
		ShellExecute(NULL, _T("open"), pImageOle->GetImagePath(), NULL, NULL, SW_SHOW);
	}
	break;
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