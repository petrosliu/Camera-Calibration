#if !defined(AFX_DLGBATCHIMGCUT_H__03939D1E_95A3_444B_9381_FF0496DC62F1__INCLUDED_)
#define AFX_DLGBATCHIMGCUT_H__03939D1E_95A3_444B_9381_FF0496DC62F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgBatchImgCut.h : header file
//

#include "ximage.h"
#include "FECFileDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgBatchImgCut dialog


class CDlgBatchImgCut : public CDialog
{
// Construction
public:
	CDlgBatchImgCut(CWnd* pParent = NULL);   // standard constructor
	~CDlgBatchImgCut();

// Dialog Data
	//{{AFX_DATA(CDlgBatchImgCut)
	enum { IDD = IDD_DIALOG_BATCH_IMAGECUT };
	CStatic	m_ctrlImgZone;
	int		m_nBottomBoundary;
	int		m_nLeftBoundary;
	int		m_nRightBoundary;
	int		m_nTopBoundary;
	//}}AFX_DATA

private:
	int m_nImgFileSum;
	CString m_strImgFilePaths[500];
	CxImage *m_pSampleImg;
	int m_nImgWidth;
	int m_nImgHeight;
	bool m_bIsLeftMouseDown;
	CPoint m_pointMouseDown;
	CPoint m_pointCurrent;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgBatchImgCut)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgBatchImgCut)
	afx_msg void OnStartCut();
	afx_msg void OnButtonLoadImages();
	afx_msg void OnOk();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGBATCHIMGCUT_H__03939D1E_95A3_444B_9381_FF0496DC62F1__INCLUDED_)
