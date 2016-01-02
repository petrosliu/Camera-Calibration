  // demoView.cpp : implementation of the CDemoView class
//

#include "stdafx.h"
#include "ximage.h"
#include "MainFrm.h"
#include "demo.h"
#include "PureColorDlg.h"
#include "demoDoc.h"
#include "demoView.h"
#include "memdc.h"
#include "math.h"
#include "DlgGrdPts.h"
#include"imagematching.h"
#include "CircleParaDlg.h"
#include "VerifyDlg.h"
#include "DlgCalibImgReSize.h"
#include"matrix.h"

#include "RRPolynomial.h"
#include "RRArray.h"


const int NOT_PURE_COLOR = 0;
const int RED = 1;
const int GREEN = 2;
const int BLUE = 3;
const int CYAN = 4;
const int MAGENTA = 5;
const int YELLOW = 6;

//#include "DlgCalibBatchImgReSize.h"
//全局变量
Calibration cali;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////////
// CDemoView

IMPLEMENT_DYNCREATE(CDemoView, CScrollView)

BEGIN_MESSAGE_MAP(CDemoView, CScrollView)
	//{{AFX_MSG_MAP(CDemoView)
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_SETFOCUS()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_CHAR()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_CALIBRATION_INPUTGRDPOS, OnCalibrationInputgrdpos)
	ON_COMMAND(ID_CALIBRATION_LOADCAMPARA, OnCalibrationLoadcampara)
	ON_COMMAND(ID_CALIBRATION_IMAGERESIZE, OnCalibImgReSize)
	ON_COMMAND(ID_IMGPOINT_COMPUTE_HT, OnConcentricCenter)
	ON_COMMAND(ID_ADJUST, OnAdjust)
	ON_COMMAND(ID_COMPUTE_CMR_PAR, OnImgpointComputeHtConcenteric)
	ON_COMMAND(ID_CALIB_CONVERT_PURECOLOR, OnCalibConvertPurecolor)
	ON_COMMAND(ID_COMPUTE_ERROR, OnComputeError)
	ON_COMMAND(ID_INVERSE_BY_X, OnInverseByX)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_SAVE_INVERSE_IMAGE, OnSaveInverseImage)
	ON_COMMAND(ID_CPMPUTE_VIRTUAL_CAMERA, OnCpmputeVirtualCamera)
	ON_COMMAND(ID_CONCENTRIC_CENTER, OnConcentricCenter)
	ON_COMMAND(ID_VERIFY_ALGORITHM, OnVerifyAlgorithm)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
	ON_MESSAGE(WM_USER_NEWIMAGE, OnNewImage)
	ON_MESSAGE(WM_USER_PROGRESS, OnNewProgress)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////
// CDemoView construction/destruction
CDemoView::CDemoView()
{

   SetScrollSizes(MM_TEXT, CSize(0, 0));
   VERIFY(m_brHatch.CreateHatchBrush(HS_DIAGCROSS, RGB(191, 191, 191)));

   myList = new int[100000];
   myListPointer = 0;
	
   myList_light = new int[100000];
   myListPointer_light = 0;
   
   m_HP.tz = (float)INT_MAX; //用于判断相机参数是否已经导入

   m_bInverse = FALSE;
}
//////////////////////////////////////////////////////////////////////////////
CDemoView::~CDemoView()
{
	delete []myList;
	delete []myList_light;
}
//////////////////////////////////////////////////////////////////////////////
BOOL CDemoView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	return CScrollView::PreCreateWindow(cs);
}
//////////////////////////////////////////////////////////////////////////////
// CDemoView drawing
void CDemoView::OnDraw(CDC* pDC)
{
	CDemoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	BOOL bPrinting = pDC->IsPrinting();

	CMemDC* pMemDC = NULL;
	if (!bPrinting) pDC = pMemDC = new CMemDC(pDC);

	//显示翻转图像
	if(m_bInverse)
	{
		CxImage *tempImg = pDoc->GetInverseImg();
		
		float zoom=pDoc->GetZoomFactor();
		if (zoom==1)
			tempImg->Draw(pDC->GetSafeHdc());
		else
		{
			CRect rect;
			rect.bottom = 0;
			rect.left = 0;
			rect.top = (int)(tempImg->GetHeight()*zoom);
			rect.right = (int)(tempImg->GetWidth()*zoom);
			tempImg->Draw(pDC->GetSafeHdc(),rect);
		}

		delete pMemDC;
		return;
	}

	//显示输入的图像
	CxImage* ima = pDoc->duplicate_image; 
	if (ima)
	{
		if (bPrinting) {
			// get size of printer page (in pixels)
			int cxPage = pDC->GetDeviceCaps(HORZRES);
			int cyPage = pDC->GetDeviceCaps(VERTRES);

			// get printer pixels per inch
			int cxInch = pDC->GetDeviceCaps(LOGPIXELSX);
			int cyInch = pDC->GetDeviceCaps(LOGPIXELSY);
			
			// Best Fit case: create a rectangle which preserves the aspect ratio
			int cx=(ima->GetWidth()*cxInch)/ima->GetXDPI();
			int cy=(ima->GetHeight()*cyInch)/ima->GetYDPI();
	
			CxImage tmp;
			tmp.Copy(*ima);
			RGBQUAD c={255,255,255,0};
			tmp.SetTransColor(c);
			tmp.AlphaStrip();
			tmp.Stretch(pDC->GetSafeHdc(), CRect(100,100,cx,cy));
		}
		else if (pDoc->GetStretchMode()) 
		{
			CRect rect;
			GetClientRect(&rect);
			ima->Draw(pDC->GetSafeHdc(),rect);
		}
		else
		{
			float zoom=pDoc->GetZoomFactor();
			if (zoom==1)
				ima->Draw(pDC->GetSafeHdc());
			else
				ima->Draw(pDC->GetSafeHdc(),
						CRect(0,0,(int)(ima->GetWidth()*zoom),(int)(ima->GetHeight()*zoom)));
		}
	}

	// 用黄笔画出用户输入的三维点
	CPen drawPenG(PS_SOLID,1,RGB(255,255,0) );
	CPen* oldPen =pDC->SelectObject(&drawPenG);	
	int u,v;

	u=m_pt.x;
	v=m_pt.y;
			
	CPoint point=GetScrollPosition();
	u = (int)( u*pDoc->GetZoomFactor() );
	v = (int)( v*pDoc->GetZoomFactor() );
	pDC->MoveTo(u-15, v-15);
	pDC->LineTo(u+15, v+15);
	pDC->MoveTo(u+15, v-15);
	pDC->LineTo(u-15, v+15);
	pDC->SelectObject(oldPen);

	
	//标出所有的识别出的图像上的控制点
	if(myListPointer == 0 && myListPointer_light == 0) 
	{
		delete pMemDC;
		return;
	}
	
	CString oo;
	int outer,iner,xxx,yyy;
	for(int s= 0;s < myListPointer;)
	{
		xxx = myList[s ++];
		yyy = myList[s ++];
		outer = myList[s ++];
		iner  = myList[s ++];

		oo = _T("");
		if(outer == RED)		oo += "R(";//"RED";
		if(outer == GREEN)		oo += "G(";//"GREEN";
		if(outer == BLUE)		oo += "B(";//"BLUE";
		if(outer == CYAN)		oo += "C(";//"CYAN";
		if(outer == MAGENTA)	oo += "M(";//"MAGENTA";
		if(outer == YELLOW)		oo += "Y(";//"YELLOW";
		if(iner == RED)			oo += "R)";//"RED";
		if(iner == GREEN)		oo += "G)";//"GREEN";
		if(iner == BLUE)		oo += "B)";//"BLUE";
		if(iner == CYAN)		oo += "C)";//"CYAN";
		if(iner == MAGENTA)		oo += "M)";//"MAGENTA";
		if(iner == YELLOW)		oo += "Y)";//"YELLOW";
		pDC->TextOut(int(pDoc->GetZoomFactor() * xxx + 5),int(pDoc->GetZoomFactor() * yyy + 5),oo);
	}
	
	for(s= 0;s < myListPointer_light;)
	{
		xxx = myList_light[s ++];
		yyy = myList_light[s ++];
		outer = myList_light[s ++];
		iner  = myList_light[s ++];

		oo = _T("");
		if(outer == RED)		oo += "R(";//"RED";
		if(outer == GREEN)		oo += "G(";//"GREEN";
		if(outer == BLUE)		oo += "B(";//"BLUE";
		if(outer == CYAN)		oo += "C(";//"CYAN";
		if(outer == MAGENTA)	oo += "M(";//"MAGENTA";
		if(outer == YELLOW)		oo += "Y(";//"YELLOW";
		if(iner == RED)			oo += "R)";//"RED";
		if(iner == GREEN)		oo += "G)";//"GREEN";
		if(iner == BLUE)		oo += "B)";//"BLUE";
		if(iner == CYAN)		oo += "C)";//"CYAN";
		if(iner == MAGENTA)		oo += "M)";//"MAGENTA";
		if(iner == YELLOW)		oo += "Y)";//"YELLOW";
		pDC->SetBkColor(RGB(0,0,0));
		pDC->SetTextColor(RGB(255,255,255));
		pDC->TextOut(int(pDoc->GetZoomFactor() * xxx + 5),int(pDoc->GetZoomFactor() * yyy + 5),oo);
	}

	delete pMemDC;
}



//////////////////////////////////////////////////////////////////////////////
// CDemoView printing
BOOL CDemoView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	pInfo->SetMaxPage(1);
	return DoPreparePrinting(pInfo);
}

//////////////////////////////////////////////////////////////////////////////
void CDemoView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

//////////////////////////////////////////////////////////////////////////////
void CDemoView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

//////////////////////////////////////////////////////////////////////////////
// CDemoView diagnostics
#ifdef _DEBUG
void CDemoView::AssertValid() const
{
	CScrollView::AssertValid();
}

//////////////////////////////////////////////////////////////////////////////
void CDemoView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

//////////////////////////////////////////////////////////////////////////////
CDemoDoc* CDemoView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDemoDoc)));

	//得到图像的宽高
	CxImage*  ima  = ((CDemoDoc*)m_pDocument)->GetImage();
	if(ima)
	{
		nHeight = ima->GetHeight();
		nWidth = ima->GetWidth();
	}
	//得到图像的宽高 end

	return (CDemoDoc*)m_pDocument;
}
#endif //_DEBUG

