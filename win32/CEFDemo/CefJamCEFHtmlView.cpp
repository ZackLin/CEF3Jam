// CefJamCEFHtmlView.cpp : implementation file
//

#include "stdafx.h"
#include "CEFDemo.h"
#include "CefJamCEFHtmlView.h"


// CefJamCEFHtmlView

IMPLEMENT_DYNCREATE(CefJamCEFHtmlView, CView)

CefJamCEFHtmlView::CefJamCEFHtmlView()
{

}

CefJamCEFHtmlView::~CefJamCEFHtmlView()
{
}

BEGIN_MESSAGE_MAP(CefJamCEFHtmlView, CView)
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CefJamCEFHtmlView drawing

void CefJamCEFHtmlView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}


// CefJamCEFHtmlView diagnostics

#ifdef _DEBUG
void CefJamCEFHtmlView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CefJamCEFHtmlView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CefJamCEFHtmlView message handlers
BOOL CefJamCEFHtmlView::CreateFromResource(UINT nID, CWnd* pParent,LPCTSTR csControlClass)
{
	CRect rc;

	CString csCompareClass=csControlClass;
	if (csCompareClass.CompareNoCase(HTMLVIEW_CONTROL_CLASS_CSTATIC)==0)
	{
		CStatic wndStatic;
		if (!wndStatic.SubclassDlgItem(nID, pParent))
			return FALSE;

		// Get static control rect, convert to parent's client coords.

		wndStatic.GetWindowRect(&rc);
		pParent->ScreenToClient(&rc);
		wndStatic.DestroyWindow();
	}
	else if (csCompareClass.CompareNoCase(HTMLVIEW_CONTROL_CLASS_CEDIT)==0)
	{
		CEdit wndEdit;
		if (!wndEdit.SubclassDlgItem(nID, pParent))
			return FALSE;

		// Get static control rect, convert to parent's client coords.

		wndEdit.GetWindowRect(&rc);
		pParent->ScreenToClient(&rc);
		wndEdit.DestroyWindow();
	}
	else
	{
		return FALSE;
	}

	// create HTML control (CHtmlView)
	return Create(NULL,						 // class name
		NULL,										 // title
		(WS_CHILD | WS_VISIBLE|WS_CLIPCHILDREN ),			 // style
		rc,										 // rectangle
		pParent,									 // parent
		nID,										 // control ID
		NULL);									 // frame/doc context not used
}


BOOL CefJamCEFHtmlView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return CView::OnEraseBkgnd(pDC);
}


int CefJamCEFHtmlView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	

	CRect rcClient;
	this->GetClientRect(&rcClient);

	// TODO:  Add your specialized creation code here
	CefRefPtr<CefJamClientHandler> client(new CefJamClientHandler());
	m_pClientHandler = client;

	CefWindowInfo info;
	info.SetAsChild( m_hWnd, rcClient);

	CefBrowserSettings browserSettings;
	
	//static_cast<CefRefPtr<CefClient> >(client)
	bool bCreate = CefBrowserHost::CreateBrowser( info,static_cast<CefRefPtr<CefClient> >(client), 
		"http://www.baidu.com", browserSettings);

	return 0;
}


void CefJamCEFHtmlView::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class

	//CView::PostNcDestroy();

}


void CefJamCEFHtmlView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if(m_pClientHandler.get())
	{
		CefRefPtr<CefBrowser> browser = m_pClientHandler->getBrowser();
		if(browser)
		{
			CefWindowHandle hwnd = browser->GetHost()->GetWindowHandle();
			RECT rect;
			this->GetClientRect(&rect);
			// ::SetWindowPos(hwnd, HWND_TOP, 0, 0, cx, cy, SWP_NOZORDER);
			::MoveWindow( hwnd, 0, 0, cx, cy, true);
		}
	}
}
