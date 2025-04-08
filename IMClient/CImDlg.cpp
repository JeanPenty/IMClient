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
		SUBSCRIBE(pSendRichedit, EVT_RE_OBJ, CImDlg::OnRecvRichEditObjEvent);


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

	{//添加个人好友
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

	{//添加群聊
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
	}
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
	case 0:
	{
		CGlobalUnits::GetInstance()->m_strFileHelperLasttalkContent = S_CW2A(sstrTmp);
		CGlobalUnits::GetInstance()->m_ttFileHelperLasttalkTime = tt;
	}
	break;
	case 1:
	{
		//
	}
	break;
	default:
		break;
	}

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

	//测试放置消息
	SStringW sstrTemp = sstrTmp;
	SStringW sstrText, sstrRevoke;
	sstrText.Format(L"<text font-size=\"10\" font-face=\"微软雅黑\" color=\"#000000\"><![CDATA[%s]]></text>", sstrTemp);
	sstrRevoke.Format(L"<text font-size=\"10\" font-face=\"微软雅黑\" color=\"#333333\"><![CDATA[%s]]></text>", sstrTemp);

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