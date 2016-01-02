

#include"stdafx.h"
#include<math.h>

#include"ImageMatching.h"
#include"Matrix.h"
#include"MatrixApp.h"
#include"NewMatrix.h"

double* LS(double *A,double *L,int m,int n)
{
	double *At=new double[n*m];
	double *AtA=new double[n*n];
	double *AtL=new double[n];

	transpose(A,At,m,n);
	mult(At,A,AtA,n,m,n);
	invers_matrix(AtA,n);
	mult(At,L,AtL,n,m,1);
	
	double *res=new double[n];
	mult(AtA,AtL,res,n,n,1);
	
	delete []At;
	delete []AtA;
	delete []AtL;

	return res;
}

BOOL ClosedFormH(double *A,int m,int n, double* &res)//解析解H
{
	double *At=new double[n*m];
	double *AtA=new double[n*n];
	
	transpose(A,At,m,n);
	mult(At,A,AtA,n,m,n);
	
	////////////////////////////
	CMatrix mtx_ATA(n, n, AtA);
	CMatrix eigenv_ATA;
	double* dblV = new double[n];//雅克比法
	
	int times=1;
	BOOL yes=FALSE;
	while(!yes)
	{
		mtx_ATA.SetData(AtA);
		yes=mtx_ATA.JacobiEigenv(dblV, eigenv_ATA, 100*times, 0.000001*times);
		times++;
	}
	///////////////////////////最小特征值 （多余步骤？）
	double temp=dblV[0];
	for(int i=1;i<n;i++)
	{
		if(temp<dblV[i])
			temp=dblV[i];
	}
	
	int index=0;
	for(i=0;i<n;i++)
	{
		if(dblV[i]<temp)
		{
			temp=dblV[i];
			index=i;
		}
	}
	
	/////////////////////////
	eigenv_ATA.GetColVector(index,res);//返回ATA最小特征值的特征向量
	//判断正负
	if((res[6]>0 && res[7]>0 && res[8]<0) ||
	   (res[6]<0 && res[7]>0 && res[8]<0) ||
	   (res[6]<0 && res[7]<0 && res[8]<0) ||
	   (res[6]>0 && res[7]<0 && res[8]<0))
	{
		for(i=0;i<n;i++)
		{
			res[i]*=-1.0;
		}
	}
	////////////////////////
	delete []dblV;
	delete []At;
	delete []AtA;
	return TRUE;
}

bool GrdToImg1(POINT3D grd,POINT2D &img, STRU_LFromH HP)
{
	float f=0;
	float r11=0,r12=0,r13=0,r21=0,r22=0,r23=0,r31=0,r32=0,r33=0;
	float tx=0,ty=0,tz=0;

	f=HP.f;
	float k=1;

	//interior matrix:
	double A[3][3];
	for(int j=0;j<3;j++)
		for(int i=0;i<3;i++)
			A[j][i]=0;

	A[0][0]=f;	A[1][1]=k*f; A[2][2]=1;
	
	//exterior elements:
	r11=HP.r11;	r12=HP.r12;	r13=HP.r13;
	r21=HP.r21;	r22=HP.r22;	r23=HP.r23;
	r31=HP.r31;	r32=HP.r32;	r33=HP.r33;

	tx=HP.tx;ty=HP.ty;tz=HP.tz;

	double R[3][3];
	double T[3];

	R[0][0]=r11;R[0][1]=r12;R[0][2]=r13;
	R[1][0]=r21;R[1][1]=r22;R[1][2]=r23;
	R[2][0]=r31;R[2][1]=r32;R[2][2]=r33;

	T[0]=tx;T[1]=ty;T[2]=tz;

	///////////////////////////////////////
	double X[3];
	X[0]=grd.x;	X[1]=grd.y; X[2]=grd.z;
	double dfT[3];
	mult(*R,X,dfT,3,3,1);

	dfT[0]=dfT[0]+T[0];
	dfT[1]=dfT[1]+T[1];
	dfT[2]=dfT[2]+T[2];

	////////////////////////
	double dfT1[3];
	mult(*A,dfT,dfT1,3,3,1);

	double x,y;
	x=dfT1[0]/dfT[2];
	y=dfT1[1]/dfT[2];

	int nWid=HP.wid;
	int nHei=HP.hei;
	img.x= x+nWid/2;
	img.y= y+nHei/2;
	
	return true;
}


void GrdToImg(POINT3D grd,POINT2D &img,const double *L)
{

	double dX,dY,dZ;
	double dIx,dIy;
	
	dX=grd.x;
	dY=grd.y;
	dZ=grd.z;

	dIx=(L[0]*dX+L[1]*dY+L[2]*dZ+L[3])/(L[8]*dX+L[9]*dY+L[10]*dZ+1);
	dIy=(L[4]*dX+L[5]*dY+L[6]*dZ+L[7])/(L[8]*dX+L[9]*dY+L[10]*dZ+1);
		
	img.x=dIx;
	img.y=dIy;
}




