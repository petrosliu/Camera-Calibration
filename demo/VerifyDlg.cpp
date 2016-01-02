// VerifyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "demo.h"
#include "VerifyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVerifyDlg dialog


CVerifyDlg::CVerifyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVerifyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVerifyDlg)
	m_nThetaS=0.0f;
	m_nPhiS=0.0f;
	m_nRadS=0.0f;	
	m_nNoiseS=0.0f;

	m_nThetaI=0.0f;
	m_nPhiI=0.0f;
	m_nRadI=0.0f;	
	m_nNoiseI=0.0f;

	m_nThetaE=0.0f;
	m_nPhiE=0.0f;
	m_nRadE=0.0f;	
	m_nNoiseE=0.0f;
	//}}AFX_DATA_INIT
}


void CVerifyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVerifyDlg)
	DDX_Text(pDX, IDC_THETAS, m_nThetaS);
	DDV_MinMaxFloat(pDX, m_nThetaS, 0.f, 90.f);	
	DDX_Text(pDX, IDC_THETAI, m_nThetaI);
	DDV_MinMaxFloat(pDX, m_nThetaI, 0.f, 90.f);
	DDX_Text(pDX, IDC_THETAE, m_nThetaE);
	DDV_MinMaxFloat(pDX, m_nThetaE, 0.f, 90.f);	
	DDX_Text(pDX, IDC_PHIS, m_nPhiS);
	DDV_MinMaxFloat(pDX, m_nPhiS, 0.f, 360.f);	
	DDX_Text(pDX, IDC_PHII, m_nPhiI);
	DDV_MinMaxFloat(pDX, m_nPhiI, 0.f, 360.f);	
	DDX_Text(pDX, IDC_PHIE, m_nPhiE);
	DDV_MinMaxFloat(pDX, m_nPhiE, 0.f, 360.f);
	DDX_Text(pDX, IDC_RADS, m_nRadS);
	DDV_MinMaxFloat(pDX, m_nRadS, 0.f, 1000.f);
	DDX_Text(pDX, IDC_RADI, m_nRadI);
	DDV_MinMaxFloat(pDX, m_nRadI, 0.f, 1000.f);
	DDX_Text(pDX, IDC_RADE, m_nRadE);
	DDV_MinMaxFloat(pDX, m_nRadE, 0.f, 1000.f);
	DDX_Text(pDX, IDC_NOISES, m_nNoiseS);
	DDV_MinMaxFloat(pDX, m_nNoiseS, 0.f, 100.f);
	DDX_Text(pDX, IDC_NOISEI, m_nNoiseI);
	DDV_MinMaxFloat(pDX, m_nNoiseI, 0.f, 100.f);
	DDX_Text(pDX, IDC_NOISEE, m_nNoiseE);
	DDV_MinMaxFloat(pDX, m_nNoiseE, 0.f, 100.f);

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CVerifyDlg, CDialog)
	//{{AFX_MSG_MAP(CVerifyDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVerifyDlg message handlers

BOOL CVerifyDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_nThetaS=0;//start
	m_nPhiS=0;
	m_nRadS=30;
	m_nNoiseS=0;

	m_nThetaI=10;//interval
	m_nPhiI=10;
	m_nRadI=10;
	m_nNoiseI=0.5;

	m_nThetaE=90;//end
	m_nPhiE=360;
	m_nRadE=70;
	m_nNoiseE=5;
	
	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
