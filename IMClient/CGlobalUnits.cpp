#include "stdafx.h"
#include "CGlobalUnits.h"

CGlobalUnits::CGlobalUnits(void)
{

}

CGlobalUnits::~CGlobalUnits(void)
{
}

CGlobalUnits* CGlobalUnits::GetInstance()
{
	static CGlobalUnits _Instance;
	return &_Instance;
}

std::string CGlobalUnits::GenerateUUID()
{
	char szbuf[100];
	GUID guid;
	::CoCreateGuid(&guid);
	sprintf(szbuf,
		"%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X",
		guid.Data1,
		guid.Data2,
		guid.Data3,
		guid.Data4[0], guid.Data4[1],
		guid.Data4[2], guid.Data4[3],
		guid.Data4[4], guid.Data4[5],
		guid.Data4[6], guid.Data4[7]);

	std::string strUUID = szbuf;
	return strUUID;
}

void CGlobalUnits::OperateShamDate()
{
	const char* shamAreas[] = {
		"湖南",
		"湖北",
		"广东",
		"广西壮族自治区",
		"河南",
		"河北",
		"山东",
		"山西",
		"北京市",
		"重庆市",
		"天津市",
		"上海市",
		"江苏",
		"江西",
		"黑龙江",
		"浙江",
		"新疆维吾尔自治区",
		"宁夏回族自治区",
		"辽宁",
		"青海",
		"陕西",
		"甘肃",
		"云南",
		"贵州",
		"西藏自治区",
		"四川",
		"内蒙古自治区",
		"台湾",
		"海南",
		"福建",
		"吉林",
		"安徽",
		"香港特别行政区",
		"澳门特别行政区",
	};

	const char* shamNames[] = {
		"苏砚之",
		"顾清远",
		"沈云归",
		"谢临风",
		"萧疏寒",
		"陆怀瑾",
		"林栖鹤",
		"叶知秋",
		"白墨尘",
		"温言蹊",
		"江暮雪",
		"楚南星",
		"柳扶风",
		"傅长卿",
		"宋玉声",
		"秦观澜",
		"周明霁",
		"韩照夜",
		"沈听澜",
		"裴行舟",
		"程砚书",
		"洛星河",
		"燕归南",
		"花间酒",
		"季羡鱼",
		"云栖梧",
		"墨染川",
		"风止水",
		"楼寒月",
		"慕容雪",
		"兰亭序",
		"徐行止",
		"纪沉舟",
		"晏清和",
		"温如言",
		"苏枕书",
		"谢流云",
		"林慕白",
		"江浸月",
		"萧远山",
		"陆沉霜",
		"沈星河",
		"顾西辞",
		"叶孤城",
		"白夜行",
		"柳寒烟",
		"宋微凉",
		"秦无眠",
		"云栖竹",
		"花满楼",
		"顾清歌",
		"谢微澜",
		"陆听雪",
		"林晚照",
		"叶知微",
		"白落梅",
		"温如初",
		"江映月",
		"楚云舒",
		"傅青瓷",
		"宋清欢",
		"周梦璃",
		"韩雪落",
		"沈画眉",
		"裴烟萝",
		"程书瑶",
		"洛轻衣",
		"燕归晚",
		"云栖月",
		"墨染衣",
		"风吟袖",
		"楼清霜",
		"兰若兮",
		"纪沉鱼",
		"晏清浅",
		"温如许",
		"苏枕月",
		"谢流萤",
		"林慕雪",
		"江浸星",
		"萧远黛",
		"陆沉烟",
		"沈星晚",
		"顾西洲",
		"叶孤鸿",
		"白夜霜",
		"柳寒衣",
		"宋微雨",
		"秦无瑕",
		"云栖霞",
		"花满溪"
	};

	const char* shamGroupNames[] = {
		"测试群聊1",
		"花满溪、云栖霞，宋微雨群聊",
		"测试群聊",
		"码农集中营",
		"开发交流群",
		"IM技术交流群",
		"企业群",
		"西北狼",
		"西南F4",
		"长三角交流群",
		"山河四省",
		"广深1234"
	};

	//添加联系人的假数据
	{
		for (int i = 0; i < 10; i++)
		{
			std::string strUUID = GenerateUUID();
			int nNameIndex = rand() % 91;
			std::string strTempName = shamNames[nNameIndex];

			std::ostringstream os;
			os.str("");
			os << strTempName;
			std::string strName = os.str();
			int nAreaIndex = rand() % 33;
			std::string strArea = shamAreas[nAreaIndex];

			m_mapPersonals.insert(std::make_pair(strUUID,
				PERSONAL_INFO(strUUID, strName, "", strArea, "", "鸟随鸾凤腾飞远，人随贤良品自高。")));
		}
	}

	//添加群的假数据
	{
		for (int i = 0; i < 4; i++)
		{
			std::string strUUID = GenerateUUID();
			int nNameIndex = rand() % 11;
			std::string strTempName = shamGroupNames[nNameIndex];

			std::ostringstream os;
			os.str("");
			os << strTempName << i;
			std::string strName = os.str();

			m_mapGroups.insert(std::make_pair(strUUID, GROUP_INFO(strUUID, strName, "五一四天假")));
		}
	}
}

