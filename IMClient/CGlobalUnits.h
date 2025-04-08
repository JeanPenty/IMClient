#pragma once
#include "apppublic.h"

class CGlobalUnits
{
public:
	CGlobalUnits(void);
	~CGlobalUnits(void);

	static CGlobalUnits* GetInstance();

public:
	PersonalsMap	m_mapPersonals;
	GroupsMap		m_mapGroups;
	GroupMembersMap	m_mapGrpmbrs;
	GHsMap			m_mapGhs;

	std::multimap<std::wstring, std::wstring>	m_PinyinMap;
	SearchInfosMap	m_mapPinyinSearch;
	SearchInfosMap	m_mapChineseSearch;

	//id-name
	std::map<std::string, std::string>			m_mapEmojisIndex;
	std::map<std::string, IBitmap*>				m_mapFace;

	//last_talk time
	std::map<std::string, std::string>			m_mapLasttalkTime;		//id-time

	LVMESSAGE_SURSEL	m_LvMessageCurSel;

	//置顶相关
	bool m_bFileHelperTopmost;
	bool m_bDYHTopmost;
	std::map<std::string, bool> m_mapPersonalTopmost;
	std::map<std::string, bool> m_mapGroupTopmost;
	std::map<std::string, bool> m_mapGZHTopmost;

	//已读、未读相关
	bool m_bFileHelperRead;
	bool m_bDYHRead;
	std::map<std::string, bool> m_mapPersonalRead;
	std::map<std::string, bool> m_mapGroupRead;
	std::map<std::string, bool> m_mapGZHRead;

	//消息免打扰相关
	bool m_bFileHelperDisturb;
	bool m_bDYHDisturb;
	std::map<std::string, bool> m_mapPersonalDisturb;
	std::map<std::string, bool> m_mapGroupDisturb;
	std::map<std::string, bool> m_mapGZHDisturb;

	//消息内容相关
	std::string m_strFileHelperLasttalkContent;
	std::string m_strDYHLasttalkContent;
	std::map<std::string, std::string> m_mapPersonalLasttalkContent;
	std::map<std::string, std::string> m_mapGroupLasttalkContent;
	std::map<std::string, std::string> m_mapGZHLasttalkContent;

	//消息时间相关
	time_t m_ttFileHelperLasttalkTime;
	time_t m_ttDYHLasttalkTime;
	std::map<std::string, time_t> m_mapPersonalLasttalkTime;
	std::map<std::string, time_t> m_mapGroupLasttalkTime;
	std::map<std::string, time_t> m_mapGZHLasttalkTime;
public:
	std::string GenerateUUID();
	void OperateShamDate();		
	void OperatePinyinMap(const std::string& strPath);	
	void OperateSerachIndex();

	void OperateEmojis();

	std::wstring EncodeChinese(const std::wstring& wstrSrc);
	std::wstring DecodeChinese(const std::wstring& wstrSrc);
	BOOL GetSimpleAndFull(const std::wstring& wstrSrc, std::wstring& SimpleStr, std::wstring& FullStr);
	BOOL IsIncludeChinese(std::wstring& wstrSrc);
};

#define GlobalUnits		CGlobalUnits::GetInstance()
