// DlgGrdPts.cpp : implementation file
//

#include "stdafx.h"
#include "demo.h"
#include "DlgGrdPts.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgGrdPts dialog


CDlgGrdPts::CDlgGrdPts(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgGrdPts::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgGrdPts)
	m_X = 0.0f;
	m_Y = 0.0f;
	m_Z = 0.0f;
	//}}AFX_DATA_INIT
}


void CDlgGrdPts::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgGrdPts)
	DDX_Text(pDX, IDC_EDIT_x, m_X);
	DDX_Text(pDX, IDC_EDIT_y, m_Y);
	DDX_Text(pDX, IDC_EDIT_Z, m_Z);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgGrdPts, CDialog)
	//{{AFX_MSG_MAP(CDlgGrdPts)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgGrdPts message handlers