void CGlobalUnits::OperatePinyinMap(const std::string& strPath)
{
	m_PinyinMap.clear();

	std::ifstream  fin(strPath.c_str(), std::ios::in);
	char  line[1024] = { 0 };
	while (fin.getline(line, sizeof(line)))
	{
		std::string strLine = line;
		std::string strKey, strValue;
		std::string::size_type pos;
		pos = strLine.find_first_of(' ');
		if (std::string::npos != pos)
		{
			strKey = strLine.substr(0, pos);
			strValue = strLine.substr(pos + 1);
			std::wstring wstrKey, wstrValue;

			SStringW sstrKey = S_CA2W(strKey.c_str());
			SStringW sstrValue = S_CA2W(strValue.c_str());
			wstrKey = sstrKey;
			wstrValue = sstrValue;
			m_PinyinMap.insert(std::make_pair(wstrKey, wstrValue));
		}
	}
}

void CGlobalUnits::OperateSerachIndex()
{
	//处理搜索索引

	//personal 
	PersonalsMap::iterator iterPersonal = m_mapPersonals.begin();
	for (; iterPersonal != m_mapPersonals.end(); iterPersonal++)
	{
		SStringW sstrName = S_CA2W(iterPersonal->second.m_strName.c_str());
		std::wstring wstrName = sstrName;
		m_mapChineseSearch.insert(std::make_pair(wstrName, SEARCH_INFO(1, iterPersonal->second.m_strID)));

		std::wstring wstrSimple, wstrFull, wstrEncode;
		wstrEncode = EncodeChinese(wstrName.c_str());
		GetSimpleAndFull(wstrEncode, wstrSimple, wstrFull);

		m_mapPinyinSearch.insert(std::make_pair(wstrSimple, SEARCH_INFO(1, iterPersonal->second.m_strID)));
		m_mapPinyinSearch.insert(std::make_pair(wstrFull, SEARCH_INFO(1, iterPersonal->second.m_strID)));
	}

	//group
	GroupsMap::iterator iterGroup = m_mapGroups.begin();
	for (; iterGroup != m_mapGroups.end(); iterGroup++)
	{
		SStringW sstrName = S_CA2W(iterGroup->second.m_strGroupName.c_str());
		std::wstring wstrName = sstrName;
		m_mapChineseSearch.insert(std::make_pair(wstrName, SEARCH_INFO(2, iterGroup->second.m_strGroupID)));

		std::wstring wstrSimple, wstrFull, wstrEncode;
		wstrEncode = EncodeChinese(wstrName.c_str());
		GetSimpleAndFull(wstrEncode, wstrSimple, wstrFull);

		m_mapPinyinSearch.insert(std::make_pair(wstrSimple, SEARCH_INFO(1, iterGroup->second.m_strGroupID)));
		m_mapPinyinSearch.insert(std::make_pair(wstrFull, SEARCH_INFO(1, iterGroup->second.m_strGroupID)));
	}
}