//////////////////////////////////////////////////////////////////////////////
void CDemoView::GetImageXY(CDemoDoc *pDoc, CxImage *ima, long *x, long *y)
{
	if (!ima || !pDoc) return;

	CPoint point=GetScrollPosition();
	float fx =(float)(*x + point.x);
	float fy =(float)(*y + point.y);
	
	fx/=pDoc->GetZoomFactor();
	fy/=pDoc->GetZoomFactor();
	
	if (pDoc->GetStretchMode())	{
		CRect rect;
		GetClientRect(&rect);
		int width = rect.right - rect.left;
		int height = rect.bottom - rect.top;
		fx *= ima->GetWidth()/(float)width;
		fy *= ima->GetHeight()/(float)height;
	}
	*x = (long)fx;
	*y = (long)fy;
}

//////////////////////////////////////////////////////////////////////////////
// CDemoView message handlers
//////////////////////////////////////////////////////////////////////////////
void CDemoView::OnMouseMove(UINT nFlags, CPoint point) 
{
	CDemoDoc* pDoc = GetDocument();
	CxImage*  ima  = pDoc->GetImage();
	if (!ima)	return;

	// We'll get the RGB values at the point the user selects
	long x = point.x;
	long y = point.y;
	GetImageXY(pDoc, ima, &x,&y);

	char s[80];
	if (ima->IsInside(x,y))	{

		long yflip = ima->GetHeight() - y - 1;
		sprintf(s,"x: %d y: %d  idx: %d", x, y, ima->GetPixelIndex(x,yflip));
		RGBQUAD rgb=ima->GetPixelColor(x,yflip);

		if (ima->HasAlpha()) rgb.rgbReserved=ima->AlphaGet(x,yflip);
		else rgb.rgbReserved=ima->GetPaletteColor(ima->GetPixelIndex(x,yflip)).rgbReserved;

		//显示HSI;
		double m_dblHSIH=0, m_dblHSIS=0, m_dblHSII=0;
		RGBtoHSI(rgb.rgbRed, rgb.rgbGreen, rgb.rgbBlue, m_dblHSIH, m_dblHSIS, m_dblHSII);

		sprintf(&s[strlen(s)],"  RGBA: (%d, %d, %d, %d)   HSI:(%.1f,%.1f,%.1f)", rgb.rgbRed, rgb.rgbGreen, rgb.rgbBlue, rgb.rgbReserved, m_dblHSIH, m_dblHSIS, m_dblHSII);

		if (nFlags & MK_LBUTTON)
			switch (pDoc->m_tool){
			case 0:
				SetCursor(LoadCursor(0,IDC_SIZEALL));
				SetScrollPos(SB_HORZ,m_RefScroll.x - point.x + m_RefPoint.x);
				SetScrollPos(SB_VERT,m_RefScroll.y - point.y + m_RefPoint.y);
				Invalidate(0);
				break;
			case 1:	//selection
				SetCursor(LoadCursor(0,IDC_CROSS));
#if CXIMAGE_DEMO_SELECT
				CDemoDoc* pDoc = GetDocument();
				float zoom=pDoc->GetZoomFactor();
				CPoint pos(GetScrollPosition());
				int i=pDoc->m_NumSel;
				pDoc->m_Sel[i].x = (long)((point.x + pos.x)/zoom);
				pDoc->m_Sel[i].y = (long)((point.y + pos.y)/zoom);
				if (i<(MAX_SEL_POINTS-2)) pDoc->m_NumSel++;
				DrawSelection();
#endif
				break;
		}

	} else strcpy(s," ");
	
	CStatusBar& statusBar = ((CMainFrame *)(AfxGetApp()->m_pMainWnd))->GetStatusBar();
	statusBar.SetPaneText(0, s);
	
	CScrollView::OnMouseMove(nFlags, point);
}

//////////////////////////////////////////////////////////////////////////////
BOOL CDemoView::OnEraseBkgnd(CDC* pDC) 
{
	return 1;
}

//////////////////////////////////////////////////////////////////////////////
void CDemoView::OnSetFocus(CWnd* pOldWnd) 
{
	CScrollView::OnSetFocus(pOldWnd);
	
	CDemoDoc* pDoc = GetDocument();
	if (pDoc) pDoc->UpdateStatusBar();
}

//////////////////////////////////////////////////////////////////////////////
void CDemoView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CDemoDoc* pDoc = GetDocument();
	CxImage* img = pDoc->GetImage();

// for test Flood Fill
	BOOL m_bTestFloodFill = TRUE;
	if(m_bTestFloodFill == TRUE)
	{
		long x = point.x;
		long y = point.y;
		GetImageXY(pDoc, img, &x,&y);	
	}	

	// Other part of orginal source	
	if (pDoc) {
		switch(pDoc->m_tool){
		case 0:
			{
				CxImage* ima = pDoc->GetImage();
				if (ima) {
					m_RefScroll = GetScrollPosition();
					m_RefPoint.x = point.x;
					m_RefPoint.y = point.y;
				}
			}
			break;
		case 2:
			if (!pDoc->GetWaitingClick()) PostMessage(WM_COMMAND,ID_VIEW_ZOOMIN);
			break;
		}

		if (pDoc->GetWaitingClick()){
			pDoc->SetWaitingClick(0);
			CxImage* ima = pDoc->GetImage();
			if (ima) {
				long x = point.x;
				long y = point.y;
				GetImageXY(pDoc, ima, &x,&y);
				if (ima->IsInside(x,y))	{
					pDoc->SubmitUndo();
					long yflip = ima->GetHeight() - y - 1;
					ima->SetTransIndex(ima->GetPixelIndex(x,yflip));
					// <DP> RGB transparency
					ima->SetTransColor(ima->GetPixelColor(x,yflip));
					pDoc->UpdateAllViews(NULL);
				}
			}
		}
#if CXIMAGE_DEMO_SELECT
		 else {
			KillTimer(1);
			pDoc->m_NumSel=0;
			m_SelShift=0;
		}
#endif
	}

	CScrollView::OnLButtonDown(nFlags, point);
}

//////////////////////////////////////////////////////////////////////////////
void CDemoView::OnLButtonUp(UINT nFlags, CPoint point) 
{
#if CXIMAGE_DEMO_SELECT
	CDemoDoc* pDoc = GetDocument();
	if (pDoc){
		CxImage* ima=pDoc->GetImage();
		if (ima){
			int i=pDoc->m_NumSel;
			if (i>2){
				pDoc->m_Sel[i].x = pDoc->m_Sel[0].x;
				pDoc->m_Sel[i].y = pDoc->m_Sel[0].y;
				pDoc->m_NumSel++;
				SetTimer(1,333,NULL);
				
				long i;
				long n=pDoc->m_NumSel;
				POINT* p = (POINT*)malloc(n*sizeof(POINT));
				long h=ima->GetHeight();
				for(i=0;i<n;i++){
					p[i].x = pDoc->m_Sel[i].x;
					p[i].y = h-1-pDoc->m_Sel[i].y;
				}
				ima->SelectionClear();
				ima->SelectionAddPolygon(p,n);
				free(p);
			} else {
				ima->SelectionDelete();
			}
			Invalidate(0);
		}
	}
#endif

	
	CScrollView::OnLButtonUp(nFlags, point);
}

//////////////////////////////////////////////////////////////////////////////
void CDemoView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	switch (lHint)
	{
	case WM_USER_NEWIMAGE:
		{
			CDemoDoc* pDoc = GetDocument();
			CxImage*  ima  = pDoc->GetImage();
			int px=GetScrollPos(SB_HORZ);
			int py=GetScrollPos(SB_VERT);
			CSize sz(GetTotalSize());
			int x=(int)(ima->GetWidth()*pDoc->GetZoomFactor());
			int y=(int)(ima->GetHeight()*pDoc->GetZoomFactor());
			SetScrollSizes(MM_TEXT,	CSize(x,y));
			CSize sz2(GetTotalSize());
			if (sz.cx!=0 && sz.cy!=0){
				SetScrollPos(SB_HORZ,sz2.cx*px/sz.cx);
				SetScrollPos(SB_VERT,sz2.cy*py/sz.cy);
			}
			CWnd* pFrame=GetParentFrame();
			if (!(pFrame->IsIconic()||pFrame->IsZoomed())){
				RECT rClient,rMainCl,rFrame,rMainFr;
				((CMainFrame *)(AfxGetApp()->m_pMainWnd))->GetClientRect(&rMainCl);
				((CMainFrame *)(AfxGetApp()->m_pMainWnd))->GetWindowRect(&rMainFr);
				pFrame->GetClientRect(&rClient);
				pFrame->GetWindowRect(&rFrame);
				pFrame->SetWindowPos(0,0,0,
					(4+rFrame.right-rFrame.left-rClient.right+rClient.left)+
					min(rMainCl.right-(rFrame.left-rMainFr.left+12),x),
					(4+rFrame.bottom-rFrame.top-rClient.bottom+rClient.top)+
					min(rMainCl.bottom-(rFrame.top-rMainFr.top+12),y),
					SWP_NOMOVE|SWP_NOZORDER);
				//ResizeParentToFit(1);
			}
			if (!ima->HasSelection()) KillTimer(1);

			CMainFrame* pMain = (CMainFrame*) AfxGetMainWnd();
			if (pMain->m_HistoBar.IsWindowVisible()){
				pDoc->m_hmax=ima->Histogram(pDoc->m_hr,pDoc->m_hg,pDoc->m_hb,pDoc->m_hgray);
				pMain->m_HistoBar.Invalidate();
			}

			break;
		}
	default:
		{
			CDemoDoc* pDoc = GetDocument();
			if (pDoc){
				CxImage*  ima  = pDoc->GetImage();
				if (ima){
					if (pDoc->GetStretchMode()) SetScrollSizes(MM_TEXT,	CSize(0,0));
					else SetScrollSizes(MM_TEXT,CSize((int)(ima->GetWidth()*pDoc->GetZoomFactor()),
													  (int)(ima->GetHeight()*pDoc->GetZoomFactor())));
				}
			}
		}
	}
	CScrollView::OnUpdate(pSender, lHint, pHint);
}

//////////////////////////////////////////////////////////////////////////////
LRESULT CDemoView::OnNewImage(WPARAM wParam, LPARAM lParam)
{
	CDemoDoc* pDoc = GetDocument();
	pDoc->UpdateAllViews(0,WM_USER_NEWIMAGE);
	pDoc->UpdateStatusBar();
	return 0;
}

//////////////////////////////////////////////////////////////////////////////
LRESULT CDemoView::OnNewProgress(WPARAM wParam, LPARAM lParam)
{
	CStatusBar& statusBar = ((CMainFrame *)(AfxGetApp()->m_pMainWnd))->GetStatusBar();
	CString s;
	s.Format("%d %%",(int)wParam);
	statusBar.SetPaneText(2, s);
	statusBar.Invalidate(1);
	return 0;
}

