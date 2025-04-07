#pragma once
#include "CGlobalUnits.h"
#include <helper/SAdapterBase.h>

struct TreeItemData
{
	TreeItemData() :bGroup(false) {}
	std::string strID;
	int32_t gid;			//用户的组ID
	SStringT strImg;		//用户图像
	SStringT strName;		//用户名
	bool bGroup;

};

class CTvContactAdapter : public STreeAdapterBase<TreeItemData>
{
public:
	struct IListen
	{
		virtual void ContactTVItemClick(int nGID, const std::string& strID) = 0;
		virtual void ContactTVItemDBClick(int nGID, const std::string& strID) = 0;
		virtual void ContactTVItemRClick(int nGID, const std::string& strID) = 0;
	};

public:
	CTvContactAdapter(IListen* pListen)
	{
		m_pListen = pListen;
	}

	~CTvContactAdapter() {}

	virtual void WINAPI getView(SOUI::HTREEITEM loc, SWindow* pItem, pugi::xml_node xmlTemplate)
	{
		ItemInfo& ii = m_tree.GetItemRef((HSTREEITEM)loc);
		int itemType = getViewType(loc);

		if (0 == pItem->GetChildrenCount())
		{
			switch (itemType)
			{
			case 0:xmlTemplate = xmlTemplate.child(L"item_group");
				break;
			case 1:xmlTemplate = xmlTemplate.child(L"item_data");
				break;
			default:
				break;
			}
			pItem->InitFromXml(xmlTemplate);
			if (itemType == 0)
			{//不让点	
				pItem->GetEventSet()->setMutedState(true);
			}
		}

		pItem->SetUserData(loc);

		if (itemType == 1)
		{
			//关联事件
			pItem->GetEventSet()->subscribeEvent(EVT_ITEMPANEL_CLICK,
				Subscriber(&CTvContactAdapter::OnItemPanelClick, this));
			pItem->GetEventSet()->subscribeEvent(EVT_ITEMPANEL_DBCLICK,
				Subscriber(&CTvContactAdapter::OnItemPanelDBClick, this));
			pItem->GetEventSet()->subscribeEvent(EVT_ITEMPANEL_RCLICK,
				Subscriber(&CTvContactAdapter::OnItemPanelRClick, this));

			//设置值
			SStatic* pName = pItem->FindChildByName2<SStatic>(L"name");
			SASSERT(pName);
			pName->SetWindowText(ii.data.strName);

			SImageWnd* pFace = pItem->FindChildByName2<SImageWnd>(L"face");
			SASSERT(pFace);
			pItem->FindChildByName(L"face")->SetAttribute(L"skin", S_CT2W(ii.data.strImg));
		}
		else
		{
			pItem->FindChildByName(L"hr")->SetVisible(ii.data.gid != 1);

			SStatic* pName = pItem->FindChildByName2<SStatic>(L"name");
			SASSERT(pName);
			pName->SetWindowText(ii.data.strName);
		}
	}

	virtual int WINAPI getViewType(HSTREEITEM hItem) const
	{
		ItemInfo& ii = m_tree.GetItemRef((HSTREEITEM)hItem);
		if (ii.data.bGroup) return 0;
		else return 1;
	}

	virtual int WINAPI getViewTypeCount() const
	{
		return 2;
	}

	bool OnItemPanelClick(EventArgs* e)
	{
		EventItemPanelClick* pEvt = sobj_cast<EventItemPanelClick>(e);
		if (NULL == pEvt) return true;

		SItemPanel* pItem = sobj_cast<SItemPanel>(pEvt->sender);
		if (NULL == pItem) return true;

		HSTREEITEM loc = (HSTREEITEM)pItem->GetItemIndex();
		ItemInfo& ii = m_tree.GetItemRef(loc);
		if (!ii.data.bGroup)
		{
			m_pListen->ContactTVItemClick(ii.data.gid, ii.data.strID);
		}

		return true;
	}

	bool OnItemPanelDBClick(EventArgs* e)
	{
		EventItemPanelDbclick* pEvt = sobj_cast<EventItemPanelDbclick>(e);
		if (NULL == pEvt) return true;

		SItemPanel* pItem = sobj_cast<SItemPanel>(pEvt->sender);
		if (NULL == pItem) return true;

		HSTREEITEM loc = (HSTREEITEM)pItem->GetItemIndex();
		ItemInfo& ii = m_tree.GetItemRef(loc);
		if (!ii.data.bGroup)
		{
			m_pListen->ContactTVItemDBClick(ii.data.gid, ii.data.strID);
		}

		return true;
	}

	bool OnItemPanelRClick(EventArgs* e)
	{
		EventItemPanelRclick* pEvt = sobj_cast<EventItemPanelRclick>(e);
		if (NULL == pEvt) return true;

		SItemPanel* pItem = sobj_cast<SItemPanel>(pEvt->sender);
		if (NULL == pItem) return true;

		HSTREEITEM loc = (HSTREEITEM)pItem->GetItemIndex();
		ItemInfo& ii = m_tree.GetItemRef(loc);
		if (!ii.data.bGroup)
		{
			m_pListen->ContactTVItemRClick(ii.data.gid, ii.data.strID);
		}

		return true;
	}

private:
	IListen* m_pListen;

public:
	void AddItem(TreeItemData& data)
	{
		if (data.bGroup)  //添加群组
		{
			HSTREEITEM hRoot = InsertItem(data);
			SetItemExpanded(hRoot, TRUE);
		}
		else
		{
			HSTREEITEM hParent = GetFirstVisibleItem();
			while (ITEM_NULL != hParent)
			{
				ItemInfo& info = m_tree.GetItemRef(hParent);
				if (info.data.bGroup && info.data.gid == data.gid)
				{
					InsertItem(data, hParent);
				}
				hParent = GetNextSiblingItem(hParent);
			}
		}

		notifyBranchChanged(ITvAdapter::ITEM_ROOT);
	}

	void DeleteItem(TreeItemData& data)
	{
		HSTREEITEM hParent = GetFirstVisibleItem();
		while (ITEM_NULL != hParent)
		{
			ItemInfo& info = m_tree.GetItemRef(hParent);
			if (info.data.bGroup && info.data.gid == data.gid)
			{
				HSTREEITEM hChild = GetFirstChildItem(hParent);
				while (ITEM_NULL != hChild)
				{
					ItemInfo& childinfo = m_tree.GetItemRef(hChild);
					if (childinfo.data.bGroup == 1 && childinfo.data.strID == data.strID)
					{
						m_tree.DeleteItem(hChild);
						break;
					}
					hChild = GetNextSiblingItem(hChild);
				}
			}
			hParent = GetNextSiblingItem(hParent);
		}
		notifyBranchChanged(ITvAdapter::ITEM_ROOT);
	}

	void ModifyItem(TreeItemData& data)
	{
		HSTREEITEM hParent = GetFirstVisibleItem();
		while (ITEM_NULL != hParent)
		{
			ItemInfo& info = m_tree.GetItemRef(hParent);
			if (info.data.bGroup && info.data.gid == data.gid)
			{
				HSTREEITEM hChild = GetFirstChildItem(hParent);
				while (ITEM_NULL != hChild)
				{
					ItemInfo& childinfo = m_tree.GetItemRef(hChild);
					if (childinfo.data.bGroup == 1 && childinfo.data.strID == data.strID)
					{
						childinfo.data = data;
						break;
					}
					hChild = GetNextSiblingItem(hChild);
				}
			}
			hParent = GetNextSiblingItem(hParent);
		}
		notifyBranchChanged(ITvAdapter::ITEM_ROOT);
	}
};

