#if !defined(AFX_DLGCALIBIMGRESIZE_H__D8D57E66_0F79_4C9E_BE21_9B3995BD654D__INCLUDED_)
#define AFX_DLGCALIBIMGRESIZE_H__D8D57E66_0F79_4C9E_BE21_9B3995BD654D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCalibImgReSize.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgCalibImgReSize dialog

class CDlgCalibImgReSize : public CDialog
{
// Construction
public:
	CDlgCalibImgReSize(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgCalibImgReSize)
	enum { IDD = IDD_DIALOG_CALIBRATION_IMAGERESIZE };
	int		m_nHeight;
	int		m_nWidth;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCalibImgReSize)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCalibImgReSize)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCALIBIMGRESIZE_H__D8D57E66_0F79_4C9E_BE21_9B3995BD654D__INCLUDED_)
