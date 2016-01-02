// demoView.h : interface of the CDemoView class
//
/////////////////////////////////////////////////////////////////////////////
#define CXIMAGE_DEMO_SELECT 1


#include"dlgdlt.h"
#include"cali.h"
#include "MainFrm.h"


class CDemoDoc;

class CDemoView : public CScrollView
{
protected: // create from serialization only
	CDemoView();
	DECLARE_DYNCREATE(CDemoView)

// Attributes
public:
	CDemoDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDemoView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
public:
	void GetImageXY(CDemoDoc *pDoc, CxImage *ima, long *x, long *y);
	virtual ~CDemoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

#if CXIMAGE_DEMO_SELECT
	long m_SelCount;
	long m_SelShift;
	void DrawSelection();
	CPoint m_RefPoint,m_RefScroll;
#endif

protected:
	CBrush m_brHatch;

// Generated message map functions
protected:
	//{{AFX_MSG(CDemoView)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnCalibrationInputgrdpos();
	afx_msg void OnCalibrationLoadcampara();
	afx_msg void OnCalibImgReSize();
	afx_msg void OnConcentricCenter();
	afx_msg void OnAdjust();
	afx_msg void OnImgpointComputeHtConcenteric();
	afx_msg void OnCalibConvertPurecolor();
	afx_msg void OnComputeError();
	afx_msg void OnInverseByX();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnSaveInverseImage();
	afx_msg void OnCpmputeVirtualCamera();
	afx_msg void OnVerifyAlgorithm();
	//}}AFX_MSG
	afx_msg LRESULT OnNewImage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnNewProgress(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

private:
	int IsPureColor(CxImage* img,int x,int y,int tolerance); 
	double RGBtoHSI(int r,int g,int b,double &m_dblHSIH, double &m_dblHSIS,double &m_dblHSII);
	double RGBmax(double r,double g,double b);
	double RGBmin(double r,double g,double b);

	bool ComputeLightcamera();
	bool ComputeObjectcamera();

	bool GenerateVirtualCamera( double r, double theta, double phi, 
								double x0, double y0, double z0, 
								double f, double w, double h);
	bool Rasterize(int &pos, double input, double noiseLevel);
//�����Уʹ�õĳ�Ա������
public:

	//ͼ��Ŀ��
	int nHeight; 
	int nWidth;

	//�����������m_HP�ӿռ�㷴���Ӧͼ���
	STRU_LFromH  m_HP;
	CPoint m_pt;         //�ֹ��������ά����	

private:
		
	int* myList;        // ������������ϵͬ��ԲԲ�ĵ�x��y�������Բ����Բ����ɫ
	int myListPointer;  // ģ����ָ��Ĺ��ܣ�ָ��myList��ջ�������Ԫ��

	int* myList_light;        // ������������ϵͬ��ԲԲ�ĵ�x��y�������Բ����Բ����ɫ
	int myListPointer_light;  // ģ����ָ��Ĺ��ܣ�ָ��myList��ջ�������Ԫ��

	STRU_LFromH  m_HP_object; //��������ϵ���������   
	STRU_LFromH  m_HP_light;  //��Դ����ϵ��������� 

private:
	//��������������ʾfloodfill�㷨RGB�������ݲ�
	int m_red;
	int m_green;
	int m_blue;

	//��Բ����Բ�뾶�ı���
	float m_ratio;

	//ɫ�����ݲ�
	int m_hue;

	//��Բ����ԲԲ�ĵĲ��
	int m_x;
	int m_y;
	
	//���ӵ���Ծ�Ĳ���
	int m_jump;
	float m_rectangle;

	///////////////////////////////////
	//Ϊ�˱궨�ƹ�����ӵı�����
	float m_ratio_light;
	float light_x;
	float light_y;
	float light_z;
	
	//�궨�鵱ǰ�궨��ı��
	int m_s;
	//
	BOOL m_bInverse;
};

#ifndef _DEBUG  // debug version in demoView.cpp
inline CDemoDoc* CDemoView::GetDocument()
   { return (CDemoDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////