//////////////////////////////////////////////////////////////////////////////
#if CXIMAGE_DEMO_SELECT
void CALLBACK LineDDAProc(int x, int y, CDemoView *pview)
{
	CDC* pDC = pview->GetDC();
	if (pDC) {
		pview->m_SelCount++;
		if (pview->m_SelCount <= 4)	pDC->SetPixelV(x,y,RGB(255,255,255));
		else pDC->SetPixelV(x,y,RGB(0,0,0));
		if (pview->m_SelCount == 8) pview->m_SelCount=0;
	}
}

//////////////////////////////////////////////////////////////////////////////
void CDemoView::DrawSelection()
{
	CDemoDoc* pDoc = GetDocument();
	if (pDoc->m_NumSel>2){
		CPoint pos(GetScrollPosition());
		float zoom=pDoc->GetZoomFactor();
		m_SelCount=m_SelShift;
		for(int i=1;i<pDoc->m_NumSel;i++){
			LineDDA((int)(zoom*pDoc->m_Sel[i-1].x - pos.x),
				    (int)(zoom*pDoc->m_Sel[i-1].y - pos.y),
					(int)(zoom*pDoc->m_Sel[i].x - pos.x),
					(int)(zoom*pDoc->m_Sel[i].y - pos.y),
					(LINEDDAPROC)LineDDAProc,(LPARAM)this);
		}
	}
}
#endif

//////////////////////////////////////////////////////////////////////////////
void CDemoView::OnDestroy() 
{
	CScrollView::OnDestroy();
	((CMainFrame*)AfxGetMainWnd())->m_HistoBar.Invalidate();
#if CXIMAGE_DEMO_SELECT
	KillTimer(1);
#endif
}

//////////////////////////////////////////////////////////////////////////////
void CDemoView::OnTimer(UINT nIDEvent) 
{
#if CXIMAGE_DEMO_SELECT
	m_SelShift++;
	if (m_SelShift==8) m_SelShift=0;
	DrawSelection();
#endif
	CScrollView::OnTimer(nIDEvent);
}

//////////////////////////////////////////////////////////////////////////////
void CDemoView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (nChar == VK_ESCAPE){
		CMainFrame* pMain = (CMainFrame*) AfxGetMainWnd();
		if (pMain->m_fullscreen){
			pMain->PostMessage(WM_COMMAND, ID_VIEW_FULLSCREEN);
		}
	}
	
	CScrollView::OnChar(nChar, nRepCnt, nFlags);
}

//////////////////////////////////////////////////////////////////////////////
void CDemoView::OnRButtonDown(UINT nFlags, CPoint point) 
{

	CDemoDoc* pDoc = GetDocument();
	
	//记录当前鼠标位置
	CxImage* ima=pDoc->image;
	nHeight = ima->GetHeight();
	nWidth = ima->GetWidth();
	//记录当前鼠标位置end

	if (pDoc) {
		switch(pDoc->m_tool){
		case 2:
			if (!pDoc->GetWaitingClick()) PostMessage(WM_COMMAND,ID_VIEW_ZOOMOUT);
			return;
		}
	}
	
	CScrollView::OnRButtonDown(nFlags, point);
}

//////////////////////////////////////////////////////////////////////////////
void CDemoView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	if (bActivate)
	{
		((CMainFrame*)AfxGetMainWnd())->m_HistoBar.Invalidate();
	}
	
	CScrollView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void CDemoView::OnImgpointComputeHtConcenteric() 
{
	//计算物体坐标系下的真实相机参数
	bool object = ComputeObjectcamera();

	/*
	bool light = ComputeLightcamera();
	
	if(object && light)
	{
		int latitude, langitude;
		//////////////////////////
		//计算光源在物体坐标系中的坐标
		double light[3];

		light[0]=light_x-m_HP_light.Xs;
		light[1]=light_y-m_HP_light.Ys;
		light[2]=light_z-m_HP_light.Zs;
		
		double matrix_object[9];
		double matrix_light[9];

		matrix_object[0]=m_HP_object.r11;
		matrix_object[1]=m_HP_object.r12;
		matrix_object[2]=m_HP_object.r13;
		matrix_object[3]=m_HP_object.r21;
		matrix_object[4]=m_HP_object.r22;
		matrix_object[5]=m_HP_object.r23;
		matrix_object[6]=m_HP_object.r31;
		matrix_object[7]=m_HP_object.r32;
		matrix_object[8]=m_HP_object.r33;

		matrix_light[0]=m_HP_light.r11;
		matrix_light[1]=m_HP_light.r12;
		matrix_light[2]=m_HP_light.r13;
		matrix_light[3]=m_HP_light.r21;
		matrix_light[4]=m_HP_light.r22;
		matrix_light[5]=m_HP_light.r23;
		matrix_light[6]=m_HP_light.r31;
		matrix_light[7]=m_HP_light.r32;
		matrix_light[8]=m_HP_light.r33;

		double res1[3],res2[3];
		invers_matrix(matrix_object,3);
		mult(matrix_light,light,res1,3,3,1);
		mult(matrix_object,res1,res2,3,3,1);

		light[0]=res2[0]+m_HP_object.Xs;
		light[1]=res2[1]+m_HP_object.Ys;
		light[2]=res2[2]+m_HP_object.Zs;

		//计算光源的经纬度
		double r=sqrt(light[0]*light[0]+light[1]*light[1]+light[2]*light[2]);
		double r2=sqrt(light[0]*light[0]+light[2]*light[2]);
		latitude=acos(light[1]/r)*180/PI;
		langitude=acos(light[2]/r2)*180/PI;

		if(light[0]<=0)
		{
			langitude*=-1;
			langitude+=360;
		}
		//////////////////////////
		//保存文件
		CDemoDoc* pDoc = GetDocument();
		CString strFileSpt=pDoc->m_strFileName.Left(pDoc->m_strFileName.GetLength()-4) +".bmp.spt";
		FILE* fp=fopen(strFileSpt,"w");
		if(fp==NULL)
			return;
		
		fprintf(fp,"%f\n", m_HP_object.f);
		fprintf(fp,"%f\n", m_HP_object.k);

		//for photogrammetry :
		fprintf(fp,"%d %d\n", m_HP_object.wid, m_HP_object.hei);
		fprintf(fp,"%f %f %f\n", m_HP_object.Xs, m_HP_object.Ys, m_HP_object.Zs);
		fprintf(fp,"%f %f %f\n", m_HP_object.faita, m_HP_object.omiga, m_HP_object.kapa);
		fprintf(fp,"%d %d\n", latitude,langitude);

		//for computer vision :
		fprintf(fp," %1.10lf  %1.10lf  %1.10lf  \n ",m_HP_object.r11,m_HP_object.r12,m_HP_object.r13);
		fprintf(fp," %1.10lf  %1.10lf  %1.10lf  \n ",m_HP_object.r21,m_HP_object.r22,m_HP_object.r23);
		fprintf(fp," %1.10lf  %1.10lf  %1.10lf  \n ",m_HP_object.r31,m_HP_object.r32,m_HP_object.r33);
		fprintf(fp," %f %f %f\n",m_HP_object.tx,m_HP_object.ty,m_HP_object.tz);
		fprintf(fp,"%lf %lf %lf %lf\n",m_HP_object.P[0][0],m_HP_object.P[0][1],m_HP_object.P[0][2],m_HP_object.P[0][3]);
		fprintf(fp,"%lf %lf %lf %lf\n",m_HP_object.P[1][0],m_HP_object.P[1][1],m_HP_object.P[1][2],m_HP_object.P[1][3]);
		fprintf(fp,"%lf %lf %lf %lf\n",m_HP_object.P[2][0],m_HP_object.P[2][1],m_HP_object.P[2][2],m_HP_object.P[2][3]);

		fclose(fp);

		CString strMsg;
		strMsg="物体坐标系：相机位置保存为"+strFileSpt+"文件(包括光源定位)";
		AfxMessageBox(strMsg);
		///////////////////////////////////////////
	}
	*/
}


//输入空间点坐标，计算对应的图像点
void CDemoView::OnCalibrationInputgrdpos() 
{
	// TODO: Add your command handler code here
	if(m_HP.tz == INT_MAX)
	{
		AfxMessageBox("还没有导入相机参数");
		return;
	}

	// TODO: Add your command handler code here
	CDlgGrdPts dlg;
	dlg.m_X=0;
	dlg.m_Y=0;
	dlg.m_Z=0;
		
	if(dlg.DoModal()==IDOK)
	{
		float X=0,Y=0,Z=0;
		
		X=dlg.m_X; 
		Y=dlg.m_Y; 
		Z=dlg.m_Z; 

		POINT3D grd;
		grd.x=X;
		grd.y=Y;
		grd.z=Z;
		POINT2D img;

	
		GrdToImg1(grd,img,m_HP);
		m_pt.x=(long)img.x;
		m_pt.y=(long)img.y;
	
		Invalidate();
	}
	
}

//导入相机参数
void CDemoView::OnCalibrationLoadcampara() 
{
	char strFilter[]="(*.spt)|*.spt||";
	CFileDialog fDlg(1,"spt","*.spt",OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,strFilter);

	if(fDlg.DoModal()==IDOK)
	{
		CString sf=fDlg.GetPathName();
		FILE *fp=fopen(sf,"r");

		fscanf(fp,"%lf",&m_HP.f);
		fscanf(fp,"%lf",&m_HP.k);

		fscanf(fp,"%d %d",&m_HP.wid,&m_HP.hei);
		fscanf(fp,"%lf %lf %lf",&m_HP.Xs,   &m_HP.Ys,   &m_HP.Zs);
		fscanf(fp,"%lf %lf %lf",&m_HP.faita,&m_HP.omiga,&m_HP.kapa);
		
		///////////////////////////////
		fscanf(fp,"%lf",&m_HP.r11);
		fscanf(fp,"%lf",&m_HP.r12);
		fscanf(fp,"%lf",&m_HP.r13);
			
		fscanf(fp,"%lf",&m_HP.r21);
		fscanf(fp,"%lf",&m_HP.r22);
		fscanf(fp,"%lf",&m_HP.r23);
			
		fscanf(fp,"%lf",&m_HP.r31);
		fscanf(fp,"%lf",&m_HP.r32);
		fscanf(fp,"%lf",&m_HP.r33);
			
		fscanf(fp,"%lf",&m_HP.tx);
		fscanf(fp,"%lf",&m_HP.ty);
		fscanf(fp,"%lf",&m_HP.tz);
			
		fscanf(fp,"%lf %lf %lf %lf",&m_HP.P[0][0],&m_HP.P[0][1],&m_HP.P[0][2],&m_HP.P[0][3]);
		fscanf(fp,"%lf %lf %lf %lf",&m_HP.P[1][0],&m_HP.P[1][1],&m_HP.P[1][2],&m_HP.P[1][3]);
		fscanf(fp,"%lf %lf %lf %lf",&m_HP.P[2][0],&m_HP.P[2][1],&m_HP.P[2][2],&m_HP.P[2][3]);
		
		////////////////////////////////
		fclose(fp);
	}	
	
}