void CGlobalUnits::OperateEmojis()
{
	/*	2019-03-26	yangjinpeng
	*	处理本地目录下的emoji图片
	*/
// 	std::string strPath = "emojis\\*.png";
// 	long handle;
// 	struct _finddata_t fileinfo;
// 	handle = _findfirst(strPath.c_str(), &fileinfo);
// 	if (handle == -1)
// 		return;
// 	do
// 	{
// 		std::string strUUID = GenerateUUID();
// 		m_mapEmojisIndex.insert(std::make_pair(strUUID, fileinfo.name));
// 
// 		std::string strTempPath;
// 		strTempPath.append("emojis\\");
// 		strTempPath.append(fileinfo.name);
// 		SStringW sstrPath = S_CA2W(strTempPath.c_str());
// 		IBitmap* pRes = SResLoadFromFile::LoadImage(sstrPath);
// 		if (pRes)
// 			m_mapFace.insert(std::make_pair(strUUID, pRes));
// 
// 	} while (!_findnext(handle, &fileinfo));
// 	_findclose(handle);
}

std::wstring CGlobalUnits::EncodeChinese(const std::wstring& wstrSrc)
{
	int cur;
	SStringW sstrSrc, sstrTemp, sstrReturn;
	sstrSrc = wstrSrc.c_str();

	for (int i = 0; i < sstrSrc.GetLength(); i++)
	{
		cur = sstrSrc.GetAt(i);
		sstrTemp.Format(L"%04X", cur);
		sstrReturn = sstrReturn + sstrTemp;
	}

	std::wstring wstrReturn = sstrReturn;
	return wstrReturn;
}

std::wstring CGlobalUnits::DecodeChinese(const std::wstring& wstrSrc)
{
	SStringW sstrSrc, sstrTemp, sstrReturn;
	sstrSrc = wstrSrc.c_str();

	for (int i = 0; i < sstrSrc.GetLength(); i += 4)
	{
		SStringW sstr = sstrSrc.Mid(i, 4);
		wchar_t* str1 = sstr.GetBuffer(sstr.GetLength() + 2);
		int x = (int)_tcstol(sstr, &str1, 16);//十六进制 

		sstrTemp.Format(L"%c", x);
		sstrReturn = sstrReturn + sstrTemp;
	}

	std::wstring wstrReturn = sstrReturn;
	return wstrReturn;
}

BOOL CGlobalUnits::GetSimpleAndFull(const std::wstring& wstrSrc, std::wstring& SimpleStr, std::wstring& FullStr)
{
	if (wstrSrc.empty())
		return FALSE;

	SStringW sstrSrc, sstrSimple, sstrFull, sstrTemp;
	sstrSrc = wstrSrc.c_str();
	for (int i = 0; i < sstrSrc.GetLength(); i += 4)
	{
		SStringW sstr = sstrSrc.Mid(i, 4);
		std::multimap<std::wstring, std::wstring>::iterator iter = m_PinyinMap.find((const wchar_t*)sstr);
		if (iter != m_PinyinMap.end())
			sstrTemp = iter->second.c_str();

		sstrFull = sstrFull + sstrTemp;
		sstrSimple = sstrSimple + sstrTemp.Left(1);
	}

	SimpleStr = sstrSimple;
	FullStr = sstrFull;

	return TRUE;
}

BOOL CGlobalUnits::IsIncludeChinese(std::wstring& wstrSrc)
{
	std::string strSrc = S_CW2A(wstrSrc.c_str());
	char* str = (char*)strSrc.c_str();
	char c;
	while (1)
	{
		c = *str++;
		if (c == 0) break;  //如果到字符串尾则说明该字符串没有中文字符
		if (c & 0x80)       //如果字符高位为1且下一字符高位也是1则有中文字符
			if (*str & 0x80)
				return TRUE;
	}
	return FALSE;
}