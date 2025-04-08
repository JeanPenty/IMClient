#pragma once

#include <helper/SAdapterBase.h>
#include <vector>

#include "CGlobalUnits.h"

class CLvMessageAdapter : public SAdapterBase
{
public:
	struct ItemData
	{
		int m_nType;		//0文件传输助手，1个人聊天，2多人聊天，3订阅号，4新闻，5公众号
		std::string m_strID;
	};

	struct IListen
	{
		virtual void OnMessageItemClick(int& nIndex) = 0;
		virtual void OnMessageItemRClick(int& nIndex) = 0;
	};

protected:
	bool OnEventLvSelChangeing(EventLVSelChanging* pEvt)
	{
		if (NULL == pEvt)
			return true;

		pEvt->bubbleUp = false;
		if (-1 == pEvt->iNewSel)				// 不能设置 -1 
			pEvt->bCancel = TRUE;

		return TRUE;
	}

	bool OnEventItemPanelClick(EventArgs* e)
	{
		EventItemPanelClick* pEvt = sobj_cast<EventItemPanelClick>(e);
		if (!pEvt) return false;

		SItemPanel* pItem = sobj_cast<SItemPanel>(pEvt->sender);
		if (!pItem) return false;

		int nIndex = static_cast<int>(pItem->GetItemIndex());
		ItemData* pItemData = m_vecItemDatas[nIndex];

		m_pListen->OnMessageItemClick(nIndex);
		return TRUE;
	}

	bool OnEventItemPanelRClick(EventArgs* e)
	{
		EventItemPanelRclick* pEvt = sobj_cast<EventItemPanelRclick>(e);
		if (!pEvt) return false;

		SItemPanel* pItem = sobj_cast<SItemPanel>(pEvt->sender);
		if (!pItem) return false;

		int nIndex = static_cast<int>(pItem->GetItemIndex());
		ItemData* pItemData = m_vecItemDatas[nIndex];

		m_pListen->OnMessageItemRClick(nIndex);
		return TRUE;
	}

public:
	CLvMessageAdapter(SListView* pOwner, IListen* pListen)
	{
		m_pOwner = pOwner;
		SASSERT(m_pOwner);
		m_pOwner->GetEventSet()->subscribeEvent(&CLvMessageAdapter::OnEventLvSelChangeing, this);

		m_pListen = pListen;
	}

	virtual int WINAPI getCount()
	{
		return static_cast<int>(m_vecItemDatas.size());
	}

