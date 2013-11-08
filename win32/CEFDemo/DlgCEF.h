#pragma once


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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