//重新调整图像大小.采用双线性插值技术
void CDemoView::OnCalibImgReSize() 
{
	// TODO: Add your command handler code here
	CDemoDoc* pDoc = GetDocument();
	CxImage*  ima  = pDoc->GetImage();
	
	pDoc->Stopwatch(0);
	
	
	CDlgCalibImgReSize dlgImgReSize;
	dlgImgReSize.m_nHeight = nHeight;
	dlgImgReSize.m_nWidth = nWidth;

	if(dlgImgReSize.DoModal()==IDOK)
	{
		int m_nNewHeight = dlgImgReSize.m_nHeight;
		int m_nNewWidth = dlgImgReSize.m_nWidth;
		
		if((m_nNewHeight==nHeight)&&(m_nNewWidth==nWidth)) return;
		double sx,sy;				//x和y方向的比例系数
		sx=double(m_nNewWidth)/(double)nWidth;
		sy=double(m_nNewHeight)/(double)nHeight;

		CxImage* imaNew = new CxImage(m_nNewWidth, m_nNewHeight,24);

		int i,j;

		for(i=0;i<m_nNewWidth;i++)
		{
			for(j=0;j<m_nNewHeight;j++)
			{
				double x,y;
				x=(double)i/sx;
				y=(double)j/sy;
				if(x<=0)x=0;
				if(x>=nWidth-1)x=nWidth-1;
				if(y<=0)y=0;
				if(y>=nHeight-1)y=nHeight-1;
				if( ((int)x==0)||((int)x==nWidth-1)||((int)y==0)||((int)y==nHeight-1) )
				{
					COLORREF color=ima->RGBQUADtoRGB(ima->GetPixelColor((int)x,(int)y));
					imaNew->SetPixelColor((int)i,(int)j,color);
					
				}
				else
				{
					double r1,r2,r3,r4;
					double g1,g2,g3,g4;
					double b1,b2,b3,b4;
					COLORREF c1,c2,c3,c4;
					c1=ima->RGBQUADtoRGB(ima->GetPixelColor((int)x,(int)y));
					c2=ima->RGBQUADtoRGB(ima->GetPixelColor((int)x+1,(int)y));
					c3=ima->RGBQUADtoRGB(ima->GetPixelColor((int)x,(int)y+1));
					c4=ima->RGBQUADtoRGB(ima->GetPixelColor((int)x+1,(int)y+1));
					double dis1=x-(int)x;
					double dis2=1-x+(int)x;
					double dis3=y-(int)y;
					double dis4=1-y+(int)y;
					r1=GetRValue(c1);
					g1=GetGValue(c1);
					b1=GetBValue(c1);
					r2=GetRValue(c2);
					g2=GetGValue(c2);
					b2=GetBValue(c2);
					r3=GetRValue(c3);
					g3=GetGValue(c3);
					b3=GetBValue(c3);
					r4=GetRValue(c4);
					g4=GetGValue(c4);
					b4=GetBValue(c4);

					int r=int(dis4*(r1*dis2+r2*dis1)+dis3*(r3*dis2+r4*dis1));
					if(r>255)r=255;
					int g=int(dis4*(g1*dis2+g2*dis1)+dis3*(g3*dis2+g4*dis1));
					if(g>255)g=255;
					int b=int(dis4*(b1*dis2+b2*dis1)+dis3*(b3*dis2+b4*dis1));
					if(b>255)b=255;

					imaNew->SetPixelColor(i,j,RGB((BYTE)r,(BYTE)g,(BYTE)b));
			
				}
			}
		}
	
		delete pDoc->image;
		pDoc->image = imaNew;
		
		Invalidate();
		pDoc->Stopwatch(1);
		pDoc->UpdateStatusBar();
		pDoc->UpdateAllViews(NULL);

	}
	
}


//------------------------------
//by mu_yadong
//first written:2004-2-23
//last modifed:2004-4-1
//description:judge a pixel whether belongs to  pure color
//parameter:
//x,y	--	屏幕上点的坐标
//------------------------------
int CDemoView::IsPureColor(CxImage* img,int x,int y,int tolerance) 
{
	COLORREF c;
	int nHei=img->GetHeight();
	c = img->RGBQUADtoRGB(img->GetPixelColor(x,nHei - 1 - y));
	int red = GetRValue(c);
	int green = GetGValue(c);
	int blue = GetBValue(c);
	
	double m_dblHSIH=0, m_dblHSIS=0, m_dblHSII=0;
	int  H = (int )RGBtoHSI(red, green, blue, m_dblHSIH, m_dblHSIS, m_dblHSII);
	//double tolerance = 30.0;

	//if(m_dblHSIS>0.15)
	{
		if(	H >= (180 - tolerance) && H <= (180+tolerance)	) return YELLOW;
		if(	H >= (300 - tolerance) && H <= (300+tolerance)	) return CYAN;
		if(	H >= ( 60 - tolerance) && H <= ( 60+tolerance)	) return MAGENTA;
		if(	H >= (120 - tolerance) && H <= (120+tolerance)	) return RED;
		if(	H >= (240 - tolerance) && H <= (240+tolerance)	) return GREEN;
		if(	H >= (360 - tolerance) && H <= (360+tolerance)	) return BLUE;
	}
	
	return NOT_PURE_COLOR;
}
double CDemoView::RGBmax(double r,double g,double b)
{
	double max;
	max = r;
	if(g > max) max = g;
	if(b > max) max = b;
	return max;
}
double CDemoView::RGBmin(double r,double g,double b)
{
	double min;
	min = r;
	if(g < min) min = g;
	if(b < min) min = b;
	return min;
}
double CDemoView::RGBtoHSI(int red,int green,int blue,double &m_dblHSIH, double &m_dblHSIS,double &m_dblHSII)
{
	double B,G,R,M,m,H,S,I,r,g,b;
	B=blue/255.0; 
	G=green/255.0; 
	R=red/255.0; 
	M=RGBmax(R,G,B); 
	m=RGBmin(R,G,B); 
	if((M-m)==0.0) 
	{ 
		r=g=b=0.0; 
	} 
	else 
	{ 
		r = (M - R) / (M - m); 
		g = (M - G) / (M - m); 
		b = (M - B) / (M - m); 
	} 
	
	//光强的范围为0到1.0，计算光强的公式为： 
	I=(M+m)/2; 

	//饱和度的范围为0到1.0，计算饱和度的公式为： 
	if(M==m)S=0.0; 
	else 
	{ 
		if(I<=0.5)S = (M - m) / (M + m); 
		else S = (M - m) / (2 - M - m); 
	} 

	//色调的范围为0到360，计算色调的公式为： 
	if(M ==m) H = 0; 
	else if(R == M) H = 60 * (2 + b - g); 
	else if(G == M) H = 60 * (4 + r - b); 
	else if(B == M) H = 60 * (6 + g - r); 

	m_dblHSIH = H;
	m_dblHSIS = S;
	m_dblHSII = I;

	return H;
}


