// DlgBatchImgCut.cpp : implementation file
//

#include "stdafx.h"
#include "demo.h"
#include "ProgressBar.h"
#include "DlgBatchImgCut.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgBatchImgCut dialog


CDlgBatchImgCut::CDlgBatchImgCut(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgBatchImgCut::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgBatchImgCut)
	m_nBottomBoundary = 0;
	m_nLeftBoundary = 0;
	m_nRightBoundary = 0;
	m_nTopBoundary = 0;

	m_nImgFileSum = 0;

	m_nImgHeight = 0;
	m_nImgWidth = 0;

	m_pSampleImg = NULL;

	m_bIsLeftMouseDown = FALSE;
	//}}AFX_DATA_INIT
}

CDlgBatchImgCut::~CDlgBatchImgCut()
{
	if(m_pSampleImg != NULL)
		delete m_pSampleImg;
}


void CDlgBatchImgCut::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgBatchImgCut)
	DDX_Control(pDX, IDC_IMAGE_ZONE, m_ctrlImgZone);
	DDX_Text(pDX, IDC_EDIT_BOTTOM_BOUNDARY, m_nBottomBoundary);
	DDX_Text(pDX, IDC_EDIT_LEFT_BOUNDARY, m_nLeftBoundary);
	DDX_Text(pDX, IDC_EDIT_RIGHT_BOUNDARY, m_nRightBoundary);
	DDX_Text(pDX, IDC_EDIT_TOP_BOUNDARY, m_nTopBoundary);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgBatchImgCut, CDialog)
	//{{AFX_MSG_MAP(CDlgBatchImgCut)
	ON_BN_CLICKED(ID_START_CUT, OnStartCut)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_IMAGES, OnButtonLoadImages)
	ON_BN_CLICKED(ID_OK, OnOk)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgBatchImgCut message handlers

CString FindExtension(const CString& name)
{
	int len = name.GetLength();
	int i;
	for (i = len-1; i >= 0; i--){
		if (name[i] == '.'){
			return name.Mid(i+1);
		}
	}
	return CString("");
}

