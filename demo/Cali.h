#ifndef CALI_H
#define CALI_H

#include"typedef.h"
#include "newmatrix.h"


//多片检校类：

class Calibration
{

public:
	Calibration();
	~Calibration();
	
///////////////////////////////////////////////////////////////
//	函 数 名 : SetImg
//	函数功能 : 设置相片的坐标数据：
//	处理过程 : 
//	备    注 : 
//	作    者 : xdh
//	时    间 : 2003年3月5日
//	返 回 值 : 
//	参数说明 : int nImgSum,
//				phIMAGE *pImgs
///////////////////////////////////////////////////////////////
   void  SetImgs(int nImgSum,phIMAGE *pImgs);

	
///////////////////////////////////////////////////////////////
//	函 数 名 : SetGcp
//	函数功能 :设置控制点坐标：
//	处理过程 : 
//	备    注 : 
//	作    者 : xdh
//	时    间 : 2003年3月5日
//	返 回 值 : 
//	参数说明 : int nGcpSum,
//				POINT3D *pGcps
///////////////////////////////////////////////////////////////
  void SetGcps(int nGcpSum,POINT3D *pGcps);


///////////////////////////////////////////////////////////
//	函 数 名 : Compute
//	函数功能 : 计算同形矩阵：
//	处理过程 : 
//	备    注 : 
//	作    者 : xdh
//	时    间 : 2003年3月5日
//	返 回 值 : 
//	参数说明 : int nImgIndex: 使用影像的序列号
///////////////////////////////////////////////////////////
	void ComputeH( );

///////////////////////////////////////////////////////////////
//	函 数 名 : ComputeFlFromH
//	函数功能 : 根据同形矩阵来解算内方位元素
//	处理过程 : 
//	备    注 : 
//	作    者 : xdh
//	时    间 : 2003年3月5日
//	返 回 值 : float
//	参数说明 : int nImgIndex：使用哪一个同形矩阵值
///////////////////////////////////////////////////////////////
	float ComputeFlFromH(STRU_LFromH  &m_HP);

	
///////////////////////////////////////////////////////////////
//	函 数 名 : ComputeVirtualCamera
//	函数功能 : 根据真实相机参数计算相应虚拟相机参数
//	处理过程 : 
//	备    注 : 
//	作    者 : SongBencong
//	时    间 : 2007年7月30日
//	返 回 值 : void
//	参数说明 : 输入参数：trueCamera   ---真实相机参数
//			   输出参数：virtualCamera---虚拟相机参数
///////////////////////////////////////////////////////////////
	void ComputeVirtualCamera(STRU_LFromH  &trueCamera,STRU_LFromH &virtualCamera);


	void TransformFromBasicCamera(CMatrix &RP, CMatrix &TP, double cubeline);


//data:
public:


	//图像点：
    int       m_nImgSum;
	phIMAGE	  *m_pImgs;

	//控制点：
    int       m_nGcpSum;
	POINT3D   *m_pGcps;  

	//拍摄面
	int m_surf;
//保存输出文件名称。
public:
	CString strFileName;
	int nWid;
	int nHei; 
};

#endif