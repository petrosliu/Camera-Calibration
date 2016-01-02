#if !defined(AFX_VERIFYDLG_H__A78AD873_B8EA_4484_8338_5DB3B5A712D5__INCLUDED_)
#define AFX_VERIFYDLG_H__A78AD873_B8EA_4484_8338_5DB3B5A712D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VerifyDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CVerifyDlg dialog

class CVerifyDlg : public CDialog
{
// Construction
public:
	CVerifyDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CVerifyDlg)
	enum { IDD = IDD_VERIFYALGRTHM };
	double m_nThetaS;
	double m_nPhiS;
	double m_nRadS;	
	double m_nNoiseS;

	double m_nThetaI;
	double m_nPhiI;
	double m_nRadI;	
	double m_nNoiseI;

	double m_nThetaE;
	double m_nPhiE;
	double m_nRadE;	
	double m_nNoiseE;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVerifyDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CVerifyDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VERIFYDLG_H__A78AD873_B8EA_4484_8338_5DB3B5A712D5__INCLUDED_)