void CDlgBatchImgCut::OnStartCut() 
{
	// TODO: Add your control notification handler code here
	if(m_nImgFileSum < 1)
	{
		AfxMessageBox("还没有选择文件!!");
		return;
	}

	UpdateData(TRUE);

	//设置进度条
	CProgressBar bar(_T("Progress"), 40, m_nImgFileSum);
// 	for (int i = 0; i < 5000; i++) 
// 	{
// 		CString str;
// 		str.Format("%d%% complete", i*100/5000);
// 		bar.SetText(str);
// 
// 		bar.StepIt();
// 	//	PeekAndPump();
// 	}	

	//确定输出文件类型
	CxImage *pImage = NULL;
	CxImage *pNewImage = NULL;
	int nWidth = m_nRightBoundary - m_nLeftBoundary + 1;
	int nHei = m_nTopBoundary - m_nBottomBoundary + 1;
	ASSERT(nWidth > 1);
	ASSERT(nHei > 1);

	CString outputExt = "jpg";
	BOOL bError = FALSE;

	//逐个文件处理
	for(int i=0;i<m_nImgFileSum;i++)
	{	
		CString filename(m_strImgFilePaths[i]);
		CString ext(FindExtension(filename));
		ext.MakeLower();

		if (ext == "") 
		{
			bError = TRUE;
			return;
		}

		//确定源图像文件的类型
		int type = 0;
		if (ext == "bmp")					type = CXIMAGE_FORMAT_BMP;
	#if CXIMAGE_SUPPORT_JPG
		else if (ext=="jpg"||ext=="jpeg")	type = CXIMAGE_FORMAT_JPG;
	#endif
	#if CXIMAGE_SUPPORT_GIF
		else if (ext == "gif")				type = CXIMAGE_FORMAT_GIF;
	#endif
	#if CXIMAGE_SUPPORT_PNG
		else if (ext == "png")				type = CXIMAGE_FORMAT_PNG;
	#endif
	#if CXIMAGE_SUPPORT_MNG
		else if (ext=="mng"||ext=="jng")	type = CXIMAGE_FORMAT_MNG;
	#endif
	#if CXIMAGE_SUPPORT_ICO
		else if (ext == "ico")				type = CXIMAGE_FORMAT_ICO;
	#endif
	#if CXIMAGE_SUPPORT_TIF
		else if (ext=="tiff"||ext=="tif")	type = CXIMAGE_FORMAT_TIF;
	#endif
	#if CXIMAGE_SUPPORT_TGA
		else if (ext=="tga")				type = CXIMAGE_FORMAT_TGA;
	#endif
	#if CXIMAGE_SUPPORT_PCX
		else if (ext=="pcx")				type = CXIMAGE_FORMAT_PCX;
	#endif
	#if CXIMAGE_SUPPORT_WBMP
		else if (ext=="wbmp")				type = CXIMAGE_FORMAT_WBMP;
	#endif
	#if CXIMAGE_SUPPORT_WMF
		else if (ext=="wmf"||ext=="emf")	type = CXIMAGE_FORMAT_WMF;
	#endif
	#if CXIMAGE_SUPPORT_J2K
		else if (ext=="j2k"||ext=="jp2")	type = CXIMAGE_FORMAT_J2K;
	#endif
	#if CXIMAGE_SUPPORT_JBG
		else if (ext=="jbg")				type = CXIMAGE_FORMAT_JBG;
	#endif
		else {bError = TRUE; return;}

		int nOutputType = type;
		
		if(pImage!=NULL)
		{
			delete pImage;
			pImage = NULL;
		}

		pImage = new CxImage(filename, type);
		if (!pImage->IsValid())
		{
			AfxMessageBox(pImage->GetLastError());
			delete pImage;
			pImage = NULL;
			bError = TRUE;
			return;
		}

		if(pNewImage != NULL)
		{ 
			delete pNewImage;
			pNewImage = NULL;
		}

		pNewImage = new CxImage;
		pNewImage->Create(nWidth,nHei,24,CXIMAGE_FORMAT_JPG);
		int row,col;
		RGBQUAD RGBColor;

		for(row = 0; row < nHei-1; row++)
		{
			for(col = 0; col < nWidth-1; col++)
			{
				RGBColor = pImage->GetPixelColor(col+m_nLeftBoundary, row+m_nBottomBoundary);
				pNewImage->SetPixelColor(col,row,RGBColor);
			}
		}

		CString strAddedExt,strTemp;
		strAddedExt.Format("-%dx%d.%s",nWidth,nHei,"jpg");
		strTemp = filename+strAddedExt;
		pNewImage->Save(strTemp,CXIMAGE_FORMAT_JPG);



		//显示进度条
		CString str;
		str.Format("%d%% complete", (i+1)*100/m_nImgFileSum);
		bar.SetText(str);
		bar.StepIt();
	}

	AfxMessageBox("转换完成");
}

void CDlgBatchImgCut::OnButtonLoadImages() 
{
	// TODO: Add your control notification handler code here
	//加载多幅图像：
	char strFilter[]="BMP (*.bmp)|*.bmp|JPEG (*.jpg)|*.jpg|GIF (*.gif)|*.gif|PNG (*.png)|*.png|All Files (*.*)|*.*||";

	CFECFileDialog fDlg(1,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,strFilter);
	
	fDlg.m_ofn.Flags |=OFN_ALLOWMULTISELECT;//allow to multiple selection
	fDlg.m_ofn.lpstrTitle="Select the Image file to convert:";


	if(fDlg.DoModal()==IDOK)
	{
		CWaitCursor wt;
		POSITION pos=fDlg.GetStartPosition();
		if(pos==NULL) return;

		int nSum=0;
		while(pos!=NULL)
		{
			m_strImgFilePaths[nSum]=fDlg.GetNextPathName(pos);
			//fDlg.GetNextPathName(pos);
			nSum++;
		}
		
		m_nImgFileSum=nSum;
	}

	if(m_nImgFileSum > 0)
	{
		m_pSampleImg = new CxImage;
		m_pSampleImg->Load(m_strImgFilePaths[0]);

		m_nImgHeight = m_pSampleImg->GetHeight();
		m_nImgWidth = m_pSampleImg->GetWidth();

		Invalidate();
	}
}

void CDlgBatchImgCut::OnOk() 
{
	// TODO: Add your control notification handler code here
	if(m_pSampleImg != NULL)
 		delete m_pSampleImg;
}

