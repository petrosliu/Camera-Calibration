#if !defined(AFX_DLGGRDPTS_H__4BCB42A7_DFA0_452F_B4A0_D037E9D4F261__INCLUDED_)
#define AFX_DLGGRDPTS_H__4BCB42A7_DFA0_452F_B4A0_D037E9D4F261__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgGrdPts.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgGrdPts dialog

class CDlgGrdPts : public CDialog
{
// Construction
public:
	CDlgGrdPts(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgGrdPts)
	enum { IDD = IDD_DIALOG_SD_DATA };
	float	m_X;
	float	m_Y;
	float	m_Z;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgGrdPts)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgGrdPts)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGGRDPTS_H__4BCB42A7_DFA0_452F_B4A0_D037E9D4F261__INCLUDED_)