void CDemoView::OnConcentricCenter() 
{
	//得到要处理的图片的指针
	CDemoDoc* pDoc = GetDocument();
	CxImage* img = pDoc->GetImage();
	pDoc->duplicate_image->Copy(*img);

	//每次运算之前都要重新初始化堆栈
	for(int k = 0;k < 100000;k ++)
	{
		myList[k] = 0;
		myList_light[k] = 0;
	}
	myListPointer = 0;
	myListPointer_light = 0;

	//通过对话框，设定要用的的参数
	CCircleParaDlg paraDlg;

	if( paraDlg.DoModal() == IDOK)
	{
		m_red = paraDlg.m_red;
		m_green = paraDlg.m_green;
		m_blue = paraDlg.m_blue;
		m_ratio = paraDlg.m_ratio;
		m_hue = paraDlg.m_hue;
		m_x = paraDlg.m_x;
		m_y = paraDlg.m_y;
		m_jump = paraDlg.m_jump;
		m_rectangle = paraDlg.m_rectangle;
		
		m_ratio_light=paraDlg.m_light_ratio;
		light_x=paraDlg.m_light_x;
		light_y=paraDlg.m_light_y;
		light_z=paraDlg.m_light_z;
		
		m_s=paraDlg.m_s;
	}
	else
	{
		return ;            //没有设定的话，直接返回// by fxf
		m_red = 30;
		m_green = 30;
		m_blue = 30;
		m_ratio = 2;
		m_hue = 20;
		m_x = 30;
		m_y = 30;
		m_jump = 8;
		m_rectangle = 2.0;
		m_s = 0;
	}

	int nWid=img->GetWidth();
	int nHei=img->GetHeight();
	
	//初始化bmark数组，标识象素是否已经被扫描
	bool *bMark = new bool[nWid * nHei];
	for(long cc = 0; cc < nWid * nHei; cc ++)
		bMark[cc] = false;
		
	//逐点扫描
	int outerPixelCount;	//记录外圆点数
	int innerPixelCount;	//记录内圆点数
	
	//保存内圆的圆心
	long circle_x, circle_y;	

	//保存外圆的圆心
	int w_x,w_y;

	//temptemp保存找到的同心圆的个数
	int circle_number_object = 0;
	int circle_number_light = 0;

	//保存外圆和内圆的颜色
	int color_out,color_in;

	//保存外圆和内圆的范围
	int outerMinX,outerMinY,outerMaxX,outerMaxY;
	int innerMinX,innerMinY,innerMaxX,innerMaxY;

	for(int y = 0; y < nHei;y ++)
		for(int x = 0; x < nWid;x ++)
		{
			//判断此象素点是否已被访问过
			if(bMark[y * nWid + x]) continue;
			
			//判断此象素点是否为纯色
			color_out = IsPureColor(img,x,nHei-1-y,m_hue);
			if(color_out == NOT_PURE_COLOR) 
			{ bMark[y * nWid + x] = true;continue;}
			
			//先跳跃填充
			if(y + m_jump < nHei && ! bMark[(y + m_jump) * nWid + x]
				&& IsPureColor(img,x,nHei-1-y-m_jump,m_hue) == color_out)
			{
				pDoc->FloodFill(CPoint(x,y+m_jump),img,bMark,m_red,m_green,m_blue);
				
				if( ! bMark[y * nWid + x] ) 
					pDoc->UnFloodFill(img,bMark);
			}

			//如果跳跃填充不管用，就顺序填充。
			if( ! bMark[y * nWid + x] )
			{			
				pDoc->FloodFill(CPoint(x,y),img,bMark,m_red,m_green,m_blue);				
			}
			
			//根据填充后的数据，可以得到外圆的信息
			outerPixelCount = pDoc->pixelCount;
			w_x = pDoc->circle_x/outerPixelCount;
			w_y = pDoc->circle_y/outerPixelCount;
			outerMinX = pDoc->min_x;
			outerMinY = pDoc->min_y;
			outerMaxX = pDoc->max_x;
			outerMaxY = pDoc->max_y;
			
			//如果外圆点数不足，则舍弃；
			if(outerPixelCount < 200 ) continue;

			//如果外圆的圆心被访问过，则也舍弃；
			if( bMark[w_y * nWid + w_x]) continue;

			int m;
			/////////////////////////////////////////////////////
			/*	for(int m=0;m<pDoc->ptPointer;)
				{
					int x=pDoc->ptStack[m++];
					int y=pDoc->ptStack[m++];
					pDoc->duplicate_image->SetPixelColor(x,y,RGB(0,0,0));
				}*/
			////////////////////////////////////////////////////////

			//获取圆心的颜色，如果不是纯色，还是舍弃；
			color_in = IsPureColor(img,w_x,nHei-1-w_y,m_hue);
			if(color_in == NOT_PURE_COLOR) continue;
			
			//填充内圆
			pDoc->FloodFill(CPoint(w_x,w_y),img,bMark,m_red,m_green,m_blue);
			
			//根据填充后的数据，可以得到内圆的信息
			innerPixelCount = pDoc->pixelCount;
			circle_x = pDoc->circle_x/innerPixelCount;
			circle_y = pDoc->circle_y/innerPixelCount;
			innerMinX = pDoc->min_x;
			innerMinY = pDoc->min_y;
			innerMaxX = pDoc->max_x;
			innerMaxY = pDoc->max_y;
			
			//如果内圆点数不足，则舍弃；
			if(innerPixelCount < 50 ) continue;

			/////////////////////////////////////////////////////
				for(m=0;m<pDoc->ptPointer;)
				{
					int x=pDoc->ptStack[m++];
					int y=pDoc->ptStack[m++];
					pDoc->duplicate_image->SetPixelColor(x,y,RGB(0,0,0));
				}
			////////////////////////////////////////////////////////

			//如果内外圆的圆心距离大于阈值，则舍弃，否则继续；
			//如果内外圆半径比例不符合要求，则舍弃，否则继续；
			//如果内外圆的极值不符合要求，则舍弃，否则继续；			
			//如果内外圆的颜色相同，则舍弃，否则继续；			
			if(abs(w_x - circle_x) < m_x && abs(w_y - circle_y) < m_y 
				&& float(outerPixelCount)/float(innerPixelCount) >  (m_ratio * m_ratio -1 -2) 
				&& float(outerPixelCount)/float(innerPixelCount) <  (m_ratio * m_ratio -1 +2) 
				&& innerMinX > outerMinX && innerMinX < outerMaxX
				&& innerMaxX > outerMinX && innerMaxX < outerMaxX
				&& innerMinY > outerMinY && innerMinY < outerMaxY
				&& innerMaxY > outerMinY && innerMaxY < outerMaxY
				&& m_rectangle* (innerMinY-outerMinY) >= 1 * (outerMaxY-innerMaxY) 
				&& 1 * (innerMinY-outerMinY) <= (outerMaxY-innerMaxY) * m_rectangle
				&& m_rectangle* (innerMinX-outerMinX) >= 1 * (outerMaxX-innerMaxX)
				&& 1 * (innerMinX-outerMinX) <= (outerMaxX-innerMaxX) * m_rectangle
				&& color_out != color_in
				)
			{	
				int i,count1,count2;
				
				count1 = count2 = 0;
				for(i=circle_x; i<=outerMaxX; i++)
				{
					if( IsPureColor(img,i,nHei-1-circle_y,m_hue) == color_out)
						count1 ++;
				}
				for(i = circle_x;i >= outerMinX;i --)
				{
					if( IsPureColor(img,i,nHei-1-circle_y,m_hue) == color_out)
						count2 ++;
				}
				
				if(count1 == 0 || count2 == 0 ||  4*fabs(count1-count2) > count1+count2)
				{
					pDoc->UnFloodFill(img,bMark);
					continue;
				}
				
				count1 = count2 = 0;
				for(i = circle_y;i <= outerMaxY;i ++)
				{
					if( IsPureColor(img,circle_x,nHei-1-i,m_hue) == color_out)
						count1 ++;
				}
				for(i = circle_y;i >= outerMinY;i --)
				{
					if( IsPureColor(img,circle_x,nHei-1-i,m_hue) == color_out)
						count2 ++;
				}

				if(count1 == 0 || count2 == 0 ||  4*fabs(count1-count2) > count1+count2)
				{
					pDoc->UnFloodFill(img,bMark);
					continue;
				}

				//记录此同心圆的位置信息
				circle_number_object ++;
				circle_x = (circle_x + w_x)/2;
				circle_y = (circle_y + w_y)/2;
				
				//在图像上做标记
				pDoc->duplicate_image->SetPixelColor(circle_x,circle_y,RGB(255,255,255));
				pDoc->duplicate_image->SetPixelColor(circle_x+1,circle_y+1,RGB(255,255,255));
				pDoc->duplicate_image->SetPixelColor(circle_x+1,circle_y-1,RGB(255,255,255));
				pDoc->duplicate_image->SetPixelColor(circle_x-1,circle_y+1,RGB(255,255,255));
				pDoc->duplicate_image->SetPixelColor(circle_x-1,circle_y-1,RGB(255,255,255));
				
				myList[myListPointer ++] = circle_x;
				myList[myListPointer ++] = nHei-1-circle_y;
				myList[myListPointer ++] = color_out;
				myList[myListPointer ++] = color_in;
			}
			else if(abs(w_x - circle_x) < m_x && abs(w_y - circle_y) < m_y 
				&& float(outerPixelCount)/float(innerPixelCount) >  (m_ratio_light * m_ratio_light -1 -3) 
				&& float(outerPixelCount)/float(innerPixelCount) <  (m_ratio_light * m_ratio_light -1 +4) 
				&& innerMinX > outerMinX && innerMinX < outerMaxX
				&& innerMaxX > outerMinX && innerMaxX < outerMaxX
				&& innerMinY > outerMinY && innerMinY < outerMaxY
				&& innerMaxY > outerMinY && innerMaxY < outerMaxY
				&& m_rectangle* (innerMinY-outerMinY) >= 1 * (outerMaxY-innerMaxY) 
				&& 1 * (innerMinY-outerMinY) <= (outerMaxY-innerMaxY) * m_rectangle
				&& m_rectangle* (innerMinX-outerMinX) >= 1 * (outerMaxX-innerMaxX)
				&& 1 * (innerMinX-outerMinX) <= (outerMaxX-innerMaxX) * m_rectangle
				&& color_out != color_in
				)
			{	
				int i,count1,count2;

				count1 = count2 = 0;
				for(i=circle_x; i<=outerMaxX; i++)
				{
					if( IsPureColor(img,i,nHei-1-circle_y,m_hue) == color_out)
						count1 ++;
				}
				for(i = circle_x;i >= outerMinX;i --)
				{
					if( IsPureColor(img,i,nHei-1-circle_y,m_hue) == color_out)
						count2 ++;
				}

				if(count1 == 0 || count2 == 0 ||  4*fabs(count1-count2) > count1+count2
					)
				{
					pDoc->UnFloodFill(img,bMark);
					continue;
				}

				count1 = count2 = 0;
				for(i = circle_y;i <= outerMaxY;i ++)
				{
					if( IsPureColor(img,circle_x,nHei-1-i,m_hue) == color_out)
						count1 ++;
				}
				for(i = circle_y;i >= outerMinY;i --)
				{
					if( IsPureColor(img,circle_x,nHei-1-i,m_hue) == color_out)
						count2 ++;
				}

				if(count1 == 0 || count2 == 0 ||  4*fabs(count1-count2) > count1+count2)
				{
					pDoc->UnFloodFill(img,bMark);
					continue;
				}

				//记录此同心圆的位置信息
				circle_number_light++;
				circle_x = (circle_x + w_x)/2;
				circle_y = (circle_y + w_y)/2;
				
				//在图像上做标记
				pDoc->duplicate_image->SetPixelColor(circle_x,circle_y,RGB(255,255,255));
				pDoc->duplicate_image->SetPixelColor(circle_x+1,circle_y+1,RGB(255,255,255));
				pDoc->duplicate_image->SetPixelColor(circle_x+1,circle_y-1,RGB(255,255,255));
				pDoc->duplicate_image->SetPixelColor(circle_x-1,circle_y+1,RGB(255,255,255));
				pDoc->duplicate_image->SetPixelColor(circle_x-1,circle_y-1,RGB(255,255,255));
				
				myList_light[myListPointer_light ++] = circle_x;
				myList_light[myListPointer_light ++] = nHei-1-circle_y;
				myList_light[myListPointer_light ++] = color_out;
				myList_light[myListPointer_light ++] = color_in;				
			}
			else
			{
				pDoc->UnFloodFill(img,bMark);
			}			
		}
		
		Invalidate();
	    
		////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////
		CString s;
		s.Format("同心圆数量：\n 物体坐标系：%ld个, 光源坐标系：%ld", circle_number_object, circle_number_light);
		AfxMessageBox(s);

		delete []bMark;
		return;

}

void CDemoView::OnAdjust() 
{
	// TODO: Add your command handler code here
	RRPolynomial ia[3];
	CFileDialog dlg(TRUE,".pln",NULL,
	                 OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
	                 _T("Polynomial file (*.pln)|*.pln||"));
	if( dlg.DoModal()==IDOK )
	{
		CString filename = dlg.GetPathName();
		FILE *f=fopen(filename,"r");
		int ch,maxorder;
		RRArray<float> coeff;
		for(int rgb=0;rgb<3;rgb++)
		{
			fscanf(f,"Channel\t%d\n",&ch);
			fscanf(f,"Order\t%d\n",&maxorder);
			coeff.setSize(maxorder+1);
			for(int i=0;i<coeff.getSize();i++)
			{
				fscanf(f,"Coeff\t%d\t%f\n",&ch,&coeff[i]);
			}
			ia[rgb]=RRPolynomial(coeff);
		}
		fclose(f);
		CDemoDoc* pDoc = GetDocument();
		CxImage*  ima  = pDoc->GetImage();
		double r1,g1,b1;
		COLORREF c1;
		int i,j;
		int nW = ima->GetWidth();
		int nH = ima->GetHeight();
		for(j=0;j<nH;j++)
			for(i=0;i<nW;i++)
			{
				c1=ima->RGBQUADtoRGB(ima->GetPixelColor(i,j));
				r1=GetRValue(c1);
				g1=GetGValue(c1);
				b1=GetBValue(c1);
				
				r1=double(pow(ia[0].getY(r1/255.),1/2.2)*255);
				g1=double(pow(ia[1].getY(g1/255.),1/2.2)*255);
				b1=double(pow(ia[2].getY(b1/255.),1/2.2)*255);
				
				ima->SetPixelColor(i,j,RGB((BYTE)r1,(BYTE)g1,(BYTE)b1));
				
			}
			
	}	
	Invalidate();
}


