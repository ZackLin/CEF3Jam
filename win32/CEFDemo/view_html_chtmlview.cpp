// view_html.cpp : implementation file
//

#include "stdafx.h"
#include "view_html_chtmlview.h"


// view_html

IMPLEMENT_DYNCREATE(view_html, CHtmlView)

view_html::view_html()
{
	this->EnableAutomation();
}

view_html::~view_html()
{
}

void view_html::DoDataExchange(CDataExchange* pDX)
{
	CHtmlView::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(view_html, CHtmlView)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_EDIT_SELECT_ALL, OnEditSelectAll)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(view_html, CHtmlView)
	DISP_FUNCTION(view_html, "Provider", Provider, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(view_html, "GetLastUserEmail",GetLastUserEmail,VT_BSTR,VTS_NONE)
END_DISPATCH_MAP()

// view_html diagnostics

#ifdef _DEBUG
void view_html::AssertValid() const
{
	CHtmlView::AssertValid();
}

void view_html::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}
#endif //_DEBUG


// view_html message handlers
void view_html::PostNcDestroy() 
{  
	//delete it in cview::postncdestroy
	//CHtmlView::PostNcDestroy();
	//delete this;
}


void view_html::Provider(LPCTSTR url)
{
	
}

HRESULT view_html::OnGetHostInfo(DOCHOSTUIINFO *pInfo)
{
	HRESULT hr = CHtmlView::OnGetHostInfo(pInfo);

	if (NULL!=pInfo)
	{
		pInfo->dwFlags |= DOCHOSTUIFLAG_NO3DBORDER;
		pInfo->dwFlags |= DOCHOSTUIFLAG_SCROLL_NO;
	}

	return hr;
}

HRESULT view_html::OnGetExternal(LPDISPATCH *lppDispatch)
{
	*lppDispatch = GetIDispatch(TRUE);
	return S_OK;
}

void view_html::OnBeforeNavigate2( LPCTSTR lpszURL,
	DWORD nFlags,
	LPCTSTR lpszTargetFrameName,
	CByteArray& baPostedData,
	LPCTSTR lpszHeaders,
	BOOL* pbCancel )
{
	const TCHAR APP_PROTOCOL[] = _T("app:");
	int len = _tcslen(APP_PROTOCOL);
	if (_tcsnicmp(lpszURL, APP_PROTOCOL, len)==0) {
		OnAppCmd(lpszURL + len);
		*pbCancel = TRUE;
	}

	// cancel if not start with "http"
	const TCHAR HTTP_PROTOCOL[] = _T("http");
	len = _tcslen(HTTP_PROTOCOL);
	if (_tcsnicmp(lpszURL, HTTP_PROTOCOL, len) != 0)
	{
		*pbCancel = TRUE;
		if (!m_strUrl.IsEmpty())
		{
			Navigate2(m_strUrl);
		}
		return;
	}
	m_strUrl = lpszURL;

	this->AutoStoreUserEmail(lpszURL);
}
 
void	view_html::ReadHTMLUserName(CString tagUserName,CString& userName)
{
	IHTMLDocument2 *pHTMLDocument2=(IHTMLDocument2*)(this->GetHtmlDocument());
	if (NULL==pHTMLDocument2)
	{
		return;
	}
	IHTMLElementCollection* pColl = NULL;
	HRESULT hr;
	hr=pHTMLDocument2->get_all(&pColl);
	if(hr==S_OK && pColl)
	{
		LONG celem;
		hr=pColl->get_length(&celem);
		if(hr==S_OK)
		{
			VARIANT varIndex,var2;
			for(int i=0;i<celem;i++)
			{
				varIndex.vt=VT_UINT;
				varIndex.lVal=i;
				VariantInit(&var2);
				IDispatch* pDisp;
				hr=pColl->item(varIndex,var2,&pDisp);
				if(hr==S_OK)
				{
					IHTMLElement*pElem;
					hr=pDisp->QueryInterface(IID_IHTMLElement,(void**)&pElem);
					if(hr==S_OK)
					{
						CString ts;
						VARIANT_BOOL remember_pwd = -1;
						BSTR bs;
						pElem->get_tagName(&bs);
						ts=CString(bs);
						if(ts=="INPUT")
						{
							IHTMLInputElement* input;
							hr=pDisp->QueryInterface(IID_IHTMLInputElement,(void**)&input);
							input->get_name(&bs);
							ts=CString(bs);
							if(ts==tagUserName)
							{
								BSTR bstr_email;
								input->get_value(&bstr_email);
								
								userName=bstr_email;
							}
						}
					}
				}
			}
		}
	}
}
void	view_html::WriteHTMLUserName(CString tagUserName,CString userName)
{
	BOOL bSetUserEmail=FALSE;

	IHTMLDocument2 *pHTMLDocument2=(IHTMLDocument2*)(this->GetHtmlDocument());
	if (NULL==pHTMLDocument2)
	{
		return;
	}
	IHTMLElementCollection* pColl;
	HRESULT hr;
	hr=pHTMLDocument2->get_all(&pColl);
	if(hr==S_OK)
	{
		LONG celem;
		hr=pColl->get_length(&celem);
		if(hr==S_OK)
		{
			VARIANT varIndex,var2;
			for(int i=0;i<celem;i++)
			{
				varIndex.vt=VT_UINT;
				varIndex.lVal=i;
				VariantInit(&var2);
				IDispatch* pDisp;
				hr=pColl->item(varIndex,var2,&pDisp);
				if(hr==S_OK)
				{
					IHTMLElement*pElem;
					hr=pDisp->QueryInterface(IID_IHTMLElement,(void**)&pElem);
					if(hr==S_OK)
					{
						CString ts;
						VARIANT_BOOL remember_pwd = -1;
						BSTR bs;
						pElem->get_tagName(&bs);
						ts=CString(bs);
						if(ts=="INPUT")
						{
							IHTMLInputElement* input;
							hr=pDisp->QueryInterface(IID_IHTMLInputElement,(void**)&input);
							input->get_name(&bs);
							ts=CString(bs);
							if(ts==tagUserName)
							{
								BSTR bstr_email =  userName.AllocSysString();
								input->put_value(bstr_email);
								::SysFreeString(bstr_email);

								bSetUserEmail=TRUE;
							}
							else if (ts==_T("password"))
							{
								CComPtr<IHTMLControlElement> pControl=NULL;
								hr = input->QueryInterface(&pControl);
								//hr = pDisp->QueryInterface(&pControl);
								if (SUCCEEDED(hr) && NULL!=pControl && bSetUserEmail)
								{
									hr = pControl->focus();

		
									/*
									HWND hDest=this->m_hWnd;

									::PostMessage(hDest,WM_KEYDOWN,VK_TAB,0);
									::PostMessage(hDest,WM_KEYUP,VK_TAB,0);

						

									::PostMessage(hDest,WM_KEYDOWN,VK_TAB,0);
									::PostMessage(hDest,WM_KEYUP,VK_TAB,0);

								

									::PostMessage(hDest,WM_KEYDOWN,VK_TAB,0);
									::PostMessage(hDest,WM_KEYUP,VK_TAB,0);

									*/
								}
							}
						}
					}
				}
			}
		}
	}
}

