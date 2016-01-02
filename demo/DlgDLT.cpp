// DlgDLT.cpp : implementation file
//

#include "stdafx.h"
#include "demo.h"

#include "cali.h"
#include "DlgDLT.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgDLT dialog


CDlgDLT::CDlgDLT()
	: CDialog(CDlgDLT::IDD)
{
	//{{AFX_DATA_INIT(CDlgDLT)
	m_strFileCtrl = _T("");
	m_strFileImg = _T("");
	//}}AFX_DATA_INIT
}


void CDlgDLT::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgDLT)
	DDX_Text(pDX, IDC_EDIT_CTRLPT_FILE, m_strFileCtrl);
	DDX_Text(pDX, IDC_EDIT_FILE_IMG, m_strFileImg);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgDLT, CDialog)
	//{{AFX_MSG_MAP(CDlgDLT)
	ON_BN_CLICKED(IDC_BUTTON_CTRL_DLG, OnButtonCtrlDlg)
	ON_BN_CLICKED(IDC_BUTTON_IMG_DLG, OnButtonImgDlg)
	ON_BN_CLICKED(ID_CANCEL, OnCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgDLT message handlers

void CDlgDLT::OnButtonCtrlDlg() 
{
	// TODO: Add your control notification handler code here
	char strFilter[]="text file(*.txt)|.txt|(*.bmp)|*.bmp||";
	CFileDialog fDlg(1,"txt","*.txt",OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,strFilter);
	fDlg.m_ofn.lpstrTitle="Select the control data file:";

	if(fDlg.DoModal()==IDOK){
		
		//m_pMainFrm->m_pOpti->LoadCtrlData(fDlg.GetPathName());
		
		m_strFileCtrl=fDlg.GetPathName();
	}

	UpdateData(0);
}

void CDlgDLT::OnButtonImgDlg() 
{
	// TODO: Add your control notification handler code here
	
	char strFilter[]="text file(*.txt)|.txt|(*.bmp)|*.bmp||";
	
	CFileDialog fDlg(1,"txt","*.txt",OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,strFilter);
	
	fDlg.m_ofn.Flags |=OFN_ALLOWMULTISELECT;
	fDlg.m_ofn.lpstrTitle="Select the image data file:";

	if(fDlg.DoModal()==IDOK){
		
		CString strFile[10];
		POSITION pos=fDlg.GetStartPosition();
		int nSum=0;
		while(pos!=NULL){
			strFile[nSum]=fDlg.GetNextPathName(pos);
			nSum++;
		}

		m_nFileImgSum=nSum;
		m_pstrFileImg=new CString[nSum];
		for(int i=0;i<nSum;i++)
		{
			m_pstrFileImg[i]=strFile[i];			
			m_strFileImg+=strFile[i];
		}

		//m_pMainFrm->m_pOpti=new COpti(nSum);
		//m_pMainFrm->m_pOpti->SetData(nSum);
		//读入图像坐标数据：
		//for ( i=0;i<nSum;i++){
		//	m_pMainFrm->m_pOpti->LoadImgData(strFile[i]);
		//}
			
		UpdateData(0);
	}
}

void CDlgDLT::OnCancel() 
{
	// TODO: Add your control notification handler code here
	
	CDialog::OnCancel();
}
