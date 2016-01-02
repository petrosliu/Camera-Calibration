// DlgCalibBatchImgReSize.cpp : implementation file
//

#include "stdafx.h"
#include "demo.h"
#include "DlgCalibBatchImgReSize.h"

#include "ProgressBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCalibBatchImgReSize dialog

void SelectImageZone(CString imgFileName);

//for Progressbar
BOOL PeekAndPump()
{
	static MSG msg;

	while (::PeekMessage(&msg,NULL,0,0,PM_NOREMOVE)) {
		if (!AfxGetApp()->PumpMessage()) {
			::PostQuitMessage(0);
			return FALSE;
		}	
	}

	return TRUE;
}

CDlgCalibBatchImgReSize::CDlgCalibBatchImgReSize(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCalibBatchImgReSize::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCalibBatchImgReSize)
	m_nNewHeight = 480;
	m_nNewWidth = 640;
	//}}AFX_DATA_INIT
	m_bError = FALSE;
	m_nFileSum = 0;
}


void CDlgCalibBatchImgReSize::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCalibBatchImgReSize)
	DDX_Control(pDX, IDC_COMBO1, m_cbFileType);
	DDX_Text(pDX, IDC_EDIT_CALIBRATION_BATCHIMG_HEIGHT, m_nNewHeight);
	DDX_Text(pDX, IDC_EDIT_CALIBRATION_BATCHIMG_WIDTH, m_nNewWidth);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCalibBatchImgReSize, CDialog)
	//{{AFX_MSG_MAP(CDlgCalibBatchImgReSize)
	ON_BN_CLICKED(IDC_BUTTON_CALIBRATION_LOADBATCHIMG, OnButtonCalibrationLoadbatchimg)
	ON_BN_CLICKED(ID_BUTTON_CALIBRATION_BATCHIMG_CONVERT, OnButtonConvert)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCalibBatchImgReSize message handlers

void CDlgCalibBatchImgReSize::OnButtonCalibrationLoadbatchimg() 
{
	// TODO: Add your control notification handler code here


	//加载多幅图像：
	char strFilter[]="BMP (*.bmp)|*.bmp|JPEG (*.jpg)|*.jpg|GIF (*.gif)|*.gif|PNG (*.png)|*.png|All Files (*.*)|*.*||";
//	CFileDialog fDlg(1,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,strFilter);
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
			m_ImgFilesPath[nSum]=fDlg.GetNextPathName(pos);
			//fDlg.GetNextPathName(pos);
			nSum++;
		}
		
		m_nFileSum=nSum;
//		for(int i=0;i<m_nFileSum;i++)
//		{
//			m_Images[i].Load( GetApp()->m_ImgFilesPath[i] );
//		}

		//
		//显示控制面板:
//		GetMainFrame()->ShowControlBar(&GetMainFrame()->m_SideBar,1,0);
		

//		Convert();

	}
	
}

CString CDlgCalibBatchImgReSize::FindExtension(const CString& name)
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

BOOL CDlgCalibBatchImgReSize::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	int nTypeNum = m_cbFileType.GetCount();
	if(nTypeNum>0)
		m_cbFileType.SetCurSel(0);

	image = NULL;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgCalibBatchImgReSize::OnButtonConvert() 
{
	// TODO: Add your control notification handler code here
	if(m_nFileSum < 1)
	{
		AfxMessageBox("还没有选择文件!!");
		return;
	}

	UpdateData(TRUE);

	//设置进度条
	CProgressBar bar(_T("Progress"), 40, m_nFileSum);
//	for (int i = 0; i < 5000; i++) {
//
//		CString str;
//		str.Format("%d%% complete", i*100/5000);
//		bar.SetText(str);
//
//		bar.StepIt();
//		PeekAndPump();
//	}	

	//确定输出文件类型
	CString outputExt;
	m_cbFileType.GetLBText(m_cbFileType.GetCurSel(),outputExt);
	outputExt.MakeLower();
	m_strOutputExt = outputExt;

	m_nOutputType = 0;
	
		if (outputExt == "bmp")					m_nOutputType = CXIMAGE_FORMAT_BMP;
	#if CXIMAGE_SUPPORT_JPG
		else if (outputExt=="jpg"||outputExt=="jpeg")	m_nOutputType = CXIMAGE_FORMAT_JPG;
	#endif
	#if CXIMAGE_SUPPORT_GIF
		else if (outputExt == "gif")				m_nOutputType = CXIMAGE_FORMAT_GIF;
	#endif
	#if CXIMAGE_SUPPORT_PNG
		else if (outputExt == "png")				m_nOutputType = CXIMAGE_FORMAT_PNG;
	#endif
	#if CXIMAGE_SUPPORT_MNG
		else if (outputExt=="mng"||outputExt=="jng")	m_nOutputType = CXIMAGE_FORMAT_MNG;
	#endif
	#if CXIMAGE_SUPPORT_ICO
		else if (outputExt == "ico")				m_nOutputType = CXIMAGE_FORMAT_ICO;
	#endif
	#if CXIMAGE_SUPPORT_TIF
		else if (outputExt=="tiff"||outputExt=="tif")	m_nOutputType = CXIMAGE_FORMAT_TIF;
	#endif
	#if CXIMAGE_SUPPORT_TGA
		else if (outputExt=="tga")				m_nOutputType = CXIMAGE_FORMAT_TGA;
	#endif
	#if CXIMAGE_SUPPORT_PCX
		else if (outputExt=="pcx")				m_nOutputType = CXIMAGE_FORMAT_PCX;
	#endif
	#if CXIMAGE_SUPPORT_WBMP
		else if (outputExt=="wbmp")				m_nOutputType = CXIMAGE_FORMAT_WBMP;
	#endif
	#if CXIMAGE_SUPPORT_WMF
		else if (outputExt=="wmf"||outputExt=="emf")	m_nOutputType = CXIMAGE_FORMAT_WMF;
	#endif
	#if CXIMAGE_SUPPORT_J2K
		else if (outputExt=="j2k"||outputExt=="jp2")	m_nOutputType = CXIMAGE_FORMAT_J2K;
	#endif
	#if CXIMAGE_SUPPORT_JBG
		else if (outputExt=="jbg")				m_nOutputType = CXIMAGE_FORMAT_JBG;
	#endif
		else {m_bError = TRUE; return;}

//逐个文件处理
	for(int i=0;i<m_nFileSum;i++)
	{
//		SelectImageZone(m_ImgFilesPath[i]);
			
		CString filename(m_ImgFilesPath[i]);
		CString ext(FindExtension(filename));
		ext.MakeLower();
		if (ext == "") {m_bError = TRUE; return;}
		m_strInputExt = ext;

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
		else {m_bError = TRUE; return;}

		m_nInputType = type;
		
		//Stopwatch(0);
		if(image!=NULL)
		{
			delete image;
			image = NULL;
		}

		image = new CxImage(filename, type);
		if (!image->IsValid()){
			AfxMessageBox(image->GetLastError());
			delete image;
			image = NULL;
			{m_bError = TRUE; return;}
		}

		//Stopwatch(1);

		//显示进度条
		CString str;
		str.Format("%d%% complete", i*100/m_nFileSum);
		bar.SetText(str);
		bar.StepIt();
		PeekAndPump();
		
		Convert(i);//转换
	}

	AfxMessageBox("转换完成");
	
}


