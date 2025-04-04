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

#include <event/NotifyCenter.h>
#include <layout/SouiLayoutParamStruct.h>


#include <com-cfg.h>
#include "trayicon/SShellNotifyIcon.h"
#include "resource.h"
#define R_IN_CPP	//定义这个开关来
#include "res\resource.h"
using namespace SOUI;

#include <Imm.h>
#pragma comment(lib, "imm32.lib")

#include <io.h>  
#include <Shlobj.h>  
#pragma comment(lib, "shell32.lib")

#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <fstream>
#include <iostream>

#include "MenuWrapper.h"

#include "utils.h"

#include "extend.ctrls/imre/SImRichEdit.h"
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