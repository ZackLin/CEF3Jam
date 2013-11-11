#ifndef __CEFJAM_CEF_HTMLVIEW_H__
#define __CEFJAM_CEF_HTMLVIEW_H__

#include "CefJamClientHandler.h"

// CefJamCEFHtmlView view


#ifndef HTMLVIEW_CONTROL_CLASS_TAG
#define HTMLVIEW_CONTROL_CLASS_TAG

#define HTMLVIEW_CONTROL_CLASS_CSTATIC	_T("CStatic")
#define HTMLVIEW_CONTROL_CLASS_CEDIT	_T("CEdit")

#endif//HTMLVIEW_CONTROL_CLASS_TAG


class CefJamCEFHtmlView : public CView
{
	DECLARE_DYNCREATE(CefJamCEFHtmlView)

public:
	CefJamCEFHtmlView();           // protected constructor used by dynamic creation
	virtual ~CefJamCEFHtmlView();

	BOOL CreateFromResource(UINT nID, CWnd* pParent,LPCTSTR csControlClass=HTMLVIEW_CONTROL_CLASS_CEDIT);

protected:
	CefRefPtr<CefJamClientHandler>	m_pClientHandler;

public:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void PostNcDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

#endif//__CEFJAM_CEF_HTMLVIEW_H__