void CDemoView::OnCalibConvertPurecolor() 
{
	
	// TODO: Add your command handler code here
	//得到要处理的图片的指针
	CDemoDoc* pDoc = GetDocument();
	CxImage* tempimg = pDoc->GetImage();	
	CxImage* img=pDoc->duplicate_image;
	img->Copy(*tempimg);

	int nfxfTolerance = 14;
	int i,j,yflip,nPureColor;
	for(j=0; j<int(img->GetHeight()); j++)
	{
		yflip = img->GetHeight() - j - 1;
		for(i=0; i< int(img->GetWidth()); i++)
		{
			nPureColor = IsPureColor(img,i,j,nfxfTolerance) ;
			if(nPureColor==YELLOW)
			{
				img->SetPixelColor(i,yflip,RGB(255,255,0));
			}
			else if(nPureColor==CYAN)
			{
				img->SetPixelColor(i,yflip,RGB(0,255,255));
			}
			else if(nPureColor==MAGENTA)
			{
				img->SetPixelColor(i,yflip,RGB(255,0,255));
			}
			else if(nPureColor==RED)
			{
				img->SetPixelColor(i,yflip,RGB(255,0,0));
			}
			else if(nPureColor==GREEN)
			{
				img->SetPixelColor(i,yflip,RGB(0,255,0));
			}
			else if(nPureColor==BLUE)
			{
				img->SetPixelColor(i,yflip,RGB(0,0,255));
			}
			else if(nPureColor==NOT_PURE_COLOR)
			{
				img->SetPixelColor(i,yflip,RGB(255,255,255));
			}
		}
	}
	Invalidate();
}

void CDemoView::OnComputeError() 
{
	//加载图像坐标数据：
	int nPtSum=myListPointer/4;
	POINT3D* pGcps1 = new POINT3D[nPtSum];
	phIMAGE* pImgs1 = new phIMAGE;
	pImgs1->pts=new POINT2D[nPtSum];
	        
    CString oo = _T("");
	int out,in,xxx,yyy;
	int nCount=0;
	for(int s= 0;s < myListPointer;)
	{
		xxx = myList[s ++];
		yyy = myList[s ++];
		out = myList[s ++];
		in  = myList[s ++];

		int row,col;

		if(out == RED) 
			row=0;		    	
		else if(out == GREEN)
			row=1;			
		else if(out == BLUE)
			row=2;		
		else if(out == CYAN) 
            row=5;   
		else if(out == MAGENTA) 
		    row=4;
		else if(out == YELLOW) 
			row=3;

		if(in == RED) 
			col=0;		    	
		else if(in == GREEN)
			col=1;			
		else if(in == BLUE)
			col=2;		
		else if(in == CYAN) 
            col=5;   
		else if(in == MAGENTA) 
		    col=4;
		else if(in == YELLOW) 
			col=3;

		pImgs1->pts[nCount].x = xxx;
		pImgs1->pts[nCount].y = yyy;
		
		pGcps1[nCount].x = GetApp()->m_CtrlPts[m_s][row*6+col].x;
		pGcps1[nCount].y = GetApp()->m_CtrlPts[m_s][row*6+col].y;;

		nCount++;		
	}
	
	if(m_HP.tz == INT_MAX)
	{
		AfxMessageBox("还没有导入相机参数");
		return;
	}
	
	double fError=0;

	for(int i=0;i<nCount;i++)
	{		
		POINT3D grd;
		grd.x=pGcps1[i].x;
		grd.y=pGcps1[i].y;
		grd.z=0;

		POINT2D img;			
		GrdToImg1(grd,img,m_HP);

        double x,y;
		x=pImgs1->pts[i].x;
		y=pImgs1->pts[i].y;

		fError=fError+sqrt( (img.x-x)*(img.x-x)+(img.y-y)*(img.y-y) );
	}	

	fError=fError/nCount;
    CString msg;
	msg.Format("The error is %f",fError);

	AfxMessageBox(msg);

	delete []pGcps1;
	delete []pImgs1->pts;
	delete pImgs1;
}


//计算物体坐标系的相机参数
bool CDemoView::ComputeObjectcamera()
{
	//判断是否导入控制点
	if( !GetApp()->m_bIsLoadCtrlPts )
	{
		AfxMessageBox("物体坐标系：图像控制点三维坐标还没导入");
		return false;
	}
	
	if(myListPointer <=0)
	{
		AfxMessageBox("物体坐标系：还没有标定图像上的控制点");
		return false;
	}
	
	if(myListPointer <4)
	{
		AfxMessageBox("物体坐标系：图像上的控制点数量不足");
		return false;
	}

	//读取图像的尺寸
	CDemoDoc* pDoc = GetDocument();
	CxImage*  ima  = pDoc->GetImage();
	if(ima)
	{
		nHeight = ima->GetHeight();
		nWidth = ima->GetWidth();
	}
		
	//加载图像坐标数据：
	int nPtSum=myListPointer/4;             //识别出的控制点的数量
	POINT3D* pGcps1 = new POINT3D[nPtSum];  //存储控制点的三维坐标
	phIMAGE* pImgs1 = new phIMAGE;         
	pImgs1->pts=new POINT2D[nPtSum];         //存储控制点的图像坐标
	        

	int out,in,xxx,yyy;
	int nCount=0;
	for(int s= 0;s < myListPointer;)
	{
		xxx = myList[s ++];
		yyy = myList[s ++];
		out = myList[s ++];
		in  = myList[s ++];

		int row,col;

		if(out == RED) 
			row=0;		    	
		else if(out == GREEN)
			row=1;			
		else if(out == BLUE)
			row=2;		
		else if(out == CYAN) 
            row=5;   
		else if(out == MAGENTA) 
		    row=4;
		else if(out == YELLOW) 
			row=3;

		if(in == RED) 
			col=0;		    	
		else if(in == GREEN)
			col=1;			
		else if(in == BLUE)
			col=2;		
		else if(in == CYAN) 
            col=5;   
		else if(in == MAGENTA) 
		    col=4;
		else if(in == YELLOW) 
			col=3;

		pImgs1->pts[nCount].x = xxx-nWidth/2;
		pImgs1->pts[nCount].y = yyy-nHeight/2;
#if 0
		pGcps1[nCount].x = GetApp()->m_CtrlPts[m_s][row*6+col].x;
		pGcps1[nCount].y = GetApp()->m_CtrlPts[m_s][row*6+col].y;
		pGcps1[nCount].z = GetApp()->m_CtrlPts[m_s][row*6+col].z;
#else		
		pGcps1[nCount].x = GetApp()->m_CtrlPts[0][row*6+col].x;
		pGcps1[nCount].y = GetApp()->m_CtrlPts[0][row*6+col].y;
		pGcps1[nCount].z = GetApp()->m_CtrlPts[0][row*6+col].z;
#endif
		nCount++;		
	}

    //相机标定
	cali.SetImgs(nPtSum,pImgs1);
	cali.SetGcps(nPtSum,pGcps1);

	//设置图像的宽高
	cali.nWid = ima->GetWidth();
	cali.nHei = ima->GetHeight();

	cali.m_surf = m_s;
	//计算相机参数
	cali.ComputeFlFromH(m_HP_object);	

	//释放内存
	delete []pGcps1;
	delete []pImgs1->pts;
	delete pImgs1;


	//////////////////////////////////////////////////////////////////////////
	//存储相机参数
	CString strFileSpt= pDoc->m_strFileName.Left(pDoc->m_strFileName.GetLength()-4) +".bmp(object).spt";

	//输出结果：
	FILE* fp=fopen(strFileSpt,"w");
	if(fp==NULL)
		return true;
	fprintf(fp,"%f\n", m_HP_object.f);
	fprintf(fp,"%f\n", m_HP_object.k);

	//for photogrammetry :
	fprintf(fp,"%d %d\n", m_HP_object.wid, m_HP_object.hei);
	fprintf(fp,"%f %f %f\n", m_HP_object.Xs, m_HP_object.Ys, m_HP_object.Zs);
	fprintf(fp,"%f %f %f\n", m_HP_object.faita, m_HP_object.omiga, m_HP_object.kapa);

	//for computer vision :
	fprintf(fp," %1.10lf  %1.10lf  %1.10lf  \n ",m_HP_object.r11,m_HP_object.r12,m_HP_object.r13);
	fprintf(fp," %1.10lf  %1.10lf  %1.10lf  \n ",m_HP_object.r21,m_HP_object.r22,m_HP_object.r23);
	fprintf(fp," %1.10lf  %1.10lf  %1.10lf  \n ",m_HP_object.r31,m_HP_object.r32,m_HP_object.r33);
	fprintf(fp," %f %f %f\n",m_HP_object.tx,m_HP_object.ty,m_HP_object.tz);
	fprintf(fp,"%lf %lf %lf %lf\n",m_HP_object.P[0][0],m_HP_object.P[0][1],m_HP_object.P[0][2],m_HP_object.P[0][3]);
	fprintf(fp,"%lf %lf %lf %lf\n",m_HP_object.P[1][0],m_HP_object.P[1][1],m_HP_object.P[1][2],m_HP_object.P[1][3]);
	fprintf(fp,"%lf %lf %lf %lf\n",m_HP_object.P[2][0],m_HP_object.P[2][1],m_HP_object.P[2][2],m_HP_object.P[2][3]);

	fclose(fp);

	CString strMsg;
	strMsg="物体坐标系：相机位置保存为"+strFileSpt+"文件";
	AfxMessageBox(strMsg);

	/////////////////////////////////////////////////////////////////////////
	return true;
}

