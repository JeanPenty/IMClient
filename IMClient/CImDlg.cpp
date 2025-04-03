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

	::DragAcceptFiles(m_hWnd, TRUE);
	::RegisterDragDrop(m_hWnd, GetDropTarget());


	SImRichEdit* pSendRichedit = FindChildByName2<SImRichEdit>(L"send_richedit");
	SUBSCRIBE(pSendRichedit, EVT_RE_QUERY_ACCEPT, CImDlg::OnSendRichEditAcceptData);

	return TRUE;
}

void CImDlg::OnBnClickSend()
{
	SImRichEdit* pRecvRichedit = FindChildByName2<SImRichEdit>(L"recv_richedit");
	SImRichEdit* pSendRichedit = FindChildByName2<SImRichEdit>(L"send_richedit");

	CHARRANGE chr = { 0, -1 };
	SStringW strContent = pSendRichedit->GetSelectedContent(&chr);
	pugi::xml_document doc;
	if (!doc.load_buffer(strContent, strContent.GetLength() * sizeof(WCHAR)))
		return;

	int nIncrement = 0;
	pugi::xml_node node = doc.child(L"RichEditContent").first_child();
	for (; node; node = node.next_sibling())
	{
	}
}

bool CImDlg::OnSendRichEditAcceptData(EventArgs* pEvt)
{
	EventQueryAccept* e = (EventQueryAccept*)pEvt;
	if (e->Conv->GetAvtiveFormat() == CF_HDROP)
	{
		::SetForegroundWindow(m_hWnd);
		RichFormatConv::DropFiles files = e->Conv->GetDropFiles();
		DragDropFiles(files);
		return true;
	}

	return true;
}

void CImDlg::DragDropFiles(RichFormatConv::DropFiles& files)
{
	SImRichEdit* pSendRichEdit = FindChildByName2<SImRichEdit>(L"send_richedit");

	RichFormatConv::DropFiles::iterator iter = files.begin();
	for (; iter != files.end(); ++iter)
	{
		SStringW strFile = *iter;
		int nFileSize;
		FILE* fp = _wfopen(strFile, L"rb");
		if (fp)
		{
			fseek(fp, 0L, SEEK_END);
			nFileSize = ftell(fp);
			rewind(fp);
			fclose(fp);
		}
		else
			return;

		SStringW sstrContent;
		sstrContent.Format(L"<RichEditContent><metafile file=\"%s\" /></RichEditContent>", *iter);
		pSendRichEdit->InsertContent(sstrContent, RECONTENT_CARET);
	}

	::SetFocus(m_hWnd);
	pSendRichEdit->SetFocus();
	pSendRichEdit->GetContainer()->OnShowCaret(TRUE);
}