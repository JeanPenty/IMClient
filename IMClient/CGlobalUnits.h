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
