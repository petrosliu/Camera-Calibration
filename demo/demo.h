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
	int m_nDocCount;	               //文档个数
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


//以下内容用于相机检校中的全局数据存储。
public:
	bool m_bIsLoadCtrlPts;            //用于判断是否已经读取控制点文件(物体坐标系)
	POINT3D m_CtrlPts[10][36];            //已知的控制点坐标(物体坐标系)
	bool m_bIsLoadCtrlPts_light;      //用于判断是否已经读取控制点文件(光源坐标系)
	POINT3D m_CtrlPts_light[36];      //已知的控制点坐标(光源坐标系)
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
