#pragma once
#include <string>
#include <map>
#include <vector>

typedef struct times
{
	int Year;
	int Mon;
	int Day;
	int Hour;
	int Min;
	int Second;
}Times;

typedef struct LvMessageCurSel
{
	std::string m_strID;
	int			m_nType;
}LVMESSAGE_SURSEL;

typedef struct struPersonalInfo
{
	std::string m_strID;
	std::string m_strName;
	std::string m_strRemark;
	std::string m_strArea;
	std::string m_strOrigin;
	std::string m_strSign;

	struPersonalInfo() {
		m_strID = "";
		m_strName = "";
		m_strRemark = "";
		m_strArea = "";
		m_strOrigin = "";
		m_strSign = "";
	}
	struPersonalInfo(const std::string& strID, const std::string& strName, const std::string& strRemark,
		const std::string& strArea, const std::string& strOrigin, const std::string& strSign) {
		m_strID = strID;
		m_strName = strName;
		m_strRemark = strRemark;
		m_strArea = strArea;
		m_strOrigin = strOrigin;
		m_strSign = strSign;
	}
}PERSONAL_INFO;
typedef std::map<std::string, PERSONAL_INFO>	PersonalsMap;

typedef struct struGroupInfo
{
	std::string m_strGroupID;
	std::string m_strGroupName;
	std::string m_strGroupNotice;

	struGroupInfo() {
		m_strGroupID = "";
		m_strGroupName = "";
		m_strGroupNotice = "";
	}
	struGroupInfo(const std::string& strGroupID, const std::string& strGroupName,
		const std::string& strGroupNotice) {
		m_strGroupID = strGroupID;
		m_strGroupName = strGroupName;
		m_strGroupNotice = strGroupNotice;
	}
	std::vector<std::string> m_vecGroupMembers;
}GROUP_INFO;
typedef std::map<std::string, GROUP_INFO>		GroupsMap;

typedef struct struGrpmbrInfo
{
	std::string m_strGrpmbrID;
	std::string m_strGrpmbrName;
	std::string m_strGrpmbrArea;
	struGrpmbrInfo() {
		m_strGrpmbrID = "";
		m_strGrpmbrName = "";
		m_strGrpmbrArea = "";
	}
	struGrpmbrInfo(const std::string& strGrpmbrID, const std::string& strGrpmbrName,
		const std::string& strGrpmbrArea) {
		m_strGrpmbrID = strGrpmbrID;
		m_strGrpmbrName = strGrpmbrName;
		m_strGrpmbrArea = strGrpmbrArea;
	}
}GRPMBR_INFO;
typedef std::map<std::string, GRPMBR_INFO>		GroupMembersMap;

typedef struct struGHInfo
{
	std::string m_strGhID;
	std::string m_strName;
	std::string m_strSummary;
	struGHInfo() {
		m_strGhID = "";
		m_strName = "";
		m_strSummary = "";
	}
	struGHInfo(const std::string& strID, const std::string& strName, const std::string& strSummary) {
		m_strGhID = strID;
		m_strName = strName;
		m_strSummary = strSummary;
	}
}GH_INFO;
typedef std::map<std::string, GH_INFO>			GHsMap;

typedef struct struSearchInfo
{
	int m_nType;
	std::string m_strID;

	struSearchInfo() {
		m_nType = -1;
		m_strID = "";
	}
	struSearchInfo(int nType, const std::string& strID) {
		m_nType = nType;
		m_strID = strID;
	}

public:
	bool operator<(const struSearchInfo& info) const
	{
		return this->m_strID < info.m_strID;
	}
	bool operator==(const struSearchInfo& info) const
	{
		return this->m_strID == info.m_strID;
	}
}SEARCH_INFO;
typedef std::multimap<std::wstring, SEARCH_INFO> SearchInfosMap;

typedef struct struMessageDetail
{
	int m_nTalkType;	//聊天类型，比如群聊、个人聊天等
	int m_nMessageType;	//消息类型，比如文本、图片、文件等
	std::string m_strTimestamp;	//消息产生的时间
	std::string m_strMessageID;	//消息ID
	std::string m_strFrom;	//谁发送的
	std::string m_strTo;	//发给谁的
}MessageDetail;