	virtual void WINAPI getView(int position, SWindow* pItem, pugi::xml_node xmlTemplate)
	{
		if (0 == pItem->GetChildrenCount())
		{
			pItem->InitFromXml(xmlTemplate);
			pItem->GetEventSet()->
				subscribeEvent(EventItemPanelClick::EventID,
					Subscriber(&CLvMessageAdapter::OnEventItemPanelClick, this));
			pItem->GetEventSet()->
				subscribeEvent(EventItemPanelRclick::EventID,
					Subscriber(&CLvMessageAdapter::OnEventItemPanelRClick, this));
		}

		size_t sPos = static_cast<size_t>(position);
		if (sPos >= m_vecItemDatas.size()) return;

		ItemData* pItemData = m_vecItemDatas[sPos];
		if (NULL == pItemData) return;

		//avatar
		SImageWnd* pAvatar = pItem->FindChildByName2<SImageWnd>(L"item_avatar");
		SASSERT(pAvatar);
		SStringW sstrFace;
		switch (pItemData->m_nType)
		{
		case 0://file helper
		{
			sstrFace = L"skin_default_filehelper32";
		}
		break;
		case 1://personal
		{
			sstrFace = L"skin_default_personal32";
		}
		break;
		case 2://group
		{
			sstrFace = L"skin_default_group32";
		}
		break;
		default:
			break;
		}
		pAvatar->SetAttribute(L"skin", sstrFace, TRUE);

		//name
		SStatic* pName = pItem->FindChildByName2<SStatic>(L"item_name");
		SStatic* pContent = pItem->FindChildByName2<SStatic>(L"item_content");
		SStatic* pTime = pItem->FindChildByName2<SStatic>(L"item_time");
		SImageWnd* pReminder = pItem->FindChildByName2<SImageWnd>(L"item_reminder");
		SImageWnd* pRead = pItem->FindChildByName2<SImageWnd>(L"item_readdot");
		SASSERT(pName);
		SASSERT(pContent);
		SASSERT(pTime);
		SASSERT(pReminder);
		SASSERT(pRead);

		switch (pItemData->m_nType)
		{
		case 0:
		{
			pName->SetWindowText(L"文件传输助手");

			//设置消息内容
			pContent->SetWindowText(S_CA2W(CGlobalUnits::GetInstance()->m_strFileHelperLasttalkContent.c_str()));

			//设置时间
			SYSTEMTIME lpsystime;
			GetLocalTime(&lpsystime);
			time_t tt = time(NULL);
			tm* tCurr = localtime(&tt);
			tm* t = localtime(&CGlobalUnits::GetInstance()->m_ttFileHelperLasttalkTime);

			SStringW sstrTime;
			if (tCurr->tm_year == t->tm_year && tCurr->tm_mon == t->tm_mon && tCurr->tm_mday == t->tm_mday)
			{
				sstrTime.Format(L"%02d:%02d", t->tm_hour, t->tm_min);
			}
			else if (tCurr->tm_year == t->tm_year && tCurr->tm_mon == t->tm_mon && tCurr->tm_mday - 1 == t->tm_mday)
				sstrTime = L"昨天";
			else
				sstrTime.Format(L"%02d月%02d日", t->tm_mon + 1, t->tm_mday);
			pTime->SetWindowText(sstrTime);

			//设置消息提醒
			if (!CGlobalUnits::GetInstance()->m_bFileHelperDisturb)
				pReminder->SetAttribute(L"show", L"1");
			else
				pReminder->SetAttribute(L"show", L"0");

			//设置消息未读相关
			if (!CGlobalUnits::GetInstance()->m_bFileHelperRead)
				pRead->SetAttribute(L"show", L"1");
			else
				pRead->SetAttribute(L"show", L"0");
		}
		break;
		case 1:
		{
			std::string strName = "";
			PersonalsMap::iterator iter = CGlobalUnits::GetInstance()->m_mapPersonals.find(pItemData->m_strID);
			if (iter != CGlobalUnits::GetInstance()->m_mapPersonals.end())
				strName = iter->second.m_strName;
			else
				strName = pItemData->m_strID;
			pName->SetWindowText(S_CA2W(strName.c_str()));

			//设置消息内容
			SStringW sstrContent = L"";
			auto iterContent = CGlobalUnits::GetInstance()->m_mapPersonalLasttalkContent.find(pItemData->m_strID);
			if (iterContent != CGlobalUnits::GetInstance()->m_mapPersonalLasttalkContent.end())
				sstrContent = S_CA2W(iterContent->second.c_str());
			pContent->SetWindowText(sstrContent);

			//设置时间
			SStringW sstrTime = L"";
			auto iterTime = CGlobalUnits::GetInstance()->m_mapPersonalLasttalkTime.find(pItemData->m_strID);
			if (iterTime != CGlobalUnits::GetInstance()->m_mapPersonalLasttalkTime.end())
			{
				SYSTEMTIME lpsystime;
				GetLocalTime(&lpsystime);
				time_t tt = time(NULL);
				tm* tCurr = localtime(&tt);
				tm* t = localtime(&iterTime->second);
				if (tCurr->tm_year == t->tm_year && tCurr->tm_mon == t->tm_mon && tCurr->tm_mday == t->tm_mday)
				{
					sstrTime.Format(L"%02d:%02d", t->tm_hour, t->tm_min);
				}
				else if (tCurr->tm_year == t->tm_year && tCurr->tm_mon == t->tm_mon && tCurr->tm_mday - 1 == t->tm_mday)
					sstrTime = L"昨天";
				else
					sstrTime.Format(L"%02d月%02d日", t->tm_mon + 1, t->tm_mday);
			}
			pTime->SetWindowText(sstrTime);

			//设置消息提醒
			auto iterDisturb = CGlobalUnits::GetInstance()->m_mapPersonalDisturb.find(pItemData->m_strID);
			if (iterDisturb != CGlobalUnits::GetInstance()->m_mapPersonalDisturb.end())
			{
				if (!iterDisturb->second)
					pReminder->SetAttribute(L"show", L"1");
				else
					pReminder->SetAttribute(L"show", L"0");
			}
			else
				pReminder->SetAttribute(L"show", L"1");

			//设置消息未读相关
			auto iterRead = CGlobalUnits::GetInstance()->m_mapPersonalRead.find(pItemData->m_strID);
			if (iterRead != CGlobalUnits::GetInstance()->m_mapPersonalRead.end())
			{
				if (!iterRead->second)
					pRead->SetAttribute(L"show", L"1");
				else
					pRead->SetAttribute(L"show", L"0");
			}
			else
				pRead->SetAttribute(L"show", L"1");
		}
		break;
		case 2:
		{
			std::string strName = "";
			GroupsMap::iterator iter = CGlobalUnits::GetInstance()->m_mapGroups.find(pItemData->m_strID);
			if (iter != CGlobalUnits::GetInstance()->m_mapGroups.end())
				strName = iter->second.m_strGroupName;
			else
				strName = pItemData->m_strID;
			pName->SetWindowText(S_CA2W(strName.c_str()));

			//设置消息内容
			SStringW sstrContent = L"";
			auto iterContent = CGlobalUnits::GetInstance()->m_mapGroupLasttalkContent.find(pItemData->m_strID);
			if (iterContent != CGlobalUnits::GetInstance()->m_mapGroupLasttalkContent.end())
				sstrContent = S_CA2W(iterContent->second.c_str());
			pContent->SetWindowText(sstrContent);

			//设置时间
			SStringW sstrTime = L"";
			auto iterTime = CGlobalUnits::GetInstance()->m_mapGroupLasttalkTime.find(pItemData->m_strID);
			if (iterTime != CGlobalUnits::GetInstance()->m_mapGroupLasttalkTime.end())
			{
				SYSTEMTIME lpsystime;
				GetLocalTime(&lpsystime);
				time_t tt = time(NULL);
				tm* tCurr = localtime(&tt);
				tm* t = localtime(&iterTime->second);
				if (tCurr->tm_year == t->tm_year && tCurr->tm_mon == t->tm_mon && tCurr->tm_mday == t->tm_mday)
				{
					sstrTime.Format(L"%02d:%02d", t->tm_hour, t->tm_min);
				}
				else if (tCurr->tm_year == t->tm_year && tCurr->tm_mon == t->tm_mon && tCurr->tm_mday - 1 == t->tm_mday)
					sstrTime = L"昨天";
				else
					sstrTime.Format(L"%02d月%02d日", t->tm_mon + 1, t->tm_mday);
			}
			pTime->SetWindowText(sstrTime);

			//设置消息提醒
			auto iterDisturb = CGlobalUnits::GetInstance()->m_mapGroupDisturb.find(pItemData->m_strID);
			if (iterDisturb != CGlobalUnits::GetInstance()->m_mapGroupDisturb.end())
			{
				if (!iterDisturb->second)
					pReminder->SetAttribute(L"show", L"1");
				else
					pReminder->SetAttribute(L"show", L"0");
			}
			else
				pReminder->SetAttribute(L"show", L"1");

			//设置消息未读相关
			auto iterRead = CGlobalUnits::GetInstance()->m_mapGroupRead.find(pItemData->m_strID);
			if (iterRead != CGlobalUnits::GetInstance()->m_mapGroupRead.end())
			{
				if (!iterRead->second)
					pRead->SetAttribute(L"show", L"1");
				else
					pRead->SetAttribute(L"show", L"0");
			}
			else
				pRead->SetAttribute(L"show", L"1");
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
	}

	void AddItem(int nType, SStringW sstrID)
	{
		ItemData* data = new ItemData;
		data->m_nType = nType;
		data->m_strID = S_CW2A(sstrID);
		m_vecItemDatas.push_back(data);

		notifyDataSetChanged();
	}

	void DeleteAllItem()
	{
		m_vecItemDatas.clear();
		notifyDataSetChanged();
	}

	ItemData* GetItemData(int& nIndex)
	{
		return m_vecItemDatas[nIndex];
	}

	void EnsureVisible(int& nIndex)
	{
		m_pOwner->EnsureVisible(nIndex);
		notifyDataSetChanged();
	}

	void SetCurSel(SStringW sstrID)
	{
		int nIndex = GetItemIndexByID(sstrID);
		if (-1 != nIndex)
			m_pOwner->SetSel(nIndex, TRUE);
	}

	int GetItemIndexByID(SStringW sstrID)
	{
		std::string strID = S_CW2A(sstrID);
		int nIndex = -1;
		for (int i = 0; i < m_vecItemDatas.size(); i++)
		{
			if (m_vecItemDatas[i]->m_strID == strID)
			{
				nIndex = i;
				break;
			}
		}
		return nIndex;
	}

	void Update()
	{
		notifyDataSetChanged();
	}

private:
	std::vector<ItemData*>				m_vecItemDatas;				// 数据 

	SListView* m_pOwner;
	IListen* m_pListen;
};

