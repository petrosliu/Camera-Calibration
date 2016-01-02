// PureColorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "demo.h"
#include "PureColorDlg.h"
#include <stdlib.h>

#include "RRPolynomial.h"
#include "RRArray.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPureColorDlg dialog


CPureColorDlg::CPureColorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPureColorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPureColorDlg)
	m_red_pos = _T("30");
	m_blue_pos = _T("30");
	m_green_pos = _T("30");
	//}}AFX_DATA_INIT
}


void CPureColorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPureColorDlg)
	DDX_Control(pDX, IDC_SLIDER3, m_blue);
	DDX_Control(pDX, IDC_SLIDER2, m_green);
	DDX_Control(pDX, IDC_SLIDER1, m_red);
	DDX_Text(pDX, IDC_RED_POS, m_red_pos);
	DDV_MaxChars(pDX, m_red_pos, 3);
	DDX_Text(pDX, IDC_BLUE_POS, m_blue_pos);
	DDV_MaxChars(pDX, m_blue_pos, 3);
	DDX_Text(pDX, IDC_GREEN_POS, m_green_pos);
	DDV_MaxChars(pDX, m_green_pos, 3);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPureColorDlg, CDialog)
	//{{AFX_MSG_MAP(CPureColorDlg)
	ON_WM_HSCROLL()
	ON_COMMAND(ID_ADJUST, OnAdjust)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPureColorDlg message handlers

void CPureColorDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	if(pScrollBar->GetDlgCtrlID() == IDC_SLIDER1)
		m_red_pos = IntToString(m_red.GetPos());
	else
	if(pScrollBar->GetDlgCtrlID() == IDC_SLIDER2)
		m_green_pos = IntToString(m_green.GetPos());
	else if(pScrollBar->GetDlgCtrlID() == IDC_SLIDER3)
		m_blue_pos = IntToString(m_blue.GetPos());
	UpdateData(FALSE);
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

int CPureColorDlg::GetRed()
{
	//UpdateData(FALSE);
	//return m_red.GetPos();
	return 30;
}

int CPureColorDlg::GetGreen()
{
	//UpdateData(FALSE);
//	if(m_green.GetParent() == NULL)
//		MessageBox("hello");
//	int i = m_green.GetPos();
//	return i;
	return 30;
}

int CPureColorDlg::GetBlue()
{
	//UpdateData(FALSE);
	//return m_blue.GetPos();
	return 30;
}

BOOL CPureColorDlg::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CPureColorDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_red.SetRange(0,255);
	m_red.SetPos(30);
	m_red.SetTicFreq(10);
	m_green.SetRange(0,255);
	m_green.SetPos(30);
	m_green.SetTicFreq(10);
	m_blue.SetRange(0,255);
	m_blue.SetPos(30);
	m_blue.SetTicFreq(10);
	m_red.EnableWindow(FALSE);
	m_green.EnableWindow(FALSE);
	m_blue.EnableWindow(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

CString CPureColorDlg::IntToString(int a)
{
	int step = 10;
	CString temp,result; 
	temp = "";
	while(a > 9)
	{
		temp += a % step + '0';
		a = a/step;
	}
	if(a != 0) temp += '0' + a;
	if(temp.GetLength() != NULL)
	{
		for(int i = 0;i < temp.GetLength();i ++)
			result += temp[temp.GetLength() - 1 - i];
	}
	return result;

}

void CPureColorDlg::OnAdjust() 
{
	
}
