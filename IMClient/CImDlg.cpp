#include "stdafx.h"
#include "CImDlg.h"

CImDlg::CImDlg(IListen* pListen)
	: SHostWnd(_T("LAYOUT:XML_DLG_IM"))
	, m_pListen(pListen)
{
	//
}

CImDlg::~CImDlg(void)
{
	//
}

BOOL CImDlg::OnInitDialog(HWND wndFocus, LPARAM lInitParam)
{
	SetMsgHandled(FALSE);
	return TRUE;
}