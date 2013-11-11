#ifndef __CEFJAM_MFC_HTMLVIEW_H__
#define __CEFJAM_MFC_HTMLVIEW_H__


#include <afxhtml.h>			// MFC support for Windows 95 Common Controls

// CefJamMFCHtmlView html view

#ifndef HTMLVIEW_CONTROL_CLASS_TAG
#define HTMLVIEW_CONTROL_CLASS_TAG

#define HTMLVIEW_CONTROL_CLASS_CSTATIC	_T("CStatic")
#define HTMLVIEW_CONTROL_CLASS_CEDIT	_T("CEdit")

#endif//HTMLVIEW_CONTROL_CLASS_TAG

class CefJamMFCHtmlView : public CHtmlView
{
	DECLARE_DYNCREATE(CefJamMFCHtmlView)

public:
	CefJamMFCHtmlView();           // protected constructor used by dynamic creation
	virtual ~CefJamMFCHtmlView();

	BOOL CreateFromResource(UINT nID, CWnd* pParent,LPCTSTR csControlClass=HTMLVIEW_CONTROL_CLASS_CEDIT);

	//subclass from static control or only view
	void	SetTreatAsDocView(BOOL btreat);
	BOOL	GetTreatAsDocView(void);


protected:
	BOOL	m_bTreatAsDocView;

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual void PostNcDestroy();
public:
	virtual void OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, CByteArray& baPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel);
	virtual void OnNavigateComplete2(LPCTSTR strURL);
	virtual void OnNavigateError(LPCTSTR lpszURL, LPCTSTR lpszFrame, DWORD dwError, BOOL *pbCancel);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	// DocHostUIHandler overrideables
	virtual HRESULT OnGetExternal(LPDISPATCH *lppDispatch);
	virtual HRESULT OnGetHostInfo(DOCHOSTUIINFO *pInfo);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


#endif//__CEFJAM_MFC_HTMLVIEW_H__


