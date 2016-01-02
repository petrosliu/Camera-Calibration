// DlgCalibImgReSize.cpp : implementation file
//

#include "stdafx.h"
#include "demo.h"
#include "DlgCalibImgReSize.h"

//#include "demoDoc.h"
//#include "mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCalibImgReSize dialog


CDlgCalibImgReSize::CDlgCalibImgReSize(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCalibImgReSize::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCalibImgReSize)
	m_nHeight = 0;
	m_nWidth = 0;
	//}}AFX_DATA_INIT
}


void CDlgCalibImgReSize::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCalibImgReSize)
	DDX_Text(pDX, IDC_CALIBRATION_IMAGERESIZE_HEIGHT, m_nHeight);
	DDX_Text(pDX, IDC_CALIBRATION_IMAGERESIZE_WIDTH, m_nWidth);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCalibImgReSize, CDialog)
	//{{AFX_MSG_MAP(CDlgCalibImgReSize)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCalibImgReSize message handlers

BOOL CDlgCalibImgReSize::OnInitDialog() 
{
	CDialog::OnInitDialog();
	

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