//图像转换
//将文件类型为m_strSource的image图像
//转换为大小为m_nNewWidth*m_nNewHeight，类型为m_strConvert的图像，并保存。
BOOL CDlgCalibBatchImgReSize::Convert(int index)
{
	CString strSaveFileName;
	int len = m_ImgFilesPath[index].GetLength();
	int i,j;
	for (i = len-1; i >= 0; i--){
		if (m_ImgFilesPath[index][i] == '.'){
			strSaveFileName=m_ImgFilesPath[index].Left(i);
			break;
		}
	}
	CString strAddedExt,strTemp;
	strAddedExt.Format("-%dx%d.%s",m_nNewWidth,m_nNewHeight,m_strOutputExt);
	strTemp = strSaveFileName+strAddedExt;
	//strSaveFileName+=strAddedExt;

	if( (image->GetWidth()==(unsigned)m_nNewWidth)&&(image->GetHeight()==(unsigned)m_nNewHeight) )
	{
		bool retval;
		retval = image->Save(strTemp, m_nOutputType);
		if (retval) return TRUE;
		else
		{
			AfxMessageBox(image->GetLastError());
			return FALSE;
		}
	}
//	return FALSE;


	//如果宽高不一样。则进行双线性插值。
	int nWidth=image->GetWidth();
	int nHeight=image->GetHeight();

	double sx,sy;				//x和y方向的比例系数
	sx=double(m_nNewWidth)/(double)nWidth;
	sy=double(m_nNewHeight)/(double)nHeight;
	CxImage* imaNew = new CxImage(m_nNewWidth, m_nNewHeight,24);

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
				COLORREF color=image->RGBQUADtoRGB(image->GetPixelColor((int)x,(int)y));
				imaNew->SetPixelColor((int)i,(int)j,color);
				
			}
			else
			{
				double r1,r2,r3,r4;
				double g1,g2,g3,g4;
				double b1,b2,b3,b4;
				COLORREF c1,c2,c3,c4;
				c1=image->RGBQUADtoRGB(image->GetPixelColor((int)x,(int)y));
				c2=image->RGBQUADtoRGB(image->GetPixelColor((int)x+1,(int)y));
				c3=image->RGBQUADtoRGB(image->GetPixelColor((int)x,(int)y+1));
				c4=image->RGBQUADtoRGB(image->GetPixelColor((int)x+1,(int)y+1));
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
	
	delete image;
	image = imaNew;
	bool retval;
	retval = image->Save(strTemp, m_nOutputType);
	if (retval) return TRUE;
	else
	{
		AfxMessageBox(image->GetLastError());
		return FALSE;
	}
}


void SelectImageZone(CString imgFileName)
{
	CxImage *pOrigImg = new CxImage();
	pOrigImg->Load(imgFileName, CXIMAGE_FORMAT_BMP);
	int nHei = pOrigImg->GetHeight();
	int nWid = pOrigImg->GetWidth();

	CxImage *pNewImg = new CxImage(480,480,24,CXIMAGE_FORMAT_BMP);

	int i,j;
	int fromX = 581;
	int fromY = nHei - 480;
	RGBQUAD tempRGB;

	for(i = 0; i < 480; i++)
	{
		for (j = 0; j < 480; j++)
		{
			tempRGB = pOrigImg->GetPixelColor(j+fromX, i+fromY);
			pNewImg->SetPixelColor(j,i,tempRGB);
		}
	}

	CString strTemp;
	strTemp.Format("-%dx%d.%s",480,480,"bmp");
	strTemp = imgFileName+strTemp;

	pNewImg->Save(strTemp,CXIMAGE_FORMAT_BMP);

	delete pOrigImg;
	delete pNewImg;

}
