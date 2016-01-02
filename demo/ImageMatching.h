#ifndef   _IMAGE_MATCH
#define   _IMAGE_MATCH

#include"typedef.h"

//#include"xdhbmp.h"

//class CXdhBmp

double *  LS( double *A, double *L,int m,int n);
BOOL ClosedFormH(double *A,int m,int n, double* &res);

//����Lϵ�����ӵ������������Ƭ���꣺
void GrdToImg(POINT3D grd,POINT2D &img,const double *L=NULL);

//����H����õ������ⷽλԪ�����ӵ������������Ƭ���꣺
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

//�ɳڷ�ƥ��ļ�ʵ�飺
void     RelaxationMatching();

//��С���˵ķ�������������������ֵ��
void     LSComputeFM(const double *lx,const double *ly,
					 const double *rx,const double *ry,
					 int   iNum,
					 double *res);

//��С���˵ķ���������ֱ����Զ���

//input:
//    inner----�ڷ�λԪ�أ�
//	 xl-------
//	 yl-------
//	 xr-------
//	 yr-------
//   nPtSum---
//output:
//	 outer----�ⷽλԪ�أ�
//
STRU_Rotation  RLT(InnerData inner,
				const double *xl,const double *yl,
				const double *xr,const double *yr,
				int   nPtSum,
				OuterData &outer);

POINT3D   Reconstruction(POINT2D lpt,POINT2D rpt,InnerData inner,STRU_Rotation R);




   
//	 ���ý�׶�巨�������ⷽλԪ�أ�


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

//�����άͶӰ�任������
void SolveHomoTrans(const POINT2D *lPts, const POINT2D *rPts,int nPtSum,double *H);

//˫���ԵĻҶ��ڲ幫ʽ��
COLORREF BiInterpolation(float cx,float cy,CXdhBmp &bmp);

COLORREF GetNbrMean(float cx,float cy,CXdhBmp &bmp);



//��άFFT2
//

bool FFT2(const STRU_COMPLEX *pS,int nWid,int nHei,int direction,STRU_COMPLEX *pD);
//һάFFT
//
//dir=1: direct  transform:����Ҷ���任��
//dir=0: inverse transform:����Ҷ���任��
//
bool FFT(int dir,int m,double *x,double *y);
//�ж��Ƿ���2����
bool IsPowerof2(long x);

//����Lϵ�����ӵ������������Ƭ���꣺
void GrdToImg(POINT3D grd,POINT2D &img,const double *L=NULL);

//return the width of overlap area :
int  GetWidOfOverlapArea(POINT3D grd,double fInterval,
						 double fcsLen, LP L);

//����H����õ������ⷽλԪ�����ӵ������������Ƭ���꣺
bool GrdToImg1(POINT3D grd,POINT2D &img,const STRU_LFromH HP);


//����Hϵ��������ռ�㵽����ľ��룺
float GetLenToCmr(POINT3D grd, const STRU_LFromH HP);

//����Hϵ��������ռ�㸽����һ���㣺
POINT3D GetExteriorPt(POINT3D grd, const STRU_LFromH HP);
*/

#endif