// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//  are changed infrequently
//

#pragma once

#define  _CRT_SECURE_NO_WARNINGS
#define	 DLL_SOUI
#include <souistd.h>
#include <core/SHostDialog.h>
#include <control/SMessageBox.h>
#include <control/souictrls.h>
#include <res.mgr/sobjdefattr.h>
#include <com-cfg.h>
#include <event/NotifyCenter.h>
#include <layout/SouiLayoutParamStruct.h>
#include "trayicon/SShellNotifyIcon.h"

#include <helper/SMenuEx.h>

#include "resource.h"
#define R_IN_CPP	//定义这个开关来
#include "res\resource.h"

using namespace SOUI;


//用与做是否禁用输入法
#include <Imm.h>
#pragma comment(lib, "imm32.lib")

#include <io.h>  
#include <Shlobj.h>  
#pragma comment(lib, "shell32.lib")

#include <IPHlpApi.h>
#pragma comment(lib, "Iphlpapi.lib") 
#include <MMSystem.h>
#pragma comment(lib,"Winmm.lib")

#include <GdiPlus.h>
#pragma comment(lib, "Gdiplus.lib")
using namespace Gdiplus;

#include "CBitmap.h"
#include "CGdiDc.h"

#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <Mshtml.h>
#include <atlconv.h>
using namespace std;

#include <math.h>

#include "MenuWrapper.h"
#include "FileHelper.h"

#include "utils.h"
#include "extend.ctrls/imre/SImRichEdit.h"
#include "extend.ctrls/imre/ImgProvider.h"
#include "extend.ctrls/imre/RichEditObjEvents.h"
#include "extend.ctrls/SButtonEx.h"
#include "extend.ctrls/ExtendCtrls.h"
#include "extend.ctrls/SImageView.h"
#include "extend.ctrls/SListboxex.h"


#include "extend.skins/ExtendSkins.h"
#include "extend.skins/SAntialiasSkin.h"
#include "extend.skins/SVscrollbar.h"

#include "extend.events/ExtendEvents.h"

#include "extend.ctrls/gif/SAniImgFrame.h"
#include "extend.ctrls/gif/SGifPlayer.h"
#include "extend.ctrls/gif/SSkinAPNG.h"
#include "extend.ctrls/gif/SSkinGif.h"

//jsoncpp
#include "../jsoncpp/include/json/json.h"
#ifdef _DEBUG
#pragma comment(lib, "../jsoncpp/lib/Debug/jsoncpp.lib")
#else
#pragma comment(lib, "../jsoncpp/lib/Release/jsoncpp.lib")
#endif

#include "CBase64.h"





enum {
	EVT_SNAPSHOTFINISH = EVT_EXTERNAL_BEGIN + 0x0001,
	EVT_SNAPSHOTCANCEL,
};

//截图相关事件
class EventSnapshotFinish : public TplEventArgs<EventSnapshotFinish>
{
	SOUI_CLASS_NAME(EventSnapshotFinish, L"evtsnapshot_finish")
public:
	EventSnapshotFinish(SObject* pSender)
		: TplEventArgs<EventSnapshotFinish>(pSender) {}
	enum { EventID = EVT_SNAPSHOTFINISH };
};

class EventSnapshotCancel : public TplEventArgs<EventSnapshotCancel>
{
	SOUI_CLASS_NAME(EventSnapshotCancel, L"evtsnapshot_cancel")
public:
	EventSnapshotCancel(SObject* pSender)
		: TplEventArgs<EventSnapshotCancel>(pSender) {}
	enum { EventID = EVT_SNAPSHOTCANCEL };
};