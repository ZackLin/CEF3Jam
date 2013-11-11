#include "StdAfx.h"
#include "CefJamMFCHtmlView.h"

// CefJamMFCHtmlView.cpp : implementation file
//


// CefJamMFCHtmlView

IMPLEMENT_DYNCREATE(CefJamMFCHtmlView, CHtmlView)

CefJamMFCHtmlView::CefJamMFCHtmlView()
{
	this->EnableAutomation();
	m_bTreatAsDocView=FALSE;
}

CefJamMFCHtmlView::~CefJamMFCHtmlView()
{
	m_bTreatAsDocView=FALSE;
}

void CefJamMFCHtmlView::DoDataExchange(CDataExchange* pDX)
{
	CHtmlView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CefJamMFCHtmlView, CHtmlView)
	ON_WM_MOUSEACTIVATE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CefJamMFCHtmlView diagnostics

#ifdef _DEBUG
void CefJamMFCHtmlView::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CefJamMFCHtmlView::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}
#endif //_DEBUG


// CefJamMFCHtmlView message handlers

BOOL CefJamMFCHtmlView::CreateFromResource(UINT nID, CWnd* pParent,LPCTSTR csControlClass)
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


void	CefJamMFCHtmlView::SetTreatAsDocView(BOOL btreat)
{
	m_bTreatAsDocView=btreat;
}
BOOL	CefJamMFCHtmlView::GetTreatAsDocView(void)
{
	return m_bTreatAsDocView;
}

void CefJamMFCHtmlView::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	//view destroy will
	//CHtmlView::PostNcDestroy();
}


BOOL CefJamMFCHtmlView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	return CHtmlView::PreTranslateMessage(pMsg);
}


int CefJamMFCHtmlView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// TODO: Add your message handler code here and/or call default
	if (m_bTreatAsDocView)
	{
		return CWnd::OnMouseActivate(pDesktopWnd,nHitTest,message);
	}

	return CHtmlView::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

HRESULT CefJamMFCHtmlView::OnGetExternal(LPDISPATCH *lppDispatch)
{
	*lppDispatch = GetIDispatch(TRUE);

	return S_OK;
}

HRESULT CefJamMFCHtmlView::OnGetHostInfo(DOCHOSTUIINFO *pInfo)
{
	HRESULT hr = CHtmlView::OnGetHostInfo(pInfo);

	if (NULL!=pInfo)
	{
		pInfo->dwFlags |= DOCHOSTUIFLAG_NO3DBORDER;
		pInfo->dwFlags |= DOCHOSTUIFLAG_SCROLL_NO;
	}

	return hr;
}

void CefJamMFCHtmlView::OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, CByteArray& baPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel)
{
	// TODO: Add your specialized code here and/or call the base class

	CHtmlView::OnBeforeNavigate2(lpszURL, nFlags, lpszTargetFrameName, baPostedData, lpszHeaders, pbCancel);
}


void CefJamMFCHtmlView::OnNavigateComplete2(LPCTSTR strURL)
{
	// TODO: Add your specialized code here and/or call the base class

	CHtmlView::OnNavigateComplete2(strURL);
}


void CefJamMFCHtmlView::OnNavigateError(LPCTSTR lpszURL, LPCTSTR lpszFrame, DWORD dwError, BOOL *pbCancel)
{
	// TODO: Add your specialized code here and/or call the base class

	CHtmlView::OnNavigateError(lpszURL, lpszFrame, dwError, pbCancel);
}



BOOL CefJamMFCHtmlView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return FALSE;
	return CHtmlView::OnEraseBkgnd(pDC);
}
