#if !defined(AFX_CIRCLEPARADLG_H__5F441E38_C082_4561_A7DF_8C6DF95B7285__INCLUDED_)
#define AFX_CIRCLEPARADLG_H__5F441E38_C082_4561_A7DF_8C6DF95B7285__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CircleParaDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCircleParaDlg dialog

class CCircleParaDlg : public CDialog
{
// Construction
public:
	CCircleParaDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCircleParaDlg)
	enum { IDD = IDD_DIALOG1 };
	float	m_ratio;
	int		m_red;
	int		m_green;
	int		m_blue;
	int		m_hue;
	int		m_x;
	int		m_y;
	int		m_jump;
	float	m_rectangle;
	float	m_light_ratio;
	float	m_light_x;
	float	m_light_y;
	float	m_light_z;
	int		m_s;

	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCircleParaDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCircleParaDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnReset();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CIRCLEPARADLG_H__5F441E38_C082_4561_A7DF_8C6DF95B7285__INCLUDED_)
