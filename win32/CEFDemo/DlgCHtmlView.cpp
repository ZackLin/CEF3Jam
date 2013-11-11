// DlgCHtmlView.cpp : implementation file
//

#include "stdafx.h"
#include "CEFDemo.h"
#include "DlgCHtmlView.h"
#include "afxdialogex.h"


// CDlgCHtmlView dialog

IMPLEMENT_DYNAMIC(CDlgCHtmlView, CDialogEx)

CDlgCHtmlView::CDlgCHtmlView(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgCHtmlView::IDD, pParent)
{
	m_pHtmlView=NULL;
}

CDlgCHtmlView::~CDlgCHtmlView()
{
	if (NULL!=m_pHtmlView)
	{
		m_pHtmlView->DestroyWindow();
		delete m_pHtmlView;
		m_pHtmlView=NULL;
	}
}

void CDlgCHtmlView::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgCHtmlView, CDialogEx)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CDlgCHtmlView message handlers


BOOL CDlgCHtmlView::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	CRect rcClient;
	this->GetClientRect(&rcClient);

	GetDlgItem(IDC_EDIT1)->MoveWindow(&rcClient);

	m_pHtmlView=new CefJamMFCHtmlView;
	m_pHtmlView->CreateFromResource(IDC_EDIT1,this);

	m_pHtmlView->Navigate2(_T("http://www.baidu.com"));

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgCHtmlView::OnSize(UINT nType, int cx, int cy)
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


BOOL CDlgCHtmlView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return FALSE;
	return CDialogEx::OnEraseBkgnd(pDC);
}
