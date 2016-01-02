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
//相机检校使用的成员变量。
public:

	//图像的宽高
	int nHeight; 
	int nWidth;

	//利用相机参数m_HP从空间点反算对应图像点
	STRU_LFromH  m_HP;
	CPoint m_pt;         //手工输入的三维坐标	

private:
		
	int* myList;        // 保存物体坐标系同心圆圆心的x，y坐标和外圆，内圆的颜色
	int myListPointer;  // 模拟了指针的功能，指向myList堆栈最上面的元素

	int* myList_light;        // 保存物体坐标系同心圆圆心的x，y坐标和外圆，内圆的颜色
	int myListPointer_light;  // 模拟了指针的功能，指向myList堆栈最上面的元素

	STRU_LFromH  m_HP_object; //物体坐标系的相机参数   
	STRU_LFromH  m_HP_light;  //光源坐标系的相机参数 

private:
	//以下三个变量表示floodfill算法RGB分量的容差
	int m_red;
	int m_green;
	int m_blue;

	//外圆和内圆半径的比率
	float m_ratio;

	//色调的容差
	int m_hue;

	//外圆和内圆圆心的差距
	int m_x;
	int m_y;
	
	//种子点跳跃的步伐
	int m_jump;
	float m_rectangle;

	///////////////////////////////////
	//为了标定灯光而增加的变量；
	float m_ratio_light;
	float light_x;
	float light_y;
	float light_z;
	
	//标定块当前标定面的标号
	int m_s;
	//
	BOOL m_bInverse;
};

#ifndef _DEBUG  // debug version in demoView.cpp
inline CDemoDoc* CDemoView::GetDocument()
   { return (CDemoDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////
