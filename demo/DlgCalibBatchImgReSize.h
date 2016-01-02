#if !defined(AFX_DLGCALIBBATCHIMGRESIZE_H__1FE317CE_2413_4358_A0BC_FB3DD1C24405__INCLUDED_)
#define AFX_DLGCALIBBATCHIMGRESIZE_H__1FE317CE_2413_4358_A0BC_FB3DD1C24405__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCalibBatchImgReSize.h : header file
//

#include "ximage.h"
#include "FECFileDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgCalibBatchImgReSize dialog

class CDlgCalibBatchImgReSize : public CDialog
{
// Construction
public:
	CDlgCalibBatchImgReSize(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgCalibBatchImgReSize)
	enum { IDD = IDD_DIALOG_CALIBRATION_BATCH_IMAGERESIZE };
	CComboBox	m_cbFileType;
	int		m_nNewHeight;
	int		m_nNewWidth;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCalibBatchImgReSize)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCalibBatchImgReSize)
	afx_msg void OnButtonCalibrationLoadbatchimg();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonConvert();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	BOOL Convert(int index);
	CString FindExtension(const CString& name);
	CString m_ImgFilesPath[500]; //ͼ���ļ���
	int     m_nFileSum;       //ͼ����Ŀ

	CString	m_strOutputExt; //ת��ͼ������	
	int		m_nOutputType;
	CString	m_strInputExt; //Դͼ������	
	int		m_nInputType;

	BOOL    m_bError;		  //ת�����̳���;
	CxImage* image; //��ǰ�����ͼ��
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCALIBBATCHIMGRESIZE_H__1FE317CE_2413_4358_A0BC_FB3DD1C24405__INCLUDED_)
