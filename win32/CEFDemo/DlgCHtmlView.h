#pragma once


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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
