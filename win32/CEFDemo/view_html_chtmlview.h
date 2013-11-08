#pragma once

#ifdef _WIN32_WCE
#error "CHtmlView is not supported for Windows CE."
#endif 

#include <afxhtml.h>

class CMyIEWnd : public CWnd 
{
	DECLARE_DYNCREATE(CMyIEWnd)
public:
	CMyIEWnd();
	~CMyIEWnd();

public:
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint pos);
	DECLARE_MESSAGE_MAP()
};

// view_html html view

class view_html : public CHtmlView
{
	DECLARE_DYNCREATE(view_html)

public:
	view_html();           // protected constructor used by dynamic creation
	virtual ~view_html();

	//operations
public:
	//export to js
	void Provider(LPCTSTR url);
	BSTR GetLastUserEmail(void);
	//end

	void	GotoProvider(CString csProvider);

	void	AutoFillUserName(void);
	void	AutoStoreUserEmail(CString csUrlParameter);

	void	ReadHTMLUserName(CString tagUserName,CString& userName);
	void	WriteHTMLUserName(CString tagUserName,CString userName);
	// Normally, CHtmlView destroys itself in PostNcDestroy,
	// but we don't want to do that for a control since a control
	// is usually implemented as a stack object in a dialog.
	//
	virtual void PostNcDestroy();

	// DocHostUIHandler overrideables
	virtual HRESULT OnGetHostInfo(DOCHOSTUIINFO *pInfo);

	virtual HRESULT OnGetExternal(LPDISPATCH *lppDispatch);


	//event
	// override to trap "app:" pseudo protocol
	virtual void OnBeforeNavigate2( LPCTSTR lpszURL,
		DWORD nFlags,
		LPCTSTR lpszTargetFrameName,
		CByteArray& baPostedData,
		LPCTSTR lpszHeaders,
		BOOL* pbCancel );

	virtual void OnNavigateComplete2(LPCTSTR strURL);

	virtual void OnDocumentComplete(LPCTSTR lpszURL);

	HWND GetLastChild(HWND hwndParent);

	// override to handle links to "app:mumble...". lpszWhere will be "mumble"
	virtual void OnAppCmd(LPCTSTR lpszWhere);

	virtual void OnNavigateError(LPCTSTR lpszURL, LPCTSTR lpszFrame, DWORD dwError, BOOL *pbCancel);

	virtual HRESULT OnTranslateAccelerator(LPMSG lpMsg,
		const GUID* pguidCmdGroup, DWORD nCmdID);

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnEditCut();
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	afx_msg void OnEditSelectAll(); 

protected:
	CString	m_strProvider;

	CString m_csUserName;

	CMyIEWnd m_myIEWnd;

	CString		m_strUrl;
public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	DECLARE_DISPATCH_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};



