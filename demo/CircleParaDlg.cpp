  // CircleParaDlg.cpp : implementation file
//

#include "stdafx.h"
#include "demo.h"
#include "CircleParaDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCircleParaDlg dialog


CCircleParaDlg::CCircleParaDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCircleParaDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCircleParaDlg)
	m_ratio = 0.0f;
	m_red = 0;
	m_green = 0;
	m_blue = 0;
	m_hue = 0;
	m_x = 0;
	m_y = 0;
	m_jump = 0;
	m_rectangle = 0.0f;
	m_light_ratio = 0.0f;
	m_light_x = 0.0f;
	m_light_y = 0.0f;
	m_light_z = 0.0f;
	m_s = 0;
	//}}AFX_DATA_INIT
}


void CCircleParaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCircleParaDlg)
	DDX_Text(pDX, IDC_EDIT4, m_ratio);
	DDV_MinMaxFloat(pDX, m_ratio, 0.f, 1000.f);
	DDX_Text(pDX, IDC_EDIT1, m_red);
	DDV_MinMaxInt(pDX, m_red, 0, 255);
	DDX_Text(pDX, IDC_EDIT2, m_green);
	DDV_MinMaxInt(pDX, m_green, 0, 255);
	DDX_Text(pDX, IDC_EDIT3, m_blue);
	DDV_MinMaxInt(pDX, m_blue, 0, 255);
	DDX_Text(pDX, IDC_EDIT5, m_hue);
	DDV_MinMaxInt(pDX, m_hue, 0, 60);
	DDX_Text(pDX, IDC_EDIT6, m_x);
	DDV_MinMaxInt(pDX, m_x, 0, 30);
	DDX_Text(pDX, IDC_EDIT7, m_y);
	DDV_MinMaxInt(pDX, m_y, 0, 30);
	DDX_Text(pDX, IDC_EDIT8, m_jump);
	DDV_MinMaxInt(pDX, m_jump, 0, 1000);
	DDX_Text(pDX, IDC_EDIT9, m_rectangle);
	DDV_MinMaxFloat(pDX, m_rectangle, 0.f, 1000.f);
	DDX_Text(pDX, IDC_light_ratio, m_light_ratio);
	DDV_MinMaxFloat(pDX, m_light_ratio, 0.f, 1000.f);
	DDX_Text(pDX, IDC_light_x, m_light_x);
	DDX_Text(pDX, IDC_light_y, m_light_y);
	DDX_Text(pDX, IDC_light_z, m_light_z);
	DDX_Text(pDX, IDC_EDIT10, m_s);
	DDV_MinMaxInt(pDX, m_s, 0, 5);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCircleParaDlg, CDialog)
	//{{AFX_MSG_MAP(CCircleParaDlg)
	ON_BN_CLICKED(IDRESET, OnReset)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCircleParaDlg message handlers

BOOL CCircleParaDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_red = 30;
	m_green = 30;
	m_blue = 30;
	m_ratio = 2.0;
	m_hue = 30;
	m_x = 5;
	m_y = 5;
	m_jump = 8;
	m_rectangle = 2.0;

	m_light_ratio = 3.0f;
	m_light_x = 0.0f;
	m_light_y = 10.0f;
	m_light_z = 0.0f;
	
	m_s = 0;

	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCircleParaDlg::OnReset() 
{
	// TODO: Add your control notification handler code here
	m_red = 30;
	m_green = 30;
	m_blue = 30;
	m_ratio = 2.0;
	m_hue = 30;
	m_x = 30;
	m_y = 30;
	m_jump = 8;
	m_rectangle = 2.0;

	m_light_ratio = 3.0f;
	m_light_x = 0.0f;
	m_light_y = 0.0f;
	m_light_z = 10.0f;

	m_s = 1;

	UpdateData(false);	
}
