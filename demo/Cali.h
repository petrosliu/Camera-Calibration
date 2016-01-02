#ifndef CALI_H
#define CALI_H

#include"typedef.h"
#include "newmatrix.h"


//��Ƭ��У�ࣺ

class Calibration
{

public:
	Calibration();
	~Calibration();
	
///////////////////////////////////////////////////////////////
//	�� �� �� : SetImg
//	�������� : ������Ƭ���������ݣ�
//	������� : 
//	��    ע : 
//	��    �� : xdh
//	ʱ    �� : 2003��3��5��
//	�� �� ֵ : 
//	����˵�� : int nImgSum,
//				phIMAGE *pImgs
///////////////////////////////////////////////////////////////
   void  SetImgs(int nImgSum,phIMAGE *pImgs);

	
///////////////////////////////////////////////////////////////
//	�� �� �� : SetGcp
//	�������� :���ÿ��Ƶ����꣺
//	������� : 
//	��    ע : 
//	��    �� : xdh
//	ʱ    �� : 2003��3��5��
//	�� �� ֵ : 
//	����˵�� : int nGcpSum,
//				POINT3D *pGcps
///////////////////////////////////////////////////////////////
  void SetGcps(int nGcpSum,POINT3D *pGcps);


///////////////////////////////////////////////////////////
//	�� �� �� : Compute
//	�������� : ����ͬ�ξ���
//	������� : 
//	��    ע : 
//	��    �� : xdh
//	ʱ    �� : 2003��3��5��
//	�� �� ֵ : 
//	����˵�� : int nImgIndex: ʹ��Ӱ������к�
///////////////////////////////////////////////////////////
	void ComputeH( );

///////////////////////////////////////////////////////////////
//	�� �� �� : ComputeFlFromH
//	�������� : ����ͬ�ξ����������ڷ�λԪ��
//	������� : 
//	��    ע : 
//	��    �� : xdh
//	ʱ    �� : 2003��3��5��
//	�� �� ֵ : float
//	����˵�� : int nImgIndex��ʹ����һ��ͬ�ξ���ֵ
///////////////////////////////////////////////////////////////
	float ComputeFlFromH(STRU_LFromH  &m_HP);

	
///////////////////////////////////////////////////////////////
//	�� �� �� : ComputeVirtualCamera
//	�������� : ������ʵ�������������Ӧ�����������
//	������� : 
//	��    ע : 
//	��    �� : SongBencong
//	ʱ    �� : 2007��7��30��
//	�� �� ֵ : void
//	����˵�� : ���������trueCamera   ---��ʵ�������
//			   ���������virtualCamera---�����������
///////////////////////////////////////////////////////////////
	void ComputeVirtualCamera(STRU_LFromH  &trueCamera,STRU_LFromH &virtualCamera);


	void TransformFromBasicCamera(CMatrix &RP, CMatrix &TP, double cubeline);


//data:
public:


	//ͼ��㣺
    int       m_nImgSum;
	phIMAGE	  *m_pImgs;

	//���Ƶ㣺
    int       m_nGcpSum;
	POINT3D   *m_pGcps;  

	//������
	int m_surf;
//��������ļ����ơ�
public:
	CString strFileName;
	int nWid;
	int nHei; 
};

#endif