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

	//处理最近回话列表以及设置适配器
	SListView* pLvMessage = FindChildByName2<SListView>(L"lv_message");
	SASSERT(pLvMessage);
	pLvMessage->EnableScrollBar(SSB_HORZ, FALSE);
	m_pLvMessageAdapter = new CLvMessageAdapter(pLvMessage, this);
	pLvMessage->SetAdapter(m_pLvMessageAdapter);
	m_pLvMessageAdapter->Release();

	//此处需要根据最近会话的列表去填充的，现在这里暂时默认为没有最近会话
	{
		//添加一个文件助手
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


	//处理搜索列表以及设置适配器

	//处理通讯录列表以及设置适配器
	STreeView* pTreeView = FindChildByName2<STreeView>("tv_contact");
	pTreeView->EnableScrollBar(SSB_HORZ, FALSE);
	m_pTvContactAdapter = new CTvContactAdapter(this);
	pTreeView->SetAdapter(m_pTvContactAdapter);
	m_pTvContactAdapter->Release();
#pragma region 测试往联系人列表中添加数据
	{//添加新的朋友
		TreeItemData team;
		team.bGroup = true;
		team.gid = 1;
		team.strName = L"新的朋友";
		m_pTvContactAdapter->AddItem(team);

		TreeItemData item;
		item.gid = team.gid;
		item.bGroup = FALSE;
		item.strID = "new_friends";
		item.strName = _T("新的朋友");
		item.strImg = _T("skin_default_personal32");
		m_pTvContactAdapter->AddItem(item);
	}

	{//添加公众号
		TreeItemData team;
		team.bGroup = true;
		team.gid = 2;
		team.strName = L"公众号";
		m_pTvContactAdapter->AddItem(team);

		TreeItemData item;
		item.gid = team.gid;
		item.bGroup = FALSE;
		item.strID = "gzh";
		item.strName = _T("公众号");
		item.strImg = _T("skin_default_gzh32");
		m_pTvContactAdapter->AddItem(item);
	}

	{//添加订阅号
		TreeItemData team;
		team.bGroup = true;
		team.gid = 3;
		team.strName = L"订阅号";
		m_pTvContactAdapter->AddItem(team);

		TreeItemData item;
		item.gid = team.gid;
		item.bGroup = FALSE;
		item.strID = "dyh";
		item.strName = _T("订阅号");
		item.strImg = _T("skin_default_dyh32");
		m_pTvContactAdapter->AddItem(item);
	}

	{//添加群聊
		TreeItemData team;
		team.bGroup = true;
		team.gid = 4;
		team.strName = L"群聊";
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

	{//添加个人好友
		TreeItemData data;
		data.bGroup = true;
		data.gid = 5;
		data.strName = L"好友";
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

			//处理头像
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
					//使用默认头像
					sstrFacePath = L"default_res\\default_portrait.png";
				}
				else
				{
					//使用自定义头像
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


	//处理新朋友列表以及设置适配器
	SListView* pLvNewFriend = FindChildByName2<SListView>(L"lv_newfrined");
	SASSERT(pLvNewFriend);
	m_pLvNewFriendAdapter = new CLvNewFriendAdapter(pLvNewFriend, this);
	pLvNewFriend->SetAdapter(m_pLvNewFriendAdapter);
	m_pLvNewFriendAdapter->Release();
#pragma region 测试添加新朋友数据
	for (int i = 0; i < 20; i++)
	{
		SStringW sstrID;
		sstrID.Format(L"%d", i);
		SStringW sstrAvatar = L"";
		SStringW sstrName;
		sstrName.Format(L"新朋友%d", i);
		SStringW sstrMessage;
		sstrMessage.Format(L"你好 我是新朋友%d 这是一条验证消息", i);
		int nState = 0;
		if (i == 0)
			nState = 2;
		else
			nState = i % 2;

		m_pLvNewFriendAdapter->AddItem(sstrID, sstrAvatar, sstrName, sstrMessage, nState);
	}
#pragma endregion

	//处理公众号列表以及设置适配器
	STileView* pTileGZH = FindChildByName2<STileView>(L"tileview_gzh");
	SASSERT(pTileGZH);
	m_pTileViewGZHAdapter = new CGZHTileViewAdapter(this);
	pTileGZH->SetAdapter(m_pTileViewGZHAdapter);
	m_pTileViewGZHAdapter->Release();
#pragma region 测试添加公众号数据
	for (int i = 0; i < 10; i++)
	{
		SStringW sstrID;
		sstrID.Format(L"%d", i);
		SStringW sstrName;
		sstrName.Format(L"公众号%d", i);
		std::string strAvatar = "";
		std::string strID = S_CW2A(sstrID);
		std::string strName = S_CW2A(sstrName);
		m_pTileViewGZHAdapter->AddItem(strID, strName, strAvatar);
	}
#pragma endregion

	//处理订阅号列表以及设置适配器
	STileView* pTileDYH = FindChildByName2<STileView>(L"tileview_dyh");
	SASSERT(pTileDYH);
	m_pTileViewDYHAdapter = new CDYHTileViewAdapter(this);
	pTileDYH->SetAdapter(m_pTileViewDYHAdapter);
	m_pTileViewDYHAdapter->Release();
#pragma region 测试添加订阅号数据
	for (int i = 0; i < 20; i++)
	{
		SStringW sstrID;
		sstrID.Format(L"%d", i);
		SStringW sstrName;
		sstrName.Format(L"订阅号%d", i);
		std::string strAvatar = "";
		std::string strID = S_CW2A(sstrID);
		std::string strName = S_CW2A(sstrName);
		m_pTileViewDYHAdapter->AddItem(strID, strName, strAvatar);
	}
#pragma endregion

	//处理群成员列表以及设置适配器
	STileView* pTileGrpMbr = FindChildByName2<STileView>(L"tileview_grpmbr");
	SASSERT(pTileGrpMbr);
	m_pTileViewGrpMbrAdapter = new CGrpMbrTileViewAdapter(this);
	pTileGrpMbr->SetAdapter(m_pTileViewGrpMbrAdapter);
	m_pTileViewGrpMbrAdapter->Release();
#pragma region 测试添加群成员数据
	for (int i = 0; i < 40; i++)
	{
		SStringW sstrID;
		sstrID.Format(L"%d", i);
		SStringW sstrName;
		sstrName.Format(L"群成员%d", i);
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
	CFileDialogEx openDlg(TRUE, _T("图片"), 0, 6,
		_T("图片文件\0*.gif;*.bmp;*.jpg;*.png\0\0"));
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

		//根据currSel查找page
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
	CFileDialogEx openDlg(TRUE, _T("文件"), 0, 6,
		_T("文件\0*.*\0\0"));
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

		//根据currSel查找page
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
		sstrName = L"文件传输助手";
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
		sstrName = L"订阅号";
	}
	break;
	case 4: //新闻
	{
		sstrName = L"新闻";
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
	//显示右键菜单
	//群、个人、公众号、订阅号、新闻的菜单各不相同，需要分别处理
	CLvMessageAdapter::ItemData* pData = m_pLvMessageAdapter->GetItemData(nIndex);

	SStringW sstrFont = L"face:微软雅黑,size:12";
	MenuWrapper menu(L"menu_messagelist", L"SMENU");
	switch (pData->m_nType)
	{
	case 0:
	{
		//文件传输助手
		if (CGlobalUnits::GetInstance()->m_bFileHelperTopmost)
			menu.AddMenu(_T("取消置顶"), 1, TRUE, FALSE, sstrFont);
		else
			menu.AddMenu(_T("置顶"), 1, TRUE, FALSE, sstrFont);
		
		if (CGlobalUnits::GetInstance()->m_bFileHelperRead)
			menu.AddMenu(_T("标为未读"), 2, TRUE, FALSE, sstrFont);
		else
			menu.AddMenu(_T("标为已读"), 2, TRUE, FALSE, sstrFont);
		
		if (CGlobalUnits::GetInstance()->m_bFileHelperDisturb)
			menu.AddMenu(_T("消息免打扰"), 3, TRUE, FALSE, sstrFont);
		else
			menu.AddMenu(_T("开启消息提醒"), 3, TRUE, FALSE, sstrFont);

		menu.AddMenu(_T("在独立窗口中打开"), 4, TRUE, FALSE, sstrFont);
		menu.AddSeperator();
		menu.AddMenu(_T("不显示聊天"), 5, TRUE, FALSE, sstrFont);
		menu.AddMenu(_T("删除聊天"), 6, TRUE, FALSE, sstrFont);

		int ret = 0;
		POINT pt;
		::GetCursorPos(&pt);

		ret = menu.ShowMenu(TPM_RETURNCMD, pt.x, pt.y, m_hWnd);
		switch (ret)
		{
		case 1:
		{
			CGlobalUnits::GetInstance()->m_bFileHelperTopmost = !CGlobalUnits::GetInstance()->m_bFileHelperTopmost;
			//需要将lvMessage中的消息项移动到置顶聊天之后
			//TODO:
		}
		break;
		case 2:
		{
			CGlobalUnits::GetInstance()->m_bFileHelperRead = !CGlobalUnits::GetInstance()->m_bFileHelperRead;
			//需要更新lvMessage中的消息项使其在头像右上角的红点已标记该消息是已读或者未读
			//TODO:
		}
		break;
		case 3:
		{
			CGlobalUnits::GetInstance()->m_bFileHelperDisturb = !CGlobalUnits::GetInstance()->m_bFileHelperDisturb;
			//需要更新lvMessage中的消息项中关于消息提醒状态
			//TODO:
		}
		break;
		case 4:
		{
			//重新创建个窗口用来展示聊天信息，从整体的展示中删除有关于这个聊天相关的数据。
		}
		break;
		case 5:
		{
			//不显示聊天
		}
		break;
		case 6:
		{
			//删除聊天
		}
		break;
		default:
			break;
		}
	}
	break;
	case 1:
	{
		//个人聊天
		auto iterTopmost = CGlobalUnits::GetInstance()->m_mapPersonalTopmost.find(pData->m_strID);
		if (iterTopmost == CGlobalUnits::GetInstance()->m_mapPersonalTopmost.end())
		{
			menu.AddMenu(_T("置顶"), 1, TRUE, FALSE, sstrFont);
			CGlobalUnits::GetInstance()->m_mapPersonalTopmost[pData->m_strID] = false;
		}
		else
		{
			if (iterTopmost->second)
				menu.AddMenu(_T("取消置顶"), 1, TRUE, FALSE, sstrFont);
			else
				menu.AddMenu(_T("置顶"), 1, TRUE, FALSE, sstrFont);
		}

		auto iterRead = CGlobalUnits::GetInstance()->m_mapPersonalRead.find(pData->m_strID);
		if (iterRead == CGlobalUnits::GetInstance()->m_mapPersonalRead.end())
		{
			menu.AddMenu(_T("标为已读"), 2, TRUE, FALSE, sstrFont);
			CGlobalUnits::GetInstance()->m_mapPersonalRead[pData->m_strID] = false;
		}
		else
		{
			if (iterRead->second)
				menu.AddMenu(_T("标为未读"), 2, TRUE, FALSE, sstrFont);
			else
				menu.AddMenu(_T("标为已读"), 2, TRUE, FALSE, sstrFont);
		}

		auto iterDisturb = CGlobalUnits::GetInstance()->m_mapPersonalDisturb.find(pData->m_strID);
		if (iterDisturb == CGlobalUnits::GetInstance()->m_mapPersonalDisturb.end())
		{
			menu.AddMenu(_T("开启消息提醒"), 3, TRUE, FALSE, sstrFont);
			CGlobalUnits::GetInstance()->m_mapPersonalDisturb[pData->m_strID] = false;
		}
		else
		{
			if (iterDisturb->second)
				menu.AddMenu(_T("消息免打扰"), 3, TRUE, FALSE, sstrFont);
			else
				menu.AddMenu(_T("开启消息提醒"), 3, TRUE, FALSE, sstrFont);
		}

		menu.AddMenu(_T("在独立窗口中打开"), 4, TRUE, FALSE, sstrFont);
		menu.AddSeperator();
		menu.AddMenu(_T("不显示聊天"), 5, TRUE, FALSE, sstrFont);
		menu.AddMenu(_T("删除聊天"), 6, TRUE, FALSE, sstrFont);

		int ret = 0;
		POINT pt;
		::GetCursorPos(&pt);

		ret = menu.ShowMenu(TPM_RETURNCMD, pt.x, pt.y, m_hWnd);
		switch (ret)
		{
		case 1:
		{
			CGlobalUnits::GetInstance()->m_mapPersonalTopmost[pData->m_strID] = !CGlobalUnits::GetInstance()->m_mapPersonalTopmost[pData->m_strID];
			//需要将lvMessage中的消息项移动到置顶聊天之后
			//TODO:
		}
		break;
		case 2:
		{
			CGlobalUnits::GetInstance()->m_mapPersonalRead[pData->m_strID] = !CGlobalUnits::GetInstance()->m_mapPersonalRead[pData->m_strID];
			//需要更新lvMessage中的消息项使其在头像右上角的红点已标记该消息是已读或者未读
			//TODO:
		}
		break;
		case 3:
		{
			CGlobalUnits::GetInstance()->m_mapPersonalDisturb[pData->m_strID] = !CGlobalUnits::GetInstance()->m_mapPersonalDisturb[pData->m_strID];
			//需要更新lvMessage中的消息项中关于消息提醒状态
			//TODO:
		}
		break;
		case 4:
		{
			//重新创建个窗口用来展示聊天信息，从整体的展示中删除有关于这个聊天相关的数据。
		}
		break;
		case 5:
		{
			//不显示聊天
		}
		break;
		case 6:
		{
			//删除聊天
		}
		break;
		default:
			break;
		}
	}
	break;
	case 2:
	{
		//多人聊天
		auto iterTopmost = CGlobalUnits::GetInstance()->m_mapGroupTopmost.find(pData->m_strID);
		if (iterTopmost == CGlobalUnits::GetInstance()->m_mapGroupTopmost.end())
		{
			menu.AddMenu(_T("置顶"), 1, TRUE, FALSE, sstrFont);
			CGlobalUnits::GetInstance()->m_mapGroupTopmost[pData->m_strID] = false;
		}
		else
		{
			if (iterTopmost->second)
				menu.AddMenu(_T("取消置顶"), 1, TRUE, FALSE, sstrFont);
			else
				menu.AddMenu(_T("置顶"), 1, TRUE, FALSE, sstrFont);
		}

		auto iterRead = CGlobalUnits::GetInstance()->m_mapGroupRead.find(pData->m_strID);
		if (iterRead == CGlobalUnits::GetInstance()->m_mapGroupRead.end())
		{
			menu.AddMenu(_T("标为已读"), 2, TRUE, FALSE, sstrFont);
			CGlobalUnits::GetInstance()->m_mapGroupRead[pData->m_strID] = false;
		}
		else
		{
			if (iterRead->second)
				menu.AddMenu(_T("标为未读"), 2, TRUE, FALSE, sstrFont);
			else
				menu.AddMenu(_T("标为已读"), 2, TRUE, FALSE, sstrFont);
		}

		auto iterDisturb = CGlobalUnits::GetInstance()->m_mapGroupDisturb.find(pData->m_strID);
		if (iterDisturb == CGlobalUnits::GetInstance()->m_mapGroupDisturb.end())
		{
			menu.AddMenu(_T("开启消息提醒"), 3, TRUE, FALSE, sstrFont);
			CGlobalUnits::GetInstance()->m_mapGroupDisturb[pData->m_strID] = false;
		}
		else
		{
			if (iterDisturb->second)
				menu.AddMenu(_T("消息免打扰"), 3, TRUE, FALSE, sstrFont);
			else
				menu.AddMenu(_T("开启消息提醒"), 3, TRUE, FALSE, sstrFont);
		}

		menu.AddMenu(_T("修改群聊名称"), 4, TRUE, FALSE, sstrFont);
		menu.AddMenu(_T("设置备注"), 5, TRUE, FALSE, sstrFont);
		menu.AddMenu(_T("在独立窗口中打开"), 6, TRUE, FALSE, sstrFont);
		menu.AddMenu(_T("保存到通讯录"), 7, TRUE, FALSE, sstrFont);   //需要判断是否已经保存到通讯录了

		menu.AddSeperator();
		menu.AddMenu(_T("不显示聊天"), 8, TRUE, FALSE, sstrFont);
		menu.AddMenu(_T("删除聊天"), 9, TRUE, FALSE, sstrFont);

		int ret = 0;
		POINT pt;
		::GetCursorPos(&pt);

		ret = menu.ShowMenu(TPM_RETURNCMD, pt.x, pt.y, m_hWnd);
		switch (ret)
		{
		case 1:
		{
			CGlobalUnits::GetInstance()->m_mapGroupTopmost[pData->m_strID] = !CGlobalUnits::GetInstance()->m_mapGroupTopmost[pData->m_strID];
			//需要将lvMessage中的消息项移动到置顶聊天之后
			//TODO:
		}
		break;
		case 2:
		{
			CGlobalUnits::GetInstance()->m_mapGroupRead[pData->m_strID] = !CGlobalUnits::GetInstance()->m_mapGroupRead[pData->m_strID];
			//需要更新lvMessage中的消息项使其在头像右上角的红点已标记该消息是已读或者未读
			//TODO:
		}
		break;
		case 3:
		{
			CGlobalUnits::GetInstance()->m_mapGroupDisturb[pData->m_strID] = !CGlobalUnits::GetInstance()->m_mapGroupDisturb[pData->m_strID];
			//需要更新lvMessage中的消息项中关于消息提醒状态
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
		//订阅号
	}
	break;
	case 4:
	{
		//新闻
	}
	break;
	case 5:
	{
		//公众号
	}
	break;
	default:
		break;
	}

	m_pLvMessageAdapter->Update();
}

void CImDlg::ContactTVItemClick(int nGID, const std::string& strID)
{
	//根据GID、ID做处理
	SStatic* pObjName = FindChildByName2<SStatic>(L"obj_name");
	SStringW sstrName = L"";
	STabCtrl* pTabChatArea = FindChildByName2<STabCtrl>(L"tab_chat_area");
	if (nGID == 1) //new friend
	{
		pTabChatArea->SetCurSel(pTabChatArea->GetPageIndex(L"common_newfriend", TRUE));
		sstrName = L"新的朋友";
	}
	else if (nGID == 2) //gzh
	{
		pTabChatArea->SetCurSel(pTabChatArea->GetPageIndex(L"common_gzh", TRUE));
		sstrName = L"公众号";
	}
	else if (nGID == 3) //dyh
	{
		pTabChatArea->SetCurSel(pTabChatArea->GetPageIndex(L"common_dyh", TRUE));
		sstrName = L"订阅号";
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
		//设置avatar
		SImageWnd* pAvatar = pPage->FindChildByName2<SImageWnd>(L"item_avatar");

		//设置name
		SStatic* pName = pPage->FindChildByName2<SStatic>(L"contact_userinfo_name");
		pName->SetWindowTextW(sstrName);

		//设置微信号
		SStatic* pWxNumber = pPage->FindChildByName2<SStatic>(L"contact_userinfo_wxnumber");
		SStringW sstrWxNumber;
		SStringW sstrNumber = L"0xFFFFFF";
		sstrWxNumber.Format(L"微信号：%s", sstrNumber);
		pWxNumber->SetWindowTextW(sstrWxNumber);

		//设置地区
		SStatic* pArea = pPage->FindChildByName2<SStatic>(L"contact_userinfo_area");
		SStringW sstrArea = L"地区：浙江 杭州";
		pArea->SetWindowTextW(sstrArea);

		//设置备注
		SStatic* pMark = pPage->FindChildByName2<SStatic>(L"contact_userinfo_mark");

		//设置个性签名

		//设置来源
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

	SStringW sstrFont = L"face:微软雅黑,size:12";
	MenuWrapper menu(L"menu_contactlist", L"SMENU");
	if (4 ==  nGID)
	{
		menu.AddMenu(_T("发送消息"), 1, TRUE, FALSE, sstrFont);
		menu.AddMenu(_T("设置备注"), 2, TRUE, FALSE, sstrFont);
		menu.AddMenu(_T("从通讯录中删除"), 3, TRUE, FALSE, sstrFont);
		menu.AddSeperator();
		menu.AddMenu(_T("退出群聊"), 4, TRUE, FALSE, sstrFont);

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
		menu.AddMenu(_T("发送消息"), 1, TRUE, FALSE, sstrFont);
		menu.AddMenu(_T("设为星标朋友"), 2, TRUE, FALSE, sstrFont);
		menu.AddMenu(_T("发送名片"), 3, TRUE, FALSE, sstrFont);
		menu.AddSeperator();
		menu.AddMenu(_T("删除好友"), 4, TRUE, FALSE, sstrFont);

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
	//获取AppPath
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

	//根据currSel查找page
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

	//将所有的信息组成一个json数据，然后在添加时再按顺序解析，在解析时将正常图片、文件消息单独添加到recv_richedit、文本跟emoji表情混合添加到recv_richedit
	//TODO:

	//给消息生成一个消息ID
	std::string strUUID = CGlobalUnits::GetInstance()->GenerateUUID();

	SStringW sstrLasttalkContent = L"";
	std::vector<std::string> vecMessages;
	pugi::xml_node node = doc.child(L"RichEditContent").first_child();
	for (; node; node = node.next_sibling())
	{
		const wchar_t* pNodeName = node.name();
		if (wcscmp(RichEditText::GetClassName(), pNodeName) == 0)//文本消息
		{
			//在文本中需要处理字符串emoji的情况
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
		else if (wcscmp(RichEditImageOle::GetClassName(), pNodeName) == 0) //图片消息
		{
			//在图片中需要将emoji表情分开处理
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

			sstrLasttalkContent += L"[图片]";
		}
		else if (wcscmp(RichEditMetaFileOle::GetClassName(), pNodeName) == 0)//文件消息
		{
			SStringW sstrPath = node.attribute(L"file").as_string();
			std::string strPath = S_CW2A(sstrPath);

			Json::Value msg;
			msg["type"] = "file";
			msg["file_path"] = strPath;

			Json::FastWriter writer;
			std::string strJson = writer.write(msg);
			vecMessages.push_back(strJson);

			sstrLasttalkContent += L"[文件]";
		}
		else
		{
			//其他的消息都可加在后续，比如@消息或者其他自定义消息
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
// 					//此处先测试纯文本的情况
// 					SStringW sstrFormat;
// 					sstrFormat.Format(L"<text font-size=\"10\" font-face=\"微软雅黑\" color=\"#000000\"><![CDATA[%s]]></text>", sstrContent);
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
// 					//将正常图片跟表情图片分开
// 					if (strImageType == "smiley_img")   //表情
// 					{
// 						//表情图片跟文本一起处理
// 						bAppend = true;
// 					}
// 					else if (strImageType == "mormal_img") //正常图片
// 					{
// 						//正常图片处理成一个单独的新的消息
// 						//需要将之前的消息独立出来
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
				//此处需要做大量的处理，需要将正常文本跟比如@、链接等分割开来
				//此处先测试纯文本的情况
				SStringW sstrFormatText;
				sstrFormatText.Format(L"<text font-size=\"10\" font-face=\"微软雅黑\" color=\"#000000\"><![CDATA[%s]]></text>", sstrContent);
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
		sstrTempTime.Format(L"%02d月%02d日  下午%02d:%02d", t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min);
	else
		sstrTempTime.Format(L"%02d月%02d日  上午%02d:%02d", t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min);

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

	//定义一个空的段落
	LPCWSTR pEmpty;
	pEmpty = L"<para id=\"msgbody\" margin=\"0,0,0,0\" break=\"1\" simulate-align=\"1\">"
		L""
		L"</para>";

	//测试放置消息发送时间
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

	//处理头像
	SStringW sstrAvatar;
	sstrAvatar.Format(L"<bkele id=\"%s\" skin=\"%s\" right-pos=\"-50,]-8,@32,@32\" cursor=\"hand\" interactive=\"1\"/>", sstrID, sstrFaceID);

	//处理消息
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


	//处理气泡
	SStringW sstrBubble = L"<bkele data=\"bubble\" right-skin=\"skin_right_bubble\" right-pos=\"{-10,{-9,-55,[10\" />";

	SStringW sstrContent;
	sstrContent.Format(
		L"<RichEditContent msgtype=\"text\" talk_type=\"personal\" type=\"ContentRight\" align=\"right\" auto-layout=\"1\">"
		L"<para break=\"1\" align=\"left\" />"
		L"%s"
		L"%s"
		L"%s"
		L"%s" //放置两个空段落
		L"</RichEditContent>", sstrAvatar, sstrTextPara, sstrBubble, pEmpty);
	pRecvRichedit->InsertContent(sstrContent, RECONTENT_LAST);
	pRecvRichedit->ScrollToBottom();

	pSendRichedit->Clear();

	//消息发送之后需要更新最近会话列表中的消息内容、消息时间等信息
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
		//此处主要做在输入框输入@时展示群成员列表
	}

	return true;
}

bool CImDlg::OnSendRichEditMenu(EventArgs* pEvt)
{
	//处理发送框的右键菜单
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
	//处理发送框中ole元素的事件，比如如果图片ole双击了则使用图片预览展示当前图片
	switch (pev->SubEventId)
	{
	case DBLCLICK_RICH_METAFILE:
	{
		//发送框文件双击事件
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
			//展示下方是否还有新消息
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
			
			//请求加载历史消息，本地数据库读取或者向服务器请求
			//TODO:
		}
	}
	return true;
}

bool CImDlg::OnRecvRichEditMenu(EventArgs* pEvt)
{
	//接收框右键菜单
	EventCtxMenu* pev = static_cast<EventCtxMenu*>(pEvt);

	return true;
}

bool CImDlg::OnRecvRichEditObjEvent(EventArgs* pEvt)
{
	EventRichEditObj* pev = (EventRichEditObj*)pEvt;
	//处理接收框中ole元素的事件，比如如果图片ole双击了则使用图片预览展示当前图片
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