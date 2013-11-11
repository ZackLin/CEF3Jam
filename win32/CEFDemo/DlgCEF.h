#pragma once

#include "CefJamCEFHtmlView.h"

// CDlgCEF dialog

class CDlgCEF : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgCEF)

public:
	CDlgCEF(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgCEF();

// Dialog Data
	enum { IDD = IDD_DIALOG2 };
protected:
	CefJamCEFHtmlView* m_pHtmlView;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