void CDlgBatchImgCut::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	// TODO: Add your message handler code here
	CRect rectImgZone;
	m_ctrlImgZone.GetWindowRect(&rectImgZone);  //屏幕坐标
	ScreenToClient(&rectImgZone);				//客户区坐标

	if (m_pSampleImg == NULL)
	{
		COLORREF backgrdColor = RGB(128,128,128);
		CBrush brush(backgrdColor);
		dc.FillRect(&rectImgZone,&brush);

		return;
	}

	//将图像缩放至绘图区域
	m_pSampleImg->Stretch(dc.GetSafeHdc(),rectImgZone);

	//在矩形选框的四个顶点处，标明对应的图像坐标
	int nHei = m_pSampleImg->GetHeight();
	int nWid = m_pSampleImg->GetWidth();

	CString text;
	text.Format("(%d,%d)",0,0);
	dc.TextOut(rectImgZone.left,rectImgZone.bottom-20,text);
	text.Format("(%d,%d)",nWid-1,0);
	dc.TextOut(rectImgZone.right-60,rectImgZone.bottom-20,text);
	text.Format("(%d,%d)",0,nHei-1);
	dc.TextOut(rectImgZone.left,rectImgZone.top,text);
	text.Format("(%d,%d)",nWid-1,nHei-1);
	dc.TextOut(rectImgZone.right-60,rectImgZone.top,text);


	DWORD	dwdStyles[] = {1,1};   
	LOGBRUSH   lb;   
	lb.lbColor = RGB(255,255,0);   
	lb.lbStyle = BS_SOLID;

	HPEN hPen = ExtCreatePen(PS_GEOMETRIC| PS_DOT|PS_ENDCAP_ROUND,1,&lb,2,dwdStyles);   
	CPen *  pOldPen   =   dc.SelectObject(CPen::FromHandle(hPen));   
	// Draw lines here
	dc.SelectObject(pOldPen);

	//获取矩形选框的四个顶点的屏幕坐标
	int nRectWidth = rectImgZone.right - rectImgZone.left + 1;
	int nRectHeight = rectImgZone.bottom - rectImgZone.top + 1;

	CPoint pointArray[4];     
	pointArray[0] = m_pointMouseDown;
	pointArray[1].x = m_pointMouseDown.x;
	pointArray[1].y = m_pointCurrent.y;
	pointArray[2] = m_pointCurrent;
	pointArray[3].x = m_pointCurrent.x;
	pointArray[3].y = m_pointMouseDown.y;
//	LPtoDP(dc.GetSafeHdc(),pointArray,4);   //客户区逻辑坐标-->屏幕坐标

	dc.MoveTo(pointArray[0]);
	dc.LineTo(pointArray[1]);
	dc.LineTo(pointArray[2]);
	dc.LineTo(pointArray[3]);
	dc.LineTo(pointArray[0]);

	//计算矩形选框对应的图像区域
	if(m_pointMouseDown.x < m_pointCurrent.x)
	{
		m_nLeftBoundary = ((m_pointMouseDown.x-rectImgZone.left)*m_nImgWidth)/nRectWidth;
		m_nRightBoundary = ((m_pointCurrent.x-rectImgZone.left)*m_nImgWidth)/nRectWidth;;
	}
	else
	{
		m_nLeftBoundary = ((m_pointCurrent.x-rectImgZone.left)*m_nImgWidth)/nRectWidth;;
		m_nRightBoundary = ((m_pointMouseDown.x-rectImgZone.left)*m_nImgWidth)/nRectWidth;
	}

	if(m_pointMouseDown.y < m_pointCurrent.y)
	{
		m_nBottomBoundary = (rectImgZone.bottom-m_pointCurrent.y)*m_nImgHeight/nRectHeight;
		m_nTopBoundary = (rectImgZone.bottom-m_pointMouseDown.y)*m_nImgHeight/nRectHeight;
	}
	else
	{
		m_nBottomBoundary = (rectImgZone.bottom-m_pointMouseDown.y)*m_nImgHeight/nRectHeight;	
		m_nTopBoundary = (rectImgZone.bottom-m_pointCurrent.y)*m_nImgHeight/nRectHeight;
	}

	UpdateData(FALSE);

	// Do not call CDialog::OnPaint() for painting messages
}

void CDlgBatchImgCut::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_pSampleImg == NULL)
		return;
	
	m_bIsLeftMouseDown = TRUE;
	m_pointMouseDown = point;

	CDialog::OnLButtonDown(nFlags, point);
}

void CDlgBatchImgCut::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(!m_bIsLeftMouseDown)
		return;

	m_pointCurrent = point;

	CDialog::OnMouseMove(nFlags, point);

	InvalidateRect(NULL,FALSE);
}

void CDlgBatchImgCut::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_bIsLeftMouseDown = FALSE;
	
	CDialog::OnLButtonUp(nFlags, point);
}