BSTR view_html::GetLastUserEmail(void)
{
	CString csUserEmail;

	BSTR csRet=T2BSTR((LPCTSTR)csUserEmail);

	return csRet;
}

void view_html::AutoFillUserName(void)
{
	if (!m_csUserName.IsEmpty())
	{
		//this->WriteHTMLUserName(FOXIT_CLOUD_SSO_HTML_TAG_USEREMAIL,m_csUserName);
	}
	
}

void view_html::OnNavigateComplete2(LPCTSTR strURL)
{
	if (!m_myIEWnd.m_hWnd) {
		HWND hwnd = GetLastChild(m_hWnd);
		m_myIEWnd.SubclassWindow(hwnd);
	}
	
	m_strUrl.Empty();
	
	CHtmlView::OnNavigateComplete2(strURL);

}

void	view_html::AutoStoreUserEmail(CString csUrlParameter)
{
	
}

void view_html::OnDocumentComplete(LPCTSTR lpszURL)
{
	CHtmlView::OnDocumentComplete(lpszURL);
}

void view_html::OnAppCmd(LPCTSTR lpszWhere)
{
	// default: do nothing
}

void view_html::OnNavigateError(LPCTSTR lpszURL, LPCTSTR lpszFrame, DWORD dwError, BOOL *pbCancel)
{
	CHtmlView::OnNavigateError(lpszURL,lpszFrame,dwError,pbCancel);
}

HRESULT view_html::OnTranslateAccelerator(LPMSG lpMsg,
	const GUID* pguidCmdGroup, DWORD nCmdID)
{
	return CHtmlView::OnTranslateAccelerator(lpMsg,pguidCmdGroup,nCmdID);
}

BOOL view_html::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	return CHtmlView::PreTranslateMessage(pMsg);
}

void view_html::OnEditCut()  
{
	ExecWB(OLECMDID_CUT, OLECMDEXECOPT_DONTPROMPTUSER, NULL, NULL);   
}

void view_html::OnEditCopy()  
{
	ExecWB(OLECMDID_COPY, OLECMDEXECOPT_DONTPROMPTUSER, NULL, NULL);   
}

void view_html::OnEditPaste()  
{
	ExecWB(OLECMDID_PASTE, OLECMDEXECOPT_DONTPROMPTUSER, NULL, NULL);   
}

void view_html::OnEditSelectAll()  
{
	ExecWB(OLECMDID_SELECTALL, OLECMDEXECOPT_DONTPROMPTUSER, NULL, NULL);  
}


void	view_html::GotoProvider(CString csProvider)
{
	this->Navigate2((LPCTSTR)csProvider);
}

BOOL view_html::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
	return CHtmlView::OnEraseBkgnd(pDC);
}


void view_html::OnSize(UINT nType, int cx, int cy)
{
	CHtmlView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
}

HWND view_html::GetLastChild(HWND hwndParent)
{
	HWND hwnd = hwndParent;
	while (TRUE) {
		HWND hwndChild = ::GetWindow(hwnd, GW_CHILD);
		if (hwndChild==NULL)
			return hwnd;
		hwnd = hwndChild;
	}
	return NULL;
}



CMyIEWnd::CMyIEWnd()
{

}

CMyIEWnd::~CMyIEWnd()
{

}

IMPLEMENT_DYNAMIC(CMyIEWnd,CWnd)   

BEGIN_MESSAGE_MAP(CMyIEWnd, CWnd)
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()

void CMyIEWnd::OnContextMenu(CWnd* pWnd, CPoint pos)
{
	return;
}

