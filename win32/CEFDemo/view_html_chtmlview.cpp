// view_html.cpp : implementation file
//

#include "stdafx.h"
#include "CloudReadingPlugin.h"
#include "view_html.h"


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

void view_html::MonitorCookie(void)
{
	//monitor castgc
	CFC_WidgetMgr* pWidgetMgr=gCloudReadingPluginApp->GetWidgetMgr();
	if (NULL==pWidgetMgr)
	{
		return;
	}

	//monitor third-party account
	WIDGETHELPER_ACTION acion;
	acion.ActionID = ACTION_MGRHELPER_CLOSE_PROVIDER;
	pWidgetMgr->GetWidgetMgrHelper()->SendHelperAction(acion);

	//login success,notify change dialog
	CWidgetMgrHideWnd* pHideWnd = pWidgetMgr->GetWidgetMgrHideCWnd();

	if (NULL!=pHideWnd && NULL!=pHideWnd->m_hWnd)
	{
		if (!gGlobalData->IsUserLogin())
		{
			pHideWnd->PostMessage(WM_CR_MESSAGE_DO_MONITOR_CASTGC,0,0);
		}

	}
}

void view_html::Provider(LPCTSTR url)
{
	//AfxMessageBox(url);
	m_strProvider=url;
	if (m_strProvider.IsEmpty())
	{
		return;
	}

	WIDGETHELPER_ACTION action;
	action.ActionID = ACTION_MGRHELPER_SHOW_PROVIDER;
	action.csData = m_strProvider;
	gCloudReadingPluginApp->GetWidgetMgr()->GetWidgetMgrHelper()->SendHelperAction(action);
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

	// add by gm 20131031
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
	// add end 20131031
	
	//note by lmj 20130312
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
	gGlobalData->RestoreLatestLoginUserEmail(csUserEmail);

	BSTR csRet=T2BSTR((LPCTSTR)csUserEmail);

	return csRet;
}

void view_html::AutoFillUserName(void)
{
	gGlobalData->RestoreLatestLoginUserEmail(m_csUserName);
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
	// add by gm 20131031
	m_strUrl.Empty();
	// add end 20131031

	//this action before onDocumentComplete
	CHtmlView::OnNavigateComplete2(strURL);

	// modify by gm 
	// modify by gm 20130815
	/*CString str = USER_HOST;
	CString str2 = USER_HOST;

	str += FOXIT_CLOUD_SSO_USER_HOST_POSTFIX_BLANK;
	str2 += FOXIT_CLOUD_SSO_USER_HOST_POSTFIX_PROFILE;*/


	/*CString str = CCR_Module::Get()->QueryApi(CRAPI_user_login).c_str();
	CString str2 = CCR_Module::Get()->QueryApi(CRAPI_user_profile).c_str();
	CString strBlank = str+_T("/blank");//add by lmj 20130915
	// modify end 20130815
	*/
	DWORD dwServiceType = 0;
	CString strServer;
	CString	strObject;
	INTERNET_PORT nPort = 0;

	CString cslogin = CCR_Module::Get()->QueryApi(CRAPI_user_login).c_str();

	CString temp = CCR_Module::Get()->QueryApi(CRAPI_user_login).c_str();
	AfxParseURL(temp, dwServiceType, strServer, strObject, nPort);
	CString str = strObject;
	temp = CCR_Module::Get()->QueryApi(CRAPI_user_profile).c_str();
	AfxParseURL(temp, dwServiceType, strServer, strObject, nPort);
	CString str2 = strObject;
	CString strBlank = str+_T("/blank");
	
	CString csUrl=strURL;
	//if (csUrl.Find((LPCTSTR)strBlank)!=-1 || csUrl.Find((LPCTSTR)str)!=-1 || csUrl.Find((LPCTSTR)str2)!=-1)
	if (csUrl.Find((LPCTSTR)strBlank)!=-1 || csUrl.Find((LPCTSTR)str2)!=-1)
	{
		this->MonitorCookie();
	}

	CString csHost = CCR_Module::Get()->QueryHost(CRHOST_SSO).c_str();
	if (!csHost.IsEmpty() && csUrl.Find(csHost)!=-1)
	{
		this->AutoFillUserName();
	}
	// modify end

	CString cscode;
	this->ReadHTMLUserName(_T("PersistentCookie"),cscode);
	if (cscode.GetLength()!=0)
	{
	//	this->WriteHTMLUserName(_T("PersistentCookie"),_T("no"));
	}

	//add by lmj 20130904
	this->AutoStoreUserEmail(strURL);
}

