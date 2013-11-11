// DlgCEF.cpp : implementation file
//

#include "stdafx.h"
#include "CEFDemo.h"
#include "DlgCEF.h"
#include "afxdialogex.h"


// CDlgCEF dialog

IMPLEMENT_DYNAMIC(CDlgCEF, CDialogEx)

CDlgCEF::CDlgCEF(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgCEF::IDD, pParent)
{
	m_pHtmlView=NULL;
}

CDlgCEF::~CDlgCEF()
{
	if (NULL!=m_pHtmlView)
	{
		m_pHtmlView->DestroyWindow();
		delete m_pHtmlView;
		m_pHtmlView=NULL;
	}
}

void CDlgCEF::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgCEF, CDialogEx)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CDlgCEF message handlers


BOOL CDlgCEF::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	CRect rcClient;
	this->GetClientRect(&rcClient);

	GetDlgItem(IDC_EDIT1)->MoveWindow(&rcClient);

	m_pHtmlView=new CefJamCEFHtmlView;
	m_pHtmlView->CreateFromResource(IDC_EDIT1,this);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgCEF::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	CRect rcClient;
	this->GetClientRect(&rcClient);
	rcClient.right = rcClient.left+cx;
	rcClient.bottom = rcClient.top+cy;

	if (NULL!=m_pHtmlView)
	{
		m_pHtmlView->MoveWindow(&rcClient);
	}
}