//计算光源坐标系的相机参数
bool CDemoView::ComputeLightcamera()
{
	//判断是否导入控制点
	if( !GetApp()->m_bIsLoadCtrlPts_light )
	{
		AfxMessageBox("光源坐标系：图像控制点三维坐标还没导入");
		return false;
	}
	
	if(myListPointer_light <=0)
	{
		AfxMessageBox("光源坐标系：还没有标定图像上的控制点");
		return false;
	}
	
	if(myListPointer_light <4)
	{
		AfxMessageBox("光源坐标系：图像上的控制点数量不足");
		return false;
	}

	//读取图像的尺寸
	CDemoDoc* pDoc = GetDocument();
	CxImage*  ima  = pDoc->GetImage();
	if(ima)
	{
		nHeight = ima->GetHeight();
		nWidth = ima->GetWidth();
	}
		
	//加载图像坐标数据：
	int nPtSum=myListPointer_light/4;       //识别出的控制点的数量
	POINT3D* pGcps1 = new POINT3D[nPtSum];  //存储控制点的三维坐标
	phIMAGE* pImgs1 = new phIMAGE;         
	pImgs1->pts=new POINT2D[nPtSum];        //存储控制点的图像坐标
	        

	int outer,iner,xxx,yyy;
	int nCount=0;
	for(int s= 0;s < myListPointer_light;)
	{
		xxx = myList_light[s ++];
		yyy = myList_light[s ++];
		outer = myList_light[s ++];
		iner  = myList_light[s ++];

		int row,col;

		if(outer == RED) 
			row=0;		    	
		else if(outer == GREEN)
			row=1;			
		else if(outer == BLUE)
			row=2;		
		else if(outer == CYAN) 
            row=5;   
		else if(outer == MAGENTA) 
		    row=4;
		else if(outer == YELLOW) 
			row=3;

		if(iner == RED) 
			col=0;		    	
		else if(iner == GREEN)
			col=1;			
		else if(iner == BLUE)
			col=2;		
		else if(iner == CYAN) 
            col=5;   
		else if(iner == MAGENTA) 
		    col=4;
		else if(iner == YELLOW) 
			col=3;

		pImgs1->pts[nCount].x = xxx-nWidth/2;
		pImgs1->pts[nCount].y = yyy-nHeight/2;
		
		pGcps1[nCount].x = GetApp()->m_CtrlPts_light[row*6+col].x;
		pGcps1[nCount].y = GetApp()->m_CtrlPts_light[row*6+col].y;

		nCount++;		
	}

    //相机标定
	Calibration cali;
	cali.SetImgs(nPtSum,pImgs1);
	cali.SetGcps(nPtSum,pGcps1);

	//设置图像的宽高
	cali.nWid = ima->GetWidth();
	cali.nHei = ima->GetHeight();

	//计算相机参数
	cali.ComputeFlFromH(m_HP_light);	

	//释放内存
	delete []pGcps1;
	delete []pImgs1->pts;
	delete pImgs1;


	//////////////////////////////////////////////////////////////////////////
	//存储相机参数
	CString strFileSpt=pDoc->m_strFileName.Left(pDoc->m_strFileName.GetLength()-4) +".bmp(light).spt";

	//输出结果：
	FILE* fp=fopen(strFileSpt,"w");
	if(fp==NULL)
		return true;
	fprintf(fp,"%f\n", m_HP_light.f);
	fprintf(fp,"%f\n", m_HP_light.k);

	//for photogrammetry :
	fprintf(fp,"%d %d\n", m_HP_light.wid, m_HP_light.hei);
	fprintf(fp,"%f %f %f\n", m_HP_light.Xs, m_HP_light.Ys, m_HP_light.Zs);
	fprintf(fp,"%f %f %f\n", m_HP_light.faita, m_HP_light.omiga, m_HP_light.kapa);

	//for computer vision :
	fprintf(fp," %1.10lf  %1.10lf  %1.10lf  \n ",m_HP_light.r11,m_HP_light.r12,m_HP_light.r13);
	fprintf(fp," %1.10lf  %1.10lf  %1.10lf  \n ",m_HP_light.r21,m_HP_light.r22,m_HP_light.r23);
	fprintf(fp," %1.10lf  %1.10lf  %1.10lf  \n ",m_HP_light.r31,m_HP_light.r32,m_HP_light.r33);

	fprintf(fp," %f %f %f\n",m_HP_light.tx,m_HP_light.ty,m_HP_light.tz);

	fprintf(fp,"%lf %lf %lf %lf\n",m_HP_light.P[0][0],m_HP_light.P[0][1],m_HP_light.P[0][2],m_HP_light.P[0][3]);
	fprintf(fp,"%lf %lf %lf %lf\n",m_HP_light.P[1][0],m_HP_light.P[1][1],m_HP_light.P[1][2],m_HP_light.P[1][3]);
	fprintf(fp,"%lf %lf %lf %lf\n",m_HP_light.P[2][0],m_HP_light.P[2][1],m_HP_light.P[2][2],m_HP_light.P[2][3]);

	fclose(fp);

	CString strMsg;
	strMsg="光源坐标系：相机位置保存为"+strFileSpt+"文件";
	AfxMessageBox(strMsg);
	
	/////////////////////////////////////////////////////////////////////////
	return true;
}

void CDemoView::OnInverseByX() 
{
	// TODO: Add your command handler code here
	CDemoDoc *pDoc = GetDocument();
	pDoc->InverseImage(1);

	//设置标志，以便显示
	m_bInverse = TRUE;
	this->Invalidate();
}

void CDemoView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	CRect rect;
	this->GetClientRect(&rect);
	if(rect.PtInRect(point))
	{
		CMenu menu;
		menu.LoadMenu(IDR_MENU_INVERSE);
		menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON,point.x,point.y,pWnd); 
	}
	
}

void CDemoView::OnSaveInverseImage() 
{
	// TODO: Add your command handler code here
	CDemoDoc *pDoc = GetDocument();
	CxImage *pInverseImg = pDoc->GetInverseImg();

	CFileDialog  dilg(FALSE, NULL, "*.jpg",    //SAVEFILENAME  Lookup table
	                  OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
	            	  "Lookup table (*.jpg)|*.jpg|All Files (*.*)|*.*||");
	dilg.m_ofn.lpstrTitle="Load Lookup table File";
	
	if (dilg.DoModal() == IDOK)
	{	
		this->UpdateData(0);
		CString fileName = dilg.GetPathName( );		
		if(fileName.IsEmpty())
		{
			MessageBox("Please select a jpg file");
		}
		pInverseImg->Save(fileName,CXIMAGE_FORMAT_JPG);
	}
}

void CDemoView::OnCpmputeVirtualCamera() 
{
	// TODO: Add your command handler code here
	//计算虚拟相机参数
	STRU_LFromH virtualCamera;
	cali.ComputeVirtualCamera(m_HP_object,virtualCamera);

	//存储虚拟相机参数
	CDemoDoc* pDoc = GetDocument();
	CString strFileSpt=pDoc->m_strFileName.Left(pDoc->m_strFileName.GetLength()-4) +".bmp_virtual.spt";
	FILE* fp=fopen(strFileSpt,"w");

	if(fp == NULL)
		return;
	
	fprintf(fp,"%f\n", virtualCamera.f);
	fprintf(fp,"%f\n", virtualCamera.k);

	//for photogrammetry :
	fprintf(fp,"%d %d\n", virtualCamera.wid, virtualCamera.hei);
	fprintf(fp,"%f %f %f\n", virtualCamera.Xs, virtualCamera.Ys, virtualCamera.Zs);
	fprintf(fp,"%f %f %f\n", virtualCamera.faita, virtualCamera.omiga, virtualCamera.kapa);

	//for computer vision :
	fprintf(fp," %1.10lf  %1.10lf  %1.10lf  \n ",virtualCamera.r11,virtualCamera.r12,virtualCamera.r13);
	fprintf(fp," %1.10lf  %1.10lf  %1.10lf  \n ",virtualCamera.r21,virtualCamera.r22,virtualCamera.r23);
	fprintf(fp," %1.10lf  %1.10lf  %1.10lf  \n ",virtualCamera.r31,virtualCamera.r32,virtualCamera.r33);
	fprintf(fp," %f %f %f\n",virtualCamera.tx,virtualCamera.ty,virtualCamera.tz);
	fprintf(fp,"%lf %lf %lf %lf\n",virtualCamera.P[0][0],virtualCamera.P[0][1],virtualCamera.P[0][2],virtualCamera.P[0][3]);
	fprintf(fp,"%lf %lf %lf %lf\n",virtualCamera.P[1][0],virtualCamera.P[1][1],virtualCamera.P[1][2],virtualCamera.P[1][3]);
	fprintf(fp,"%lf %lf %lf %lf\n",virtualCamera.P[2][0],virtualCamera.P[2][1],virtualCamera.P[2][2],virtualCamera.P[2][3]);

	fclose(fp);

	CString strMsg;
	strMsg="物体坐标系：虚拟相机位置保存为"+strFileSpt+"文件";
	AfxMessageBox(strMsg);
}

