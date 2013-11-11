#pragma once

#include "CefJamMFCHtmlView.h"
// CDlgCHtmlView dialog

class CDlgCHtmlView : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgCHtmlView)

public:
	CDlgCHtmlView(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgCHtmlView();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	CefJamMFCHtmlView* m_pHtmlView;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