void	view_html::AutoStoreUserEmail(CString csUrlParameter)
{
	//note by lmj 20130312
	//cannot collect cookie here until nagigatecomplete2
	//CString str = SSO_HOST_HTTPS;
	//str += FOXIT_CLOUD_SSO_USER_HOST_POSTFIX_LOGIN;
	CString str = CCR_Module::Get()->QueryHost(CRHOST_SSO).c_str();
	//str = str.Left(25);

	CString csUrl=csUrlParameter;
	if (csUrl.Find((LPCTSTR)str)!=-1)
	{
		this->ReadHTMLUserName(FOXIT_CLOUD_SSO_HTML_TAG_USEREMAIL,m_csUserName);
		if (!m_csUserName.IsEmpty())
		{
			gGlobalData->StoreLoginedUserEmail(m_csUserName);
		}
	}
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

BOOL	view_html::GotoFoxitCloudHome(void)
{
	//not title
	//SetWindowText(_T("Login"));
	
	// modify by gm 20130815
	CString csUserLogin = CCR_Module::Get()->QueryApi(CRAPI_user_login).c_str();
	if (csUserLogin.GetLength()==0)
	{
		return FALSE;
	}

	// modify end 20130815

	// 	CString csTmp;
	// 	csTmp.Format(_T("?%d"),FC_Rand());
	// 	csUserLogin += csTmp;
	
	//	csUserLogin = _T("http://www.baidu.com");
	//	csUserLogin = _T("http://www.google.com.hk");
	
	CString strLocale;
	CString strProfileHead;
	gGlobalData->GetLocaleString(strLocale,strProfileHead);
	// add postfix ?locale=zh-CN
	//add by lmj 20130503
	csUserLogin += _T("?locale=");
	csUserLogin += strLocale;
	//end

	// add by gm 20130729
	csUserLogin += _T("&alet=NzIwMA==");
	// add end

	//navNoReadFromCache|navNoHistory|navNoWriteToCache
	this->Navigate2((LPCTSTR)csUserLogin);
	return TRUE;
}

void	view_html::GotoFoxitCloudRegister(void)
{
	CString csUserRegister = CCR_Module::Get()->QueryApi(CRAPI_user_register).c_str();
	if (csUserRegister.GetLength()==0)
	{
		return;
	}
	
	CString strLocale;
	CString strProfileHead;
	gGlobalData->GetLocaleString(strLocale,strProfileHead);
	// add postfix ?locale=zh-CN
	//add by lmj 20130503
	csUserRegister += _T("?locale=");
	csUserRegister += strLocale;
	//end

	// add by gm 20130729
	csUserRegister += _T("&alet=NzIwMA==");
	// add end

	//navNoReadFromCache|navNoHistory|navNoWriteToCache
	this->Navigate2((LPCTSTR)csUserRegister);
}

void	view_html::GotoFoxitAccountProfile(void)
{
	//not title
	//SetWindowText(_T("Profile"));
	 
	CString strLocale;
	CString strProfileHead;
	gGlobalData->GetLocaleString(strLocale,strProfileHead);

	// modify by gm 20130815
	//CString csUserLogin = USER_HOST;
	//csUserLogin += FOXIT_CLOUD_SSO_USER_HOST_POSTFIX_PROFILE;
	CString csUserLogin = CCR_Module::Get()->QueryApi(CRAPI_user_profile).c_str();
	if (csUserLogin.IsEmpty())
		return;
	// modify end 20130815
	//	csUserLogin = _T("http://www.baidu.com");

	// add postfix ?locale=zh-CN
	//add by lmj 20130503
	csUserLogin += _T("?locale=");
	csUserLogin += strLocale;
	//end

	CString strLocation = this->GetLocationURL();
	if (strLocation == csUserLogin)
	{
		this->Refresh();
		return;
	}

	CString csHead=_T("");
	//cn
	//csHead.Format(_T("Accept-Language:zh-CN,en-US;q=0.8"));
	csHead=strProfileHead;
	//do not need it now

	//note by lmj 20130503
	//can not directly modify accept-language because its realtive IE tools-language-setting
	//using locale mechanism
	this->Navigate2((LPCTSTR)csUserLogin,NULL,NULL);
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


void view_html::OnRButtonDown(UINT nFlags, CPoint point)
{
	view_html::OnRButtonDown(nFlags, point);
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

