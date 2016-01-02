// demo.h : main header file for the DEMO application
//

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

#include "TypeDef.h"
#include "valdef.h"

/////////////////////////////////////////////////////////////////////////////
// CDemoApp:
// See demo.cpp for the implementation of this class
//

class CDemoApp : public CWinApp
{
public:
	CDemoApp();

	// Attributes
	int m_nDocCount;	               //�ĵ�����
	CMultiDocTemplate *demoTemplate;
	

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDemoApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Operations
	BOOL PromptForFileName(CString& fileName, UINT nIDSTitle, 
		DWORD dwFlags, BOOL bOpenFileDialog, int* pType=NULL);
	int GetIndexFromType(int nDocType, BOOL bOpenFileDialog);
	int GetTypeFromIndex(int nIndex, BOOL bOpenFileDialog);
	CString GetExtFromType(int nDocType);
	CString GetFileTypes(BOOL bOpenFileDialog);
	BOOL GetWritableType(int nDocType);

// Implementation

	//{{AFX_MSG(CDemoApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFileOpen() ;
	afx_msg void OnImageFromBitmap();
	afx_msg void OnCximageLoadjpegresource();
	afx_msg void OnCximageLoadiconresource();
	afx_msg void OnCximageLoadgifresource();
	afx_msg void OnCximageLoadpngresource();
	afx_msg void OnCximageLoadtifresource();
	afx_msg void OnWindowCloseall();
	afx_msg void OnCximageDemosavemultipagetiff();
	afx_msg void OnCximageDemosaveanimatedgif();
	afx_msg void OnCximageDownloadfromurl();
	afx_msg void OnCalibrationBatchImageresize();
	afx_msg void OnImgpointLoadctrlpnt();
	afx_msg void OnImportLightPoint();
	afx_msg void OnCalibrationBatchImagecut();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


//�����������������У�е�ȫ�����ݴ洢��
public:
	bool m_bIsLoadCtrlPts;            //�����ж��Ƿ��Ѿ���ȡ���Ƶ��ļ�(��������ϵ)
	POINT3D m_CtrlPts[10][36];            //��֪�Ŀ��Ƶ�����(��������ϵ)
	bool m_bIsLoadCtrlPts_light;      //�����ж��Ƿ��Ѿ���ȡ���Ƶ��ļ�(��Դ����ϵ)
	POINT3D m_CtrlPts_light[36];      //��֪�Ŀ��Ƶ�����(��Դ����ϵ)
	int nSurfSum, nPtSum;
};

extern CDemoApp theApp;

struct DocType
{
public:
	int nID;
	BOOL bRead;
	BOOL bWrite;
	const char* description;
	const char* ext;
};

CDemoApp * GetApp();

/////////////////////////////////////////////////////////////////////////////
