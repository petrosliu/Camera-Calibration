#if !defined(AFX_DLGDLT_H__3885290F_1F9F_405F_B52E_F7A500B431E9__INCLUDED_)
#define AFX_DLGDLT_H__3885290F_1F9F_405F_B52E_F7A500B431E9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgDLT.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CDlgDLT dialog
#include "resource.h"

class CDlgDLT : public CDialog
{
// Construction
public:
	CDlgDLT();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgDLT)
	enum { IDD = IDD_DIALOG_DLT };
	CString	m_strFileCtrl;
	CString	m_strFileImg;
	//}}AFX_DATA

    CString *m_pstrFileImg;
	int     m_nFileImgSum;
	
	CString  GetFileCtrl()	  {  return m_strFileCtrl; }
	CString* GetFileImgs()    {  return m_pstrFileImg; }
    int      GetFileImgsSum() { return m_nFileImgSum; }


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgDLT)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgDLT)
	afx_msg void OnButtonCtrlDlg();
	afx_msg void OnButtonImgDlg();
	afx_msg void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGDLT_H__3885290F_1F9F_405F_B52E_F7A500B431E9__INCLUDED_)