void CDemoView::OnVerifyAlgorithm() 
{
	if( !GetApp()->m_bIsLoadCtrlPts ){
		AfxMessageBox("物体坐标系：图像控制点三维坐标还没导入");
		return;
	}

	double nThetaS=0;//start
	double nPhiS=0;
	double nRadS=30;
	double nNoiseS=0;

	double nThetaI=10;//interval
	double nPhiI=10;
	double nRadI=10;
	double nNoiseI=0.5;

	double nThetaE=90;//end
	double nPhiE=360;
	double nRadE=70;
	double nNoiseE=5;

	CVerifyDlg paraDlg;
	if( paraDlg.DoModal() == IDOK)
	{
		nThetaS=paraDlg.m_nThetaS;
		nPhiS=paraDlg.m_nPhiS;
		nRadS=paraDlg.m_nRadS;
		nNoiseS=paraDlg.m_nNoiseS;

		nThetaI=paraDlg.m_nThetaI;
		nPhiI=paraDlg.m_nPhiI;
		nRadI=paraDlg.m_nRadI;
		nNoiseI=paraDlg.m_nNoiseI;

		nThetaE=paraDlg.m_nThetaE;
		nPhiE=paraDlg.m_nPhiE;
		nRadE=paraDlg.m_nRadE;
		nNoiseE=paraDlg.m_nNoiseE;
	}
	else
	{
		return;
	}

	CDemoDoc* pDoc = GetDocument();
	CString strFileSpt="error.txt";
	FILE* fp=fopen(strFileSpt,"w");

	if(fp == NULL)	return;

	STRU_LFromH vertualCamera;
	
	int nCamCount=0;
	int nRightCam=0;

	for(double nn=nNoiseS;nn<=nNoiseE;nn+=nNoiseI){
		for(double nr=nRadS;nr<=nRadE;nr+=nRadI){
			for(double nt=nThetaS;nt<nThetaE;nt+=nThetaI){
				for(double np=nPhiS;np<nPhiE;np+=nPhiI){
					m_s=0;
					int nCount = 0;
					
					POINT3D* pGcps1 = new POINT3D[36];
					phIMAGE* pImgs1 = new phIMAGE;         
					pImgs1->pts=new POINT2D[36];

				//Generate a Virtual Camera in m_HP
					GenerateVirtualCamera(nr,nt,np,
										15,0,15,
										700,640,480);
						//theta [0,180] phi[0,360]
					CMatrix h(3,3);
					h.SetElement(0,0,m_HP.r11*m_HP.f);
					h.SetElement(0,1,m_HP.r13*m_HP.f);
					h.SetElement(0,2,m_HP.tx*m_HP.f);
					h.SetElement(1,0,m_HP.r21*m_HP.f*m_HP.k);
					h.SetElement(1,1,m_HP.r23*m_HP.f*m_HP.k);
					h.SetElement(1,2,m_HP.ty*m_HP.f*m_HP.k);
					h.SetElement(2,0,m_HP.r31);
					h.SetElement(2,1,m_HP.r33);
					h.SetElement(2,2,m_HP.tz);

				//Compute virtual picture
					CMatrix zeros(3,1);
					for(int i=0;i<36;i++){

						CMatrix ctrlPnt(3,1);
						ctrlPnt.SetElement(0,0,GetApp()->m_CtrlPts[0][i].x);
						ctrlPnt.SetElement(1,0,GetApp()->m_CtrlPts[0][i].z);
						ctrlPnt.SetElement(2,0,1);
						CMatrix boundPntx(3,1);
						boundPntx.SetElement(0,0,GetApp()->m_CtrlPts[0][i].x+1);//内圆半径
						boundPntx.SetElement(1,0,GetApp()->m_CtrlPts[0][i].z);
						boundPntx.SetElement(2,0,1);
						CMatrix boundPnty(3,1);
						boundPnty.SetElement(0,0,GetApp()->m_CtrlPts[0][i].x);//内圆半径
						boundPnty.SetElement(1,0,GetApp()->m_CtrlPts[0][i].z+1);
						boundPnty.SetElement(2,0,1);

						if (zeros != ctrlPnt){

							CMatrix imgPnt(3,1);
							CMatrix imgBoundx(3,1);
							CMatrix imgBoundy(3,1);

							imgPnt=h*ctrlPnt;
							imgBoundx=h*boundPntx;
							imgBoundy=h*boundPnty;

							int imgx,imgy,boundxx,boundxy,boundyx,boundyy;
							//加噪栅格化
							Rasterize(imgx,imgPnt.GetElement(0,0)/imgPnt.GetElement(2,0),nn);
							Rasterize(imgy,imgPnt.GetElement(1,0)/imgPnt.GetElement(2,0),nn);
							Rasterize(boundxx,imgBoundx.GetElement(0,0)/imgBoundx.GetElement(2,0),nn);
							Rasterize(boundxy,imgBoundx.GetElement(1,0)/imgBoundx.GetElement(2,0),nn);
							Rasterize(boundyx,imgBoundy.GetElement(0,0)/imgBoundy.GetElement(2,0),nn);
							Rasterize(boundyy,imgBoundy.GetElement(1,0)/imgBoundy.GetElement(2,0),nn);

							if(imgx>m_HP.wid/2||imgx<-m_HP.wid/2||imgy>m_HP.hei/2||imgy<-m_HP.hei/2) {
									break;
							}
							double imgrx2=(imgx-boundxx)*(imgx-boundxx)+(imgy-boundxy)*(imgy-boundxy);
							double imgry2=(imgx-boundyx)*(imgx-boundyx)+(imgy-boundyy)*(imgy-boundyy);
							if(imgrx2*imgry2<=256 ){ 
									break;//图像内圆半径识别限制
							}

							pImgs1->pts[nCount].x = double(imgx);
							pImgs1->pts[nCount].y = double(imgy);

							pGcps1[nCount].x = GetApp()->m_CtrlPts[0][i].x;
							pGcps1[nCount].y = GetApp()->m_CtrlPts[0][i].y;
							pGcps1[nCount].z = GetApp()->m_CtrlPts[0][i].z;

							nCount++;
						}
					}
				
				//computeH
					if(nCount>=4){

						cali.SetImgs(nCount,pImgs1);
						cali.SetGcps(nCount,pGcps1);

						cali.nWid = m_HP.wid;
						cali.nHei = m_HP.hei;

						cali.m_surf = m_s;

						cali.ComputeFlFromH(m_HP_object);	

						delete []pGcps1;
						delete []pImgs1->pts;
						delete pImgs1;
					//方位误差
						double epos=sqrt(
											(fabs(m_HP.Xs)-fabs(m_HP_object.Xs))*(fabs(m_HP.Xs)-fabs(m_HP_object.Xs))+
											(fabs(m_HP.Ys)-fabs(m_HP_object.Ys))*(fabs(m_HP.Ys)-fabs(m_HP_object.Ys))+
											(fabs(m_HP.Zs)-fabs(m_HP_object.Zs))*(fabs(m_HP.Zs)-fabs(m_HP_object.Zs))
										)/
										sqrt(fabs(m_HP.Xs)*fabs(m_HP.Xs)+fabs(m_HP.Ys)*fabs(m_HP.Ys)+fabs(m_HP.Zs)*fabs(m_HP.Zs));
					//旋转矩阵角度误差
						double er1=acos((fabs(m_HP.r11)*fabs(m_HP_object.r11)+fabs(m_HP.r21)*fabs(m_HP_object.r21)+fabs(m_HP.r31)*fabs(m_HP_object.r31))/
										sqrt(fabs(m_HP.r11)*fabs(m_HP.r11)+fabs(m_HP.r21)*fabs(m_HP.r21)+fabs(m_HP.r31)*fabs(m_HP.r31))/
										sqrt(fabs(m_HP_object.r11)*fabs(m_HP_object.r11)+fabs(m_HP_object.r21)*fabs(m_HP_object.r21)+fabs(m_HP_object.r31)*fabs(m_HP_object.r31)));
						double er2=acos((fabs(m_HP.r12)*fabs(m_HP_object.r12)+fabs(m_HP.r22)*fabs(m_HP_object.r22)+fabs(m_HP.r32)*fabs(m_HP_object.r32))/
										sqrt(fabs(m_HP.r12)*fabs(m_HP.r12)+fabs(m_HP.r22)*fabs(m_HP.r22)+fabs(m_HP.r32)*fabs(m_HP.r32))/
										sqrt(fabs(m_HP_object.r12)*fabs(m_HP_object.r12)+fabs(m_HP_object.r22)*fabs(m_HP_object.r22)+fabs(m_HP_object.r32)*fabs(m_HP_object.r32)));
						double er3=acos((fabs(m_HP.r13)*fabs(m_HP_object.r13)+fabs(m_HP.r23)*fabs(m_HP_object.r23)+fabs(m_HP.r33)*fabs(m_HP_object.r33))/
										sqrt(fabs(m_HP.r13)*fabs(m_HP.r13)+fabs(m_HP.r23)*fabs(m_HP.r23)+fabs(m_HP.r33)*fabs(m_HP.r33))/
										sqrt(fabs(m_HP_object.r13)*fabs(m_HP_object.r13)+fabs(m_HP_object.r23)*fabs(m_HP_object.r23)+fabs(m_HP_object.r33)*fabs(m_HP_object.r33)));
						if(epos<=0.2){
							nCamCount++;

							fprintf(fp,"%f %f %f %f %f %f %f %f %f %f %f\n", m_HP.Xs, m_HP.Ys, m_HP.Zs, 
																	nn, nr, nt, np, 
																	epos, er1, er2, er3);
							if(nt==0) break;
						}
					}
				}
			}
		}
	}
	fclose(fp);

	CString strMsg;
	strMsg="算法验证误差数据保存为"+strFileSpt+"文件";
	AfxMessageBox(strMsg);
	return;
}

bool CDemoView::GenerateVirtualCamera(double r, double theta, double phi, 
									  double x0, double y0, double z0, 
									  double f, double w, double h){
	//theta [0,180] phi[0,360]
	//m_HP
	m_HP.f=f;
	m_HP.k=1.0;
	m_HP.wid=w;
	m_HP.hei=h;
	m_HP.Xs=r*sin(theta*PI/180)*sin(phi*PI/180)+x0;
	m_HP.Ys=r*cos(theta*PI/180)+y0;
	m_HP.Zs=r*sin(theta*PI/180)*cos(phi*PI/180)+z0;


	double cx=cos((theta-90)*PI/180);
	double sx=sin((theta-90)*PI/180);
	double cy=cos(phi*PI/180);
	double sy=sin(phi*PI/180);
	//cz=1 sz=0;

	m_HP.r11=cy;
	m_HP.r12=0;
	m_HP.r13=-sy;
	m_HP.r21=sx*sy;
	m_HP.r22=cx;
	m_HP.r23=sx*cy;
	m_HP.r31=cx*sy;
	m_HP.r32=-sx;
	m_HP.r33=cx*cy;

	m_HP.tx=-m_HP.r11*m_HP.Xs-m_HP.r12*m_HP.Ys-m_HP.r13*m_HP.Zs;
	m_HP.ty=-m_HP.r21*m_HP.Xs-m_HP.r22*m_HP.Ys-m_HP.r23*m_HP.Zs;
	m_HP.tz=-m_HP.r31*m_HP.Xs-m_HP.r32*m_HP.Ys-m_HP.r33*m_HP.Zs;

	m_HP.faita = atan(-m_HP.r31/m_HP.r33)/PI*180;
	m_HP.omiga = asin(-m_HP.r32)/PI*180;
	m_HP.kapa  = atan(m_HP.r12/m_HP.r22)/PI*180;

	m_HP.P[0][0]=f*m_HP.r11+w/2*m_HP.r31;
	m_HP.P[0][1]=f*m_HP.r12+w/2*m_HP.r32;
	m_HP.P[0][2]=f*m_HP.r13+w/2*m_HP.r33;
	m_HP.P[0][3]=f*m_HP.tx +w/2*m_HP.tz;

	m_HP.P[1][0]=f*m_HP.r21+h/2*m_HP.r31;
	m_HP.P[1][1]=f*m_HP.r22+h/2*m_HP.r32;
	m_HP.P[1][2]=f*m_HP.r23+h/2*m_HP.r33;
	m_HP.P[1][3]=f*m_HP.ty +h/2*m_HP.tz;

	m_HP.P[2][0]=m_HP.r31;
	m_HP.P[2][1]=m_HP.r32;
	m_HP.P[2][2]=m_HP.r33;
	m_HP.P[2][3]=m_HP.tz;

	return true;
}

bool CDemoView::Rasterize(int &pos, double input, double noiseLevel){
	input=input+(double(rand()%2000-1000)/1000.0)*noiseLevel;
	pos=int(input+0.5);
	return true;
}