/*

//相关系数法：
//输入为左片的模板，右片的数据地址，右片的搜索区域，右片的高度和宽度；

int MatchUsingCoor(BYTE l_r_template[],BYTE* r_pData,CRect rect_searching,CPoint *match_result,int r_Height,int r_ScanWidth)
{


    double coor_v;

	int i,j;
    int ii,jj;
    //左片模板的均值：
	double l_mean;
	double l_sum=0;
	for(i=0;i<49;i++)
       l_sum=l_sum+l_r_template[i];
	
	l_mean=l_sum/49;
	
	
	int left,right,top,bottom;

	CPoint ltp;
	ltp=rect_searching.TopLeft();
	left=ltp.x;
	top=ltp.y;

	CPoint rbp;
	rbp=rect_searching.BottomRight();
    right=rbp.x;
    bottom=rbp.y;


	BYTE r_coor_area[49][3]={0};
	BYTE r_r_template[49]={0};

	int n=0;
    int ri,rj;
	BYTE* pMove;

	//每做一次要重新赋值：
	int match_num=0;

	for(j=top+3;j<bottom-3;j++)
		for(i=left+3;i<right-3;i++)
		{
			ri=i;
			rj=j;
			
			n=0;
			for(ii=-3;ii<=3;ii++)
				//for(jj=-3;jj<=3;jj++)
			{
			 //每次拷贝7个像素的值：
			 pMove=    r_pData + 
				 ( r_Height - (rj+ii)-1 )*r_ScanWidth +
				 (ri-3)*3;
			 memcpy( *r_coor_area+n*21 ,pMove,21);
			 n++;
			}
			BYTE r_r_template[49];
			//只取一个波段：
			for ( jj=0;jj<49;jj++)
				r_r_template[jj]=r_coor_area[jj][0];
			
			

            //计算相关系数：
            //右片模板的均值：
			double r_mean;
			double r_sum=0;
            for(ii=0;ii<49;ii++)
                 r_sum=r_sum+r_r_template[ii];
			r_mean=r_sum/49;
            
			double numerator=0;//分子
			double l_std2=0;//左片方差
            double r_std2=0;//右片方差

			for(ii=0;ii<49;ii++)
			{
				numerator=numerator+(l_r_template[ii]-l_mean)*
					(r_r_template[ii]-r_mean);
				l_std2=l_std2+(l_r_template[ii]-l_mean)*(l_r_template[ii]-l_mean);
				r_std2=r_std2+(r_r_template[ii]-r_mean)*(r_r_template[ii]-r_mean);
			}
				
		coor_v=numerator/sqrt(l_std2*r_std2);

		if(coor_v>0.8)
		{
			match_result[match_num].x=i;
			match_result[match_num].y=j;

            match_num++;

		}
 

		}

return match_num;


}


//版本2：输入为左右点的坐标以及左右片的数据区地址；
//		 输出为相关系数的值；

double   ComputeCoor(BYTE* lData ,CRect &rectL,int m_lScanWidth, 
					 BYTE* rData ,CRect &rectR,int m_rScanWidth,
					 CPoint lPoint,CPoint rPoint,
					 int    bitCount )
{ 

	int ii,jj;
	int m_lWidth,m_lHeight;
	int m_rWidth,m_rHeight;
	double coor_v;
    m_lWidth=rectL.Width();
	m_lHeight=rectL.Height();
	m_rWidth=rectR.Width();
	m_rHeight=rectR.Height();
    //左片模板，模板取为 7*7：
    //左片特征点的像素坐标：
	int li,lj;
	li=lPoint.x;
	lj=lPoint.y;

    if(bitCount==24)
	{
			//变为一维来处理：
			BYTE l_corr_area[49][3]={0};
			BYTE* pMove;

			int n=0;
			for(ii=-3;ii<=3;ii++)
				//for(jj=-3;jj<=3;jj++)
			{
			 //每次拷贝7个像素的值：
			 pMove=    (lData + 
				 ( m_lHeight - (lj+ii)-1 )*m_lScanWidth+
				 (li-3)*3 );
			 memcpy( *l_corr_area+n*21 ,pMove,21);
			 n++;
			}
		   float l_template[49];
		   //取三个波段的均值：
		   for ( jj=0;jj<49;jj++)
				l_template[jj]=(float)( l_corr_area[jj][0] + l_corr_area[jj][1] + l_corr_area[jj][2] )/3;

		   //右片模板：
			n=0; 
			int ri,rj;
			ri=rPoint.x;
			rj=rPoint.y;
			//变为一维来处理：
			BYTE r_corr_area[49][3]={0};
			for(ii=-3;ii<=3;ii++)
				//for(jj=-3;jj<=3;jj++)
			{
			 //每次拷贝7个像素的值：
			 pMove=    (rData + 
				 ( m_rHeight - (rj+ii)-1 )*m_rScanWidth+
				 (ri-3)*3 );
			 memcpy( *r_corr_area+n*21 ,pMove,21);
			 n++;
			}
		    float r_template[49];
		   //取三个波段的均值：
		   for ( jj=0;jj<49;jj++)
				r_template[jj]=(float)( r_corr_area[jj][0] + r_corr_area[jj][1] + r_corr_area[jj][2] )/3;


		//计算相关系数
   
			int i;
		   //左片模板的均值：

			double l_mean;
			double l_sum=0;
			for(i=0;i<49;i++)
			   l_sum=l_sum+l_template[i];
			
			l_mean=l_sum/49;

		   //右片模板的均值：
			double r_mean;
			double r_sum=0;
			for(ii=0;ii<49;ii++)
				 r_sum=r_sum+r_template[ii];
			r_mean=r_sum/49;
    

			double numerator=0;//分子
			double l_std2=0;//左片方差
			double r_std2=0;//右片方差

			for(ii=0;ii<49;ii++)
			{
				numerator=numerator+(l_template[ii]-l_mean)*
					(r_template[ii]-r_mean);
				l_std2=l_std2+(l_template[ii]-l_mean)*(l_template[ii]-l_mean);
				r_std2=r_std2+(r_template[ii]-r_mean)*(r_template[ii]-r_mean);
			}
						
				
			double coor_v=numerator/sqrt(l_std2*r_std2);

	}
	else
	{	    //变为一维来处理：
			//BYTE l_corr_area[49]={0};
			BYTE l_template[49]={0};
			BYTE* pMove;
			int n=0;
			for(ii=-3;ii<=3;ii++)
				//for(jj=-3;jj<=3;jj++)
			{
			 //每次拷贝7个像素的值：
			 pMove=    (lData + 
				 ( m_lHeight - (lj+ii)-1 )*m_lScanWidth+
				 (li-3) );
			 memcpy( l_template+n*7 ,pMove,7);
			 n++;
			}
 

		  //右片模板：

			n=0; 
			int ri,rj;
			ri=rPoint.x;
			rj=rPoint.y;
			//变为一维来处理：
			//BYTE r_corr_area[49][3]={0};
			BYTE r_template[49]={0};

			for(ii=-3;ii<=3;ii++)
				//for(jj=-3;jj<=3;jj++)
			{
			 //每次拷贝7个像素的值：
			 pMove=    (rData + 
				 ( m_rHeight - (rj+ii)-1 )*m_rScanWidth+
				 (ri-3) );
			 memcpy( r_template+n*7 ,pMove,7);
			 n++;

			}


		//计算相关系数
   
			int i;
		   //左片模板的均值：

			double l_mean;
			double l_sum=0;
			for(i=0;i<49;i++)
			   l_sum=l_sum+l_template[i];
			
			l_mean=l_sum/49;

		   //右片模板的均值：
			double r_mean;
			double r_sum=0;
			for(ii=0;ii<49;ii++)
				 r_sum=r_sum+r_template[ii];
			r_mean=r_sum/49;
    

			double numerator=0;//分子
			double l_std2=0;//左片方差
			double r_std2=0;//右片方差

			for(ii=0;ii<49;ii++)
			{
				numerator=numerator+(l_template[ii]-l_mean)*
					(r_template[ii]-r_mean);
				l_std2=l_std2+(l_template[ii]-l_mean)*(l_template[ii]-l_mean);
				r_std2=r_std2+(r_template[ii]-r_mean)*(r_template[ii]-r_mean);
			}
					
			coor_v=numerator/sqrt(l_std2*r_std2);
	}
		return  coor_v;
}

//计算相关系数：

float GetCoor(BYTE *pL,BYTE *pR,int nHei,int nWid){


	//get the average value:
	float fLAvarage=0;
	float fRAvarage=0;
	
	for(int i=0;i<nHei*nWid;i++){

		fLAvarage=fLAvarage+pL[i];
		fRAvarage=fRAvarage+pR[i];
	}
	fLAvarage=fLAvarage/(nHei*nWid);
	fRAvarage=fRAvarage/(nHei*nWid);

	float Clr=0,Cll=0,Crr=0;
	for(i=0;i<nHei*nWid;i++){

		Clr=Clr+( pL[i]-fLAvarage )*( pR[i]-fRAvarage );
		Cll=Cll+( pL[i]-fLAvarage )*( pL[i]-fLAvarage );
		Crr=Crr+( pR[i]-fRAvarage )*( pR[i]-fRAvarage );
	}
	
	float fCoor=Clr/ ( sqrt(Cll)*sqrt(Crr) ); 
	return fCoor;
}


//松弛法的一个简单实验：
//必须使用，实践来获得感性的认识：

void     RelaxationMatching()
{
 
	CPoint* lp=new CPoint[6];
	CPoint* rp=new CPoint[5];

	lp[0]=CPoint(12,12);
	lp[1]=CPoint(10,10);
	lp[2]=CPoint(8,8);
	lp[3]=CPoint(8,6);
	lp[4]=CPoint(10,4);
	lp[5]=CPoint(6,2);

	rp[0]=CPoint(11,11);
	rp[1]=CPoint(7,8);
	rp[2]=CPoint(7,6);
	rp[3]=CPoint(9,4);
	rp[4]=CPoint(5,3);


	int ii,jj;
	//初始概率：
    float prob[6][5]={0};
	for(ii=0;ii<6;ii++)
		for(jj=0;jj<5;jj++)
			prob[ii][jj]=0.5;


	//
	float newProb[6][5]={0};

	int oi;//目标的索引；
	int cj;//类别的索引；

	float a=0.2;


	oi=0;
	cj=0;

for(int iter=0;iter<10;iter++)
{
   oi=0;
   while(oi<6)
   {
       cj=0;

	   while(cj<5)
	{

//
//计算一种匹配的qij:
		CPoint VOiCj=lp[oi]-rp[cj];
		//Oi--Cj初始的概率：
		float pOiCj=prob[oi][cj];
		//邻域对该匹配对的支持系数：
		float qOiCj=0;
		
		for(ii=0;ii<6;ii++)
				for(jj=0;jj<5;jj++)
				{

					int h=ii;
					int k=jj;

					CPoint Vhk;
					Vhk=lp[h]-rp[k];
					float phk;//概率
					phk=prob[h][k];


					CPoint v;
					v=VOiCj-Vhk;
                    
					float dv=0;
					dv=sqrt(v.x*v.x+v.y*v.y);

					if(dv<=3)
					{
						qOiCj=qOiCj+(1/(a+dv))*phk;
					}


				}
                    
//


        //计算norm值：
		float norm=0;

		for(int jj=0;jj<5;jj++)//共有5个类别
		{
    
			//q(i,j):
			int i=oi;
			int j=jj;

			CPoint  Vij;
			Vij=lp[i]-rp[j];
			//
			float pij=0;
			pij=prob[i][j];

			//对每种匹配搭配计算一个qij:
			//需要利用邻域的信息：
			float qij=0;
			//选择邻域的点：
			for(int m=0;m<6;m++)
				for(int n=0;n<5;n++)
				{

					int h=m;
					int k=n;
					CPoint Vhk;
					Vhk=lp[h]-rp[k];
					float phk;//概率
					phk=prob[h][k];

					CPoint v;
					v=Vij-Vhk;
                    
					float dv=0;
					dv=sqrt(v.x*v.x+v.y*v.y);
					
					if(dv<=3)
					{
						qij=qij+(1/(a+dv))*phk;
					}

				}

			norm=norm+prob[i][j]*(1+qij);

		}

		//计算新的概率值：
        newProb[oi][cj]=prob[oi][cj]*(1+qOiCj)/norm;
	cj++;
	}
	oi++;
   }

   for(int k=0;k<6;k++)
	   for(int kk=0;kk<5;kk++)
		   prob[k][kk]=newProb[k][kk];

}

        

}



void     LSComputeFM(const double *lx,const double *ly,
					 const double *rx,const double *ry,
					 int   iSum,
					 double *F)
{
	
	int i;
   	double *A=new double[iSum*8];
	memset(A,0,sizeof(double)*iSum*8);
	double *b=new double[iSum];
	double ltx,lty,rtx,rty;

	int ni=0;

	for(i=0;i<iSum;i++){
                
			ltx=lx[i];
			lty=ly[i];
			rtx=rx[i];
			rty=ry[i];
						
			A[i*8+0]=ltx*rtx;
			A[i*8+1]=lty*rtx;
			A[i*8+2]=rtx;
			A[i*8+3]=ltx*rty;
			A[i*8+4]=lty*rty;
			A[i*8+5]=rty;
			A[i*8+6]=ltx;
			A[i*8+7]=lty;
			b[i]=-1;
	}


    //计算广义逆：
	int m,n,ka;
	m=iSum;
	n=8;
	ka=iSum+1;
   	double eps=0.000001;
	double x[8];
	double *aa =new double[n*m];
	double *uu =new double[m*m];
	double *vv =new double[n*n];
	i=agmiv(A,m,n,b,x,aa,eps,uu,vv,ka);

	if(A!=NULL) delete A;
	if(b!=NULL) delete b;
	delete aa;
	delete uu;
	delete vv;

    //输出结果:
	double FM[3][3];
    FM[0][0]=x[0];
	FM[1][0]=x[1];
	FM[2][0]=x[2];
	FM[0][1]=x[3];
	FM[1][1]=x[4];
	FM[2][1]=x[5];
    FM[0][2]=x[6];
	FM[1][2]=x[7];
	FM[2][2]=1;

    //基础矩阵的秩应该为2，需要进行强行的约束：
	//对FM进行奇异值的分解：
	double u[3][3]={0},v[3][3]={0};
	//double eps;
	eps=0.000000000001;
	int returnv;
	returnv=bmuav(*FM,3,3,*u,*v,eps,4);
	 
	 double ss[3][3]={0};
	 
	 for(int j=0;j<3;j++)
		 for(i=0;i<3;i++)
			 ss[i][j]=0;
		 
		 ss[0][0]=FM[0][0];
		 ss[1][1]=FM[1][1];
		 
		 double temp1[3][3]={0};
		 mult(*u,*ss,*temp1,3,3,3);
		 //mult(*temp1,*v,F,3,3,3);
		 mult(*temp1,*v,*FM,3,3,3);
		 for(int ii=0;ii<3;ii++)
		 {  
			 TRACE("\n"); 
			 for(int jj=0;jj<3;jj++)
				 TRACE(" %f ",FM[ii][jj]);
		 }
		
		 F[0]=FM[0][0];
		 F[1]=FM[1][0];
		 F[2]=FM[2][0];
		 F[3]=FM[0][1];
		 F[4]=FM[1][1];
		 F[5]=FM[2][1];
		 F[6]=FM[0][2];
		 F[7]=FM[1][2];
		 F[8]=FM[2][2];
}


STRU_Rotation    RLT(InnerData inner,
				const double *xl,
				const double *yl,
				const double *xr,
				const double *yr,
				int   nPtSum,
				OuterData &outer)
{

	double *nxl=new double[nPtSum];
	double *nyl=new double[nPtSum];
	double *nxr=new double[nPtSum];
	double *nyr=new double[nPtSum];
	
	int i;

	//利用内方位元素来转换为归一化坐标：
	for( i=0;i<nPtSum;i++)
	{
		nxl[i]=xl[i]-inner.x0;
		nyl[i]=-(yl[i]-inner.y0);
		nxr[i]=xr[i]-inner.x0;
		nyr[i]=-(yr[i]-inner.y0);
	}
	////////////////////////////////////
	int m,n,ka;
	m=nPtSum;
	n=8;
	ka=nPtSum+1;
	double *a=NULL;
	a=new double[n*m];
	double *u=NULL;
	u=new double[m*m];
	double *v=NULL;
	v=new double[n*n];

	double *A=new double[nPtSum*8];
	double *L=new double[nPtSum];
	double txl,tyl,txr,tyr;

	double f=inner.f;
	
	for(i=0;i<nPtSum;i++)
	{
		txl=nxl[i];
		tyl=nyl[i];
		txr=nxr[i];
		tyr=nyr[i];
		
		A[i*8]  = tyl*txr;
		A[i*8+1]= tyl*tyr;
		A[i*8+2]=-tyl*f;
		A[i*8+3]= txr*f;
		A[i*8+4]=-f*f;
		A[i*8+5]= txl*txr;
		A[i*8+6]= txl*tyr;
		A[i*8+7]=-txl*f;
		
		L[i]=-tyr*f;
	}


	//计算广义逆,用来解超定方程
   	double eps=0.000001;
	double x[8];
	i=agmiv(A,m,n,L,x,a,eps,u,v,ka);

	//double *rL=new double[nPtSum];
	//mult(A,x,rL,m,n,1);//A矩阵的值会改变；
//	最小二乘的方法：
//	double *x;
//	x=LS(A,L,m,n);
//	double *LsrL=new double[nPtSum];
//	mult(A,x,LsrL,m,n,1);
//	
	//if(A!=NULL) delete A;
	//if(L!=NULL) delete L;

	delete a;
	delete u;
	delete v;
	delete nxl;
	delete nyl;
	delete nxr;
	delete nyr;
	/////////////////////////////////////
	double L1,L2,L3,L4,L5,L6,L7,L8,L9;
	double Bx=100;
	
	//when L5>0;
	L5=sqrt( (2*Bx*Bx)/ ( x[0]*x[0]+x[1]*x[1]+x[2]*x[2]+x[3]*x[3]+1+x[4]*x[4]-x[5]*x[5]-x[6]*x[6]-x[7]*x[7] )   );

	L1=x[0]*L5;
	L2=x[1]*L5;
	L3=x[2]*L5;
	L4=x[3]*L5;
	L6=x[4]*L5;
	L7=x[5]*L5;
	L8=x[6]*L5;
	L9=x[7]*L5;
	
	double By=( -(L1*L7+L2*L8+L3*L9)/Bx );
	double Bz=(  (L4*L7+L5*L8+L6*L9)/Bx );
	
	double a1=(L3*L5-L6*L2-Bz*L1-By*L4)/(Bx*Bx+By*By+Bz*Bz);
	double b1=(By*a1+L4)/Bx;
	double c1=(Bz*a1+L1)/Bx;
	
	double a2=(L1*L6-L3*L4-Bz*L2-By*L5)/(Bx*Bx+By*By+Bz*Bz);
	double b2=(By*a2+L5)/Bx;
	double c2=(Bz*a2+L2)/Bx;

	double a3=(L2*L4-L1*L5-Bz*L3-By*L6)/(Bx*Bx+By*By+Bz*Bz);
	double b3=(By*a3+L6)/Bx;
	double c3=(Bz*a3+L3)/Bx;



	FILE *fp=fopen("d:\\RLT_Rotation.txt","w");
	
	fprintf(fp,"when L5>0 \n");

	fprintf(fp,"%lf %lf %lf \n",a1,b1,c1);
	fprintf(fp,"%lf %lf %lf \n",a2,b2,c2);
	fprintf(fp,"%lf %lf %lf \n",a3,b3,c3);

	//ofstream file("d:\\rotation.txt");
	//    file<<a1<<" "<<a2<<" "<<a3<<endl;
	//    file<<b1<<" "<<b2<<" "<<b3<<endl;
	//    file<<c1<<" "<<c2<<" "<<c3<<endl;
	
	//以x轴为主轴的转轴系统：
	double faita,omiga,kapa ;
	faita=asin(-a3);
	omiga=atan(-b3/c3);
	kapa=atan(-a2/a1);

	//when L5<0;
	L5=-L5;
	L1=x[0]*L5;
	L2=x[1]*L5;
	L3=x[2]*L5;
	L4=x[3]*L5;
	L6=x[4]*L5;
	L7=x[5]*L5;
	L8=x[6]*L5;
	L9=x[7]*L5;
		
	double fBy=( -(L1*L7+L2*L8+L3*L9)/Bx );
	double fBz=(  (L4*L7+L5*L8+L6*L9)/Bx );
	
	double fa1=(L3*L5-L6*L2-fBz*L1-fBy*L4)/(Bx*Bx+fBy*fBy+fBz*fBz);
	double fb1=(fBy*fa1+L4)/Bx;
	double fc1=(fBz*fa1+L1)/Bx;
	double fa2=(L1*L6-L3*L4-fBz*L2-fBy*L5)/(Bx*Bx+fBy*fBy+fBz*fBz);
	double fb2=(fBy*fa2+L5)/Bx;
	double fc2=(fBz*fa2+L2)/Bx;
	double fa3=(L2*L4-L1*L5-fBz*L3-fBy*L6)/(Bx*Bx+fBy*fBy+fBz*fBz);
	double fb3=(fBy*fa3+L6)/Bx;
	double fc3=(fBz*fa3+L3)/Bx;
	double _faita,_omiga,_kapa ;

	_faita=asin( fa3);
	_omiga=atan(-fb3/fc3);
	_kapa= atan(-fa2/fa1);

	fprintf(fp,"when L5<0 \n");
	fprintf(fp,"%lf %lf %lf \n",fa1,fb1,fc1);
	fprintf(fp,"%lf %lf %lf \n",fa2,fb2,fc2);
	fprintf(fp,"%lf %lf %lf \n",fa3,fb3,fc3);

	fprintf(fp,"Bx=%lf  By=%lf Bz=%lf \n",Bx,By,Bz);
	

	fclose(fp);


	STRU_Rotation  R;
	R.Bx=Bx;  R.By=By;  R.Bz=Bz;


	

	if(c3>0)
	{

		R.a1=a1;  R.a2=a2;  R.a3=a3;
		R.b1=b1;  R.b2=b2;  R.b3=b3;
		R.c1=c1;  R.c2=c2;  R.c3=c3;

	}
	else
	{
		R.a1=fa1;  R.a2=fa2;  R.a3=fa3;
		R.b1=fb1;  R.b2=fb2;  R.b3=fb3;
		R.c1=fc1;  R.c2=fc2;  R.c3=fc3;
	}
	
	return R;
}


POINT3D   Reconstruction(POINT2D lpt,POINT2D rpt,InnerData inner,STRU_Rotation R)
{
	 double a1,a2,a3,b1,b2,b3,c1,c2,c3;
	 a1=R.a1; a2=R.a2; a3=R.a3;
	 b1=R.b1; b2=R.b2; b3=R.b3;
	 c1=R.c1; c2=R.c2; c3=R.c3;
	 
	 double Bx,By,Bz;
	 Bx=R.Bx;  By=R.By;  Bz=R.Bz;
	 
	 float f=inner.f;
	 float x0=inner.x0;
	 float y0=inner.y0;

	 double lx,ly,rx,ry;
	 lx=lpt.x-x0;
	 ly=-(lpt.y-y0);
	 rx=rpt.x-x0;
	 ry=-(rpt.y-y0);

	 double fx,fy,fz;
	 fx=a1*rx+a2*ry-a3*f;
	 fy=b1*rx+b2*ry-b3*f;
	 fz=c1*rx+c2*ry-c3*f;

	 double A[4][3];
	 double L[4];

	 A[0][0]=fz;	 A[0][1]=0;	 A[0][2]=-fx;
	 A[1][0]=0;	     A[1][1]=fz; A[1][2]=-fy;
	 A[2][0]=f;	     A[2][1]=0;	 A[2][2]=lx;
	 A[3][0]=0;	     A[3][1]=f;	 A[3][2]=ly;

	 L[0]=Bx*fz-Bz*fx;
	 L[1]=By*fz-Bz*fy;
	 L[2]=0;
	 L[3]=0;

	 double res[3];
	 
	 double At[3][4],AtA[3][3],AtL[3];

	 transpose(*A,*At,4,3);
     mult(*At,*A,*AtA,3,4,3);
	 inv(*AtA,3);
	 mult(*At,L,AtL,3,4,1);
	 mult(*AtA,AtL,res,3,3,1);

	 POINT3D XYZ;
	 XYZ.x=res[0];
	 XYZ.y=res[1];
	 XYZ.z=res[2];
    
	 return XYZ;
}


void  AngleCone(InnerData inner,
				const double *x,const double *y,
				const POINT3D *XYZ,
				int   nPtSum,
				OuterData &outer)
{	
	double f=inner.f;
	double x0=inner.x0;
	double y0=inner.y0;
   
	//坐标的归一化：
	int i,j;
	int nSum=0;
	for(i=0;i<nPtSum;i++)
		nSum=nSum+i;

	double *A=new double[nSum*3];
	double *L=new double[nSum];
	
	double  dfXi,dfYi,dfZi,dfXj,dfYj,dfZj;
	double  dfxi,dfyi,dfxj,dfyj;

	double si,sj;
	double SI,SJ;

	double X0=0,Y0=0,Z0=0;
    
	//控制点的两两组合：
	int nIndex=0;
	for(i=0;i<nPtSum;i++)
		for(j=i+1;j<nPtSum;j++)
		{   						
			dfxi=x[i];	   dfyi=y[i];
			dfXi=XYZ[i].x; dfYi=XYZ[i].y; dfZi=XYZ[i].z;
			dfxj=x[j];     dfyj=y[j];
			dfXj=XYZ[j].x; dfYj=XYZ[j].y; dfZj=XYZ[j].z;

			si=x[i]*x[i]+y[i]*y[i]+f*f;
			sj=x[j]*x[j]+y[j]*y[j]+f*f;
			SI=(X0-dfXi)*(X0-dfXi)+
				(Y0-dfYi)*(Y0-dfYi)+(Z0-dfZi)*(Z0-dfZi);
			SI=(X0-dfXj)*(X0-dfXj)+
				(Y0-dfYj)*(Y0-dfYj)+(Z0-dfZj)*(Z0-dfZj);
			
			double cosij=(dfxi*dfxj+dfyi*dfyj+f*f)/(si*sj);
			double cosIJ=( (X0-dfXi)*(X0-dfXj)+
						 (Y0-dfYi)*(Y0-dfYj)+
						 (Z0-dfZi)*(Z0-dfZj) )/(SI*SJ);

			double Tji=1/(SI*SJ)*( 1-(SI/SJ)*cosij );
			double Tij=1/(SI*SJ)*( 1-(SJ/SI)*cosij );

			double aij=(X0-dfXi)*Tji+(X0-dfXj)*Tij;
			double bij=(Y0-dfYi)*Tji+(Y0-dfYj)*Tij;
			double cij=(Z0-dfZi)*Tji+(Z0-dfZj)*Tij;

			A[nIndex*3]=aij;
			A[nIndex*3+1]=bij;
			A[nIndex*3+2]=cij;

			L[nIndex]=cosij-cosIJ;
		}
}


double* LS(double *A,double *L,int m,int n)
{
	double *At=new double[n*m];
	double *AtA=new double[n*n];
	double *AtL=new double[n];

	transpose(A,At,m,n);
	mult(At,A,AtA,n,m,n);
	invers_matrix(AtA,n);
	mult(At,L,AtL,n,m,1);
	
	double *res=new double[n];
	mult(AtA,AtL,res,n,n,1);
	
	delete At;
	delete AtA;
	delete AtL;

	return res;
}



//
//  功能：用最小二乘法来拟合出一条直线;
//  输入：edgePts------点的坐标；
//        nPtsSum------点数；
//  输出：A,B,C--------直线系数；

void    GetLineParas(const EP *edgePts,const int nPtsSum,float  &A,float  &B,float &C)
{
         double *M=  new double[nPtsSum*2];
		 double *Mt= new double[nPtsSum*2];
		 double *MtM=new double[2*2];
		 double *L=  new double[nPtsSum];
		 double *MtL=new double[2]; 
		 double res[2];
		 
		 for(int i=0;i<nPtsSum;i++)
		 {

			 M[2*i]  =edgePts[i].x;
			 M[2*i+1]=1;
			 L[i]=edgePts[i].y;
		 }

		 transpose(M,Mt,nPtsSum,2);
		 mult(Mt,M,MtM,2,nPtsSum,2);
		 mult(Mt,L,MtL,2,nPtsSum,1);
		 inv(MtM,2);
		 mult(MtM,MtL,res,2,2,1);
		 
		 A=res[0];
		 B=-1;
		 C=res[1];
		 
		 //计算残差：
}

void    GetLineParas1(const EP *edgePts,const int nPtsSum,float  &A,float  &B,float &C)
{

		 double *M=  new double[nPtsSum*2];
		 double *Mt= new double[nPtsSum*2];
		 double *MtM=new double[2*2];
		 double *L=  new double[nPtsSum];
		 double *MtL=new double[2]; 
		 double res[2];
		 
		 for(int i=0;i<nPtsSum;i++)
		 {

			 M[2*i]  =edgePts[i].x;
			 M[2*i+1]=edgePts[i].y;
			 L[i]=-1;
		 }

		 transpose(M,Mt,nPtsSum,2);
		 mult(Mt,M,MtM,2,nPtsSum,2);
		 mult(Mt,L,MtL,2,nPtsSum,1);
		 inv(MtM,2);
		 
		 mult(MtM,MtL,res,2,2,1);
		 
		 A=res[0];
		 B=res[1];
		 C=1;
		 
		 //计算残差：
}

void    SimulatePlane(POINT3D *pts,int nPtSum)
{
	//aX+bY+c=-Z;
	int i;
	double *A=new double[nPtSum*3];
	double *L=new double[nPtSum];
	double X,Y,Z;
	for(i=0;i<nPtSum;i++)
	{
		X=pts[i].x;
		Y=pts[i].y;
		Z=pts[i].z;

		A[i*3]=X;
		A[i*3+1]=Y;
		A[i*3+2]=1;
		L[i]=-Z;
	}
	double *res=LS(A,L,nPtSum,3);

	for(i=0;i<nPtSum;i++)
	{
		X=pts[i].x;
		Y=pts[i].y;
		Z=-(res[0]*X+res[1]*Y+res[2]);
		pts[i].z=Z;
	}
}


void SolveHomoTrans(const POINT2D *lPts, const POINT2D *rPts,int nPtSum ,double *H)
{
	double *A=new double[2*nPtSum*8];
	double *L=new double[2*nPtSum];
	double *res=new double[8];
	double xl,yl,xr,yr;

	for(int i=0;i<nPtSum;i++)
	{

		xl=lPts[i].x;
		yl=lPts[i].y;
		
		xr=rPts[i].x;
		yr=rPts[i].y;

		A[2*i*8]=xl;
		A[2*i*8+1]=yl;
		A[2*i*8+2]=1;
		A[2*i*8+3]=0;
		A[2*i*8+4]=0;
		A[2*i*8+5]=0;
		A[2*i*8+6]=-xr*xl;
		A[2*i*8+7]=-xr*yl;

		A[(2*i+1)*8]=0;
		A[(2*i+1)*8+1]=0;
		A[(2*i+1)*8+2]=0;
		A[(2*i+1)*8+3]=xl;
		A[(2*i+1)*8+4]=yl;
		A[(2*i+1)*8+5]=1;
		A[(2*i+1)*8+6]=-yr*xl;
		A[(2*i+1)*8+7]=-yr*yl;

		L[2*i]  =xr;
		L[2*i+1]=yr;
	}

	double *tp=NULL;
	tp=LS(A,L,2*nPtSum,8);

	for(i=0;i<8;i++)
	{
		H[i]=tp[i];
	}

}

COLORREF BiInterpolation(float cx,float cy,CXdhBmp &bmp)
{
	        COLORREF color;
	        
	        int lx=cx;
			int ty=cy;
                  
			COLORREF g1,g2,g3,g4;
			bmp.GetPixel(lx,ty,g1);
			bmp.GetPixel(lx+1,ty,g2);
			bmp.GetPixel(lx+1,ty+1,g3);
			bmp.GetPixel(lx,ty+1,g4);
		
			
			int rg1=0,rg2=0,rg3=0,rg4=0;
			rg1=GetRValue(g1);
			rg2=GetRValue(g2);
			rg3=GetRValue(g3);
			rg4=GetRValue(g4);
			
			int gg1=0,gg2=0,gg3=0,gg4=0;
			gg1=GetGValue(g1);
			gg2=GetGValue(g2);
			gg3=GetGValue(g3);
			gg4=GetGValue(g4);
			
			int bg1=0,bg2=0,bg3=0,bg4=0;
			bg1=GetBValue(g1);
			bg2=GetBValue(g2);
			bg3=GetBValue(g3);
			bg4=GetBValue(g4);
			
			float dx1=cx-lx;
			float dx2=1-dx1;
			float dy1=cy-ty;
			float dy2=1-dy1;
			
			//双线性内插：
			int ngr=dx2*dy2*rg1+dx1*dy2*rg2+dx1*dy1*rg3+dx2*dy1*rg4;
			int ngg=dx2*dy2*gg1+dx1*dy2*gg2+dx1*dy1*gg3+dx2*dy1*gg4;
			int ngb=dx2*dy2*bg1+dx1*dy2*bg2+dx1*dy1*bg3+dx2*dy1*bg4;

			color=RGB(ngr,ngg,ngb);

			return color;
}

COLORREF GetNbrMean(float cx,float cy,CXdhBmp &bmp)
{

		    COLORREF color;
	        
			int lx=cx;
			int ty=cy;
			
			COLORREF *g=new COLORREF[9];
			
			bmp.GetPixel(lx,ty,g[1]);
			bmp.GetPixel(lx,ty+1,g[2]);
			bmp.GetPixel(lx,ty-1,g[3]);

			bmp.GetPixel(lx-1,ty,g[4]);
			bmp.GetPixel(lx-1,ty+1,g[5]);
			bmp.GetPixel(lx-1,ty-1,g[6]);
			
			bmp.GetPixel(lx+1,ty,g[7]);
			bmp.GetPixel(lx+1,ty+1,g[8]);
			bmp.GetPixel(lx+1,ty-1,g[0]);
			

			//remember to initialization;
			float ngr=0,ngg=0,ngb=0;
			
			for(int i=0;i<9;i++)
			{
				ngr=ngr+GetRValue(g[i]);
				ngg=ngg+GetGValue(g[i]);
				ngb=ngb+GetBValue(g[i]);
			}

			ngr=ngr/9;
			ngg=ngg/9;
			ngb=ngb/9;

			color=RGB(ngr,ngg,ngb);

			return color;
}


bool FFT2(const STRU_COMPLEX *pS,int nWid,int nHei,int direction,STRU_COMPLEX *pD)
{
	if( pS==NULL) return FALSE;
		
	long w,h;
	w=nWid;
	h=nHei;
	
	bool bXpow2 = IsPowerof2(w);
	bool bYpow2 = IsPowerof2(h);
	
	//if bForceFFT, width AND height must be powers of 2
	if ( !(bXpow2 && bYpow2) )
	{
		long i;
		
		i=0;
		while((1<<i)<w) i++;
		w=1<<i;
		bXpow2=true;

		i=0;
		while((1<<i)<h) i++;
		h=1<<i;
		bYpow2=true;
	}
		
	double* real;
	double* imag;
	long j,k,m;

	_complex **grid;
	
	//double mean = tmpReal->Mean();
	// Allocate memory for the grid 
	grid = (_complex **)malloc(w * sizeof(_complex));
	for (k=0;k<w;k++)
	{
		grid[k] = (_complex *)malloc(h * sizeof(_complex));
	}
	
	//首先将扩张部分补为0：
    for (j=0;j<h;j++)
	{
		for (k=0;k<w;k++) 
		{	
			grid[k][j].x=0;
			grid[k][j].y=0;
		}
	}

	for(j=0;j<nHei;j++)
		for(k=0;k<nWid;k++)
		{
			grid[k][j].x=pS[j*nWid+k].x;
			grid[k][j].y=pS[j*nWid+k].y;
		}
	
	//DFT buffers
	double *real2,*imag2;
	real2 = (double*)malloc(max(w,h) * sizeof(double));
	imag2 = (double*)malloc(max(w,h) * sizeof(double));

	// Transform the rows 
	real = (double *)malloc(w * sizeof(double));
	imag = (double *)malloc(w * sizeof(double));

	m=0;
	while((1<<m)<w) m++;

	for (j=0;j<h;j++)
	{
		for (k=0;k<w;k++) 
		{
			real[k] = grid[k][j].x;
			imag[k] = grid[k][j].y;
		}
		if (bXpow2) 
			FFT(direction,m,real,imag);
		//else
			//DFT(direction,w,real,imag,real2,imag2);
		for (k=0;k<w;k++)
		{
			grid[k][j].x = real[k];
			grid[k][j].y = imag[k];
		}
	}

	free(real);
	free(imag);

	// Transform the columns 
	real = (double *)malloc(h * sizeof(double));
	imag = (double *)malloc(h * sizeof(double));

	m=0;
	while((1<<m)<h) m++;

	for (k=0;k<w;k++) 
	{
		for (j=0;j<h;j++)
		{
			real[j] = grid[k][j].x;
			imag[j] = grid[k][j].y;
		}

		if (bYpow2) 
			FFT(direction,m,real,imag);
		//else
			//DFT(direction,h,real,imag,real2,imag2);

		for (j=0;j<h;j++) 
		{
			grid[k][j].x = real[j];
			grid[k][j].y = imag[j];
		}
	}

	free(real);
	free(imag);

	free(real2);
	free(imag2);
	
	// converting from double to byte, there is a HUGE loss in the dynamics
	//  "nn" tries to keep an acceptable SNR, but 8bit=48dB: don't ask more 
	double nn=pow(2,log(max(w,h))/log(2)-4);
	//reversed gain for reversed transform
	if (direction==-1) nn=1/nn;
	//bMagnitude : just to see it on the screen
	
	bool bMagnitude=1;

	if(bMagnitude)nn*=4;
	
	//保存到raw格式的文件：
	FILE *fp=fopen("D://fft.raw","wb");
	for(j=0;j<h;j++)
		for(k=0;k<w;k++)
		{
			//转换为幅度：
			BYTE g=(BYTE)max(0,min(255,(nn*(3+log(_cabs(grid[k][j]))))));
			fwrite(&g,1,1,fp);
		}

	fclose(fp);
	
	fp=fopen("D:\\fft.spt","w");
		fprintf(fp,"%d %d %d ",w,h,24);
	fclose(fp);

	for (k=0;k<w;k++) 
		free (grid[k]);
	free (grid);

	return true;
}

////////////////////////////////////////////////////////////////////////////////

//   This computes an in-place complex-to-complex FFT 
//   x and y are the real and imaginary arrays of n=2^m points.
//   o(n)=n*log2(n)
//   dir =  1 gives forward transform
//   dir = -1 gives reverse transform 
//   Written by Paul Bourke, July 1998
//   FFT algorithm by Cooley and Tukey, 1965 


bool FFT(int dir,int m,double *x,double *y)
{
	long nn,i,i1,j,k,i2,l,l1,l2;
	double c1,c2,tx,ty,t1,t2,u1,u2,z;

	// Calculate the number of points 
	nn = 1<<m;

	// Do the bit reversal 
	i2 = nn >> 1;
	j = 0;
	for (i=0;i<nn-1;i++) {
		
		if (i < j) 
		{
			tx = x[i];
			ty = y[i];
			x[i] = x[j];
			y[i] = y[j];
			x[j] = tx;
			y[j] = ty;
		}
		
		k = i2;
		while (k <= j) 
		{
			j -= k;
			k >>= 1;
		}
		j += k;
	}

	// Compute the FFT 
	c1 = -1.0;
	c2 = 0.0;
	l2 = 1;

	for (l=0;l<m;l++) 
	{
		l1 = l2;
		l2 <<= 1;

		u1 = 1.0;
		u2 = 0.0;
	
		for (j=0;j<l1;j++) 
		{
			for (i=j;i<nn;i+=l2) 
			{
				i1 = i + l1;
				
				t1 = u1 * x[i1] - u2 * y[i1];
				t2 = u1 * y[i1] + u2 * x[i1];

				x[i1] = x[i] - t1;
				y[i1] = y[i] - t2;
				
				x[i] += t1;
				y[i] += t2;
			}
			
			z =  u1 * c1 - u2 * c2;
			u2 = u1 * c2 + u2 * c1;

			u1 = z;
		}
		
		c2 = sqrt((1.0 - c1) / 2.0);

		if (dir == 1)
			c2 = -c2;
		
		c1 = sqrt((1.0 + c1) / 2.0);
	}
	///////////////////////////////////////////////////////////////////////////



	// Scaling for forward transform 
	if (dir == 1)
	{
		for (i=0;i<nn;i++) 
		{
			x[i] /= (double)nn;
			y[i] /= (double)nn;
		}
	}

	return true;
}


bool IsPowerof2(long x)
{	
	long i=0;
	
	while ((1<<i)<x) i++;
	
	if (x==(1<<i)) return true;

	return false;
}
	
void GrdToImg(POINT3D grd,POINT2D &img,const double *L)
{

	double dX,dY,dZ;
	double dIx,dIy;
	
	dX=grd.x;
	dY=grd.y;
	dZ=grd.z;

	dIx=(L[0]*dX+L[1]*dY+L[2]*dZ+L[3])/(L[8]*dX+L[9]*dY+L[10]*dZ+1);
	dIy=(L[4]*dX+L[5]*dY+L[6]*dZ+L[7])/(L[8]*dX+L[9]*dY+L[10]*dZ+1);
		
	img.x=dIx;
	img.y=dIy;
}

int  GetWidOfOverlapArea(POINT3D grd,double fInterval,
						 double fcsLen, LP L)
{
		float sx=L.sx;
		float sy=L.sy;
		float sz=L.sz;

		float fL=sqrt( (grd.x-sx)*(grd.x-sx) +(grd.y-sy)*(grd.y-sy) +(grd.z-sz)*(grd.z-sz) );
		int   nSize=(fcsLen/fL)*fInterval+0.5;

		return nSize;
}


bool GrdToImg1(POINT3D grd,POINT2D &img,const STRU_LFromH HP)
{
	float f=0;
	float r11=0,r12=0,r13=0,r21=0,r22=0,r23=0,r31=0,r32=0,r33=0;
	float tx=0,ty=0,tz=0;

	f=HP.f;
	float k=HP.k;

	//interior matrix:
	double A[3][3];
	for(int j=0;j<3;j++)
		for(int i=0;i<3;i++)
			A[j][i]=0;

	A[0][0]=f;	A[1][1]=k*f; A[2][2]=1;

	
	//exterior elements:
	r11=HP.r11;	r12=HP.r12;	r13=HP.r13;
	r21=HP.r21;	r22=HP.r22;	r23=HP.r23;
	r31=HP.r31;	r32=HP.r32;	r33=HP.r33;

	tx=HP.tx;ty=HP.ty;tz=HP.tz;

	double R[3][3];
	double T[3];

	R[0][0]=r11;R[0][1]=r12;R[0][2]=r13;
	R[1][0]=r21;R[1][1]=r22;R[1][2]=r23;
	R[2][0]=r31;R[2][1]=r32;R[2][2]=r33;

	T[0]=tx;T[1]=ty;T[2]=tz;

	///////////////////////////////////////
	double X[3];
	X[0]=grd.x;	X[1]=grd.y; X[2]=grd.z;
	double dfT[3];
	mult(*R,X,dfT,3,3,1);

	dfT[0]=dfT[0]+T[0];
	dfT[1]=dfT[1]+T[1];
	dfT[2]=dfT[2]+T[2];

	////////////////////////
	double dfT1[3];
	mult(*A,dfT,dfT1,3,3,1);

	double x,y;
	x=dfT1[0]/dfT[2];
	y=dfT1[1]/dfT[2];

	img.x=x;
	img.y=y;

	return true;
}


float GetLenToCmr(POINT3D grd,const STRU_LFromH HP)
{
	
	float L=0;
	
	float r11=0,r12=0,r13=0,r21=0,r22=0,r23=0,r31=0,r32=0,r33=0;
	float tx=0,ty=0,tz=0;
	
	//exterior elements:
	r11=HP.r11;	r12=HP.r12;	r13=HP.r13;
	r21=HP.r21;	r22=HP.r22;	r23=HP.r23;
	r31=HP.r31;	r32=HP.r32;	r33=HP.r33;

	tx=HP.tx;ty=HP.ty;tz=HP.tz;

	double R[3][3];
	double T[3];

	R[0][0]=r11;R[0][1]=r12;R[0][2]=r13;
	R[1][0]=r21;R[1][1]=r22;R[1][2]=r23;
	R[2][0]=r31;R[2][1]=r32;R[2][2]=r33;
	////////////////////////////////////////////
	
	double X[3];
	X[0]=grd.x; 
	X[1]=grd.y;
	X[2]=grd.z;

	double rX[3];

	mult(*R,X,rX,3,3,1);

	L=sqrt((rX[0]-tx)*(rX[0]-tx)+(rX[1]-ty)*(rX[1]-ty)+(rX[2]-tz)*(rX[2]-tz));

	return L;

}


POINT3D GetExteriorPt(POINT3D grd, const STRU_LFromH HP)
{

	float r11=0,r12=0,r13=0,r21=0,r22=0,r23=0,r31=0,r32=0,r33=0;
	float tx=0,ty=0,tz=0;
	
	//exterior elements:
	r11=HP.r11;	r12=HP.r12;	r13=HP.r13;
	r21=HP.r21;	r22=HP.r22;	r23=HP.r23;
	r31=HP.r31;	r32=HP.r32;	r33=HP.r33;

	tx=HP.tx;ty=HP.ty;tz=HP.tz;

	double R[3][3];
	double T[3];

	R[0][0]=r11;R[0][1]=r12;R[0][2]=r13;
	R[1][0]=r21;R[1][1]=r22;R[1][2]=r23;
	R[2][0]=r31;R[2][1]=r32;R[2][2]=r33;

	T[0]=tx; T[1]=ty; T[2]=tz;
	////////////////////////////////////////////
	
	double X[3];
	X[0]=grd.x; 
	X[1]=grd.y;
	X[2]=grd.z;

	double rX[3];
	mult(*R,X,rX,3,3,1);

	//求单位向量：
	double sx=0,sy=0,sz=0;
	double norm=0;
	norm=sqrt( ( rX[0]-T[0])*( rX[0]-T[0]) 
		      +( rX[1]-T[1])*( rX[1]-T[1])
			  +( rX[2]-T[2])*( rX[2]-T[2]) );

	sx=( rX[0]+T[0] )/norm;
	sy=( rX[1]+T[1] )/norm;
	sz=( rX[2]+T[2] )/norm;

	sx=sx*norm*0.999;
	sy=sy*norm*0.999;
	sz=sz*norm*0.999;

	double t1[3];

	t1[0]=sx-T[0];
	t1[1]=sy-T[1];
	t1[2]=sz-T[2];

	//旋转复原：
	invers_matrix(*R,3);
	double res[3];
	mult(*R,t1,res,3,3,1);

	POINT3D eP;
	eP.x=res[0];
	eP.y=res[1];
	eP.z=res[2];
	
	return eP;
}
*/




