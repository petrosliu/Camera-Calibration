#if !defined(AFX_PURECOLORDLG_H__C97F0BDE_8B70_4A86_A850_AE2F6235B668__INCLUDED_)
#define AFX_PURECOLORDLG_H__C97F0BDE_8B70_4A86_A850_AE2F6235B668__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PureColorDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPureColorDlg dialog

class CPureColorDlg : public CDialog
{
// Construction
public:	
	CPureColorDlg(CWnd* pParent = NULL);   // standard constructor
	
// Dialog Data
	//{{AFX_DATA(CPureColorDlg)
	enum { IDD = IDD_PURECOLOR };
	CSliderCtrl	m_blue;
	CSliderCtrl	m_green;
	CSliderCtrl	m_red;
	CString	m_red_pos;
	CString	m_blue_pos;
	CString	m_green_pos;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPureColorDlg)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPureColorDlg)
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL OnInitDialog();
	afx_msg void OnAdjust();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	int GetBlue();
	int GetGreen();
	int GetRed();
private:
	CString IntToString(int);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PURECOLORDLG_H__C97F0BDE_8B70_4A86_A850_AE2F6235B668__INCLUDED_)
