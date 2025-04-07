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
		SASSERT(pName);
		SStringW sstrName;
		switch (pItemData->m_nType)
		{
		case 0:
			sstrName = L"文件传输助手";
			break;
		case 1:
		{
			std::string strName = "";
			PersonalsMap::iterator iter = CGlobalUnits::GetInstance()->m_mapPersonals.find(pItemData->m_strID);
			if (iter != CGlobalUnits::GetInstance()->m_mapPersonals.end())
				strName = iter->second.m_strName;
			else
				strName = pItemData->m_strID;
			sstrName = S_CA2W(strName.c_str());
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
			sstrName = S_CA2W(strName.c_str());
		}
		break;
		default:
			break;
		}
		pName->SetWindowText(sstrName);

		//content
		SStatic* pContent = pItem->FindChildByName2<SStatic>(L"item_content");
		SASSERT(pContent);
		//pContent->SetWindowText(S_CW2T(pItemData->m_sstrContent));

		//time
		//根据定义的时间类型处理展示，如果是时间戳则需要处理，如果是文本则直接展示
		SStatic* pTime = pItem->FindChildByName2<SStatic>(L"item_time");
		SASSERT(pTime);
// 		pTime->SetWindowText(S_CW2T(pItemData->m_sstrTime));
// 
// 		//reminder
// 		SImageWnd* pReminder = pItem->FindChildByName2<SImageWnd>(L"item_reminder");
// 		SASSERT(pReminder);
// 		if (pItemData->m_bReminder)
// 		{
// 			//展示
// 			pReminder->SetAttribute(L"show", L"1");
// 		}
// 		else
// 		{
// 			//不展示
// 			pReminder->SetAttribute(L"show", L"0");
// 		}
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

private:
	std::vector<ItemData*>				m_vecItemDatas;				// 数据 

	SListView* m_pOwner;
	IListen* m_pListen;
};

