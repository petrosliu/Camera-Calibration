#ifndef   _IMAGE_MATCH
#define   _IMAGE_MATCH

#include"typedef.h"

//#include"xdhbmp.h"

//class CXdhBmp

double *  LS( double *A, double *L,int m,int n);
BOOL ClosedFormH(double *A,int m,int n, double* &res);

//利用L系数来从地面坐标计算相片坐标：
void GrdToImg(POINT3D grd,POINT2D &img,const double *L=NULL);

//利用H解算得到的内外方位元素来从地面坐标计算相片坐标：
bool GrdToImg1(POINT3D grd,POINT2D &img,const STRU_LFromH HP);



/*

int      MatchUsingCoor(BYTE l_r_template[],BYTE* r_pData,
				   CRect rect_searching,CPoint *match_result,
				   int r_Height,int r_ScanWidth);

double   ComputeCoor(BYTE* lData ,CRect &rectL, int m_lScanWidth,
					 BYTE* rData ,CRect &rectR, int m_rScanWidth,
					 CPoint lPoint,CPoint rPoint,
					 int bitCount);

float	 GetCoor(BYTE *pL,BYTE *pR,int nHei,int nWid);

//松弛法匹配的简单实验：
void     RelaxationMatching();

//最小二乘的方法来计算出基础矩阵的值：
void     LSComputeFM(const double *lx,const double *ly,
					 const double *rx,const double *ry,
					 int   iNum,
					 double *res);

//最小二乘的方法来进行直接相对定向：

//input:
//    inner----内方位元素；
//	 xl-------
//	 yl-------
//	 xr-------
//	 yr-------
//   nPtSum---
//output:
//	 outer----外方位元素；
//
STRU_Rotation  RLT(InnerData inner,
				const double *xl,const double *yl,
				const double *xr,const double *yr,
				int   nPtSum,
				OuterData &outer);

POINT3D   Reconstruction(POINT2D lpt,POINT2D rpt,InnerData inner,STRU_Rotation R);




   
//	 利用角锥体法来解求外方位元素；


void  AngleCone(InnerData inner,
				const double *x,const double *y,
				const POINT3D *XYZ,
				int   nPtSum,
				OuterData &outer);


double *  LS( double *A, double *L,int m,int n);

void   GetLineParas(const EP *edgePts,const int nPtsSum,float  &A,float  &B,float &C);
void   GetLineParas1(const EP *edgePts,const int nPtsSum,float  &A,float  &B,float &C);


//using the ls method to simulate a plane
void   SimulatePlane(POINT3D *pts,int nPtSum);

//解算二维投影变换参数：
void SolveHomoTrans(const POINT2D *lPts, const POINT2D *rPts,int nPtSum,double *H);

//双线性的灰度内插公式：
COLORREF BiInterpolation(float cx,float cy,CXdhBmp &bmp);

COLORREF GetNbrMean(float cx,float cy,CXdhBmp &bmp);



//二维FFT2
//

bool FFT2(const STRU_COMPLEX *pS,int nWid,int nHei,int direction,STRU_COMPLEX *pD);
//一维FFT
//
//dir=1: direct  transform:傅立叶正变换；
//dir=0: inverse transform:傅立叶反变换；
//
bool FFT(int dir,int m,double *x,double *y);
//判断是否是2的幂
bool IsPowerof2(long x);

//利用L系数来从地面坐标计算相片坐标：
void GrdToImg(POINT3D grd,POINT2D &img,const double *L=NULL);

//return the width of overlap area :
int  GetWidOfOverlapArea(POINT3D grd,double fInterval,
						 double fcsLen, LP L);

//利用H解算得到的内外方位元素来从地面坐标计算相片坐标：
bool GrdToImg1(POINT3D grd,POINT2D &img,const STRU_LFromH HP);


//利用H系数来计算空间点到相机的距离：
float GetLenToCmr(POINT3D grd, const STRU_LFromH HP);

//利用H系数来计算空间点附近的一个点：
POINT3D GetExteriorPt(POINT3D grd, const STRU_LFromH HP);
*/

#endif