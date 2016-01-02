


#include "stdafx.h"
#include "Cali.h"
#include "matrix.h"
#include "newmatrix.h"

#include "imagematching.h"
#include "valdef.h"
#include "minpack_non.h"

#include "demoview.h"

#include "math.h"
  
extern Calibration cali;

#define MAX_E 30

int elimination[MAX_E];

void compute_function(int* m, int* n, double* m_variable, double* m_result)
{
	CMatrix A(3,3);
	CMatrix B(3,3);
	CMatrix H(3,3);
	
	A.SetElement(0,0,m_variable[6]);
	A.SetElement(0,1,0.0);
	A.SetElement(0,2,0.0);
	A.SetElement(1,0,0.0);
	A.SetElement(1,1,m_variable[6]);
	A.SetElement(1,2,0.0);
	A.SetElement(2,0,0.0);
	A.SetElement(2,1,0.0);
	A.SetElement(2,2,1.0);
		
	B.SetElement(0,2,m_variable[0]);
	B.SetElement(1,2,m_variable[1]);
	B.SetElement(2,2,m_variable[2]);
	
	/////////////////////////////
	double m_angle=sqrt(m_variable[3]*m_variable[3]+
		                m_variable[4]*m_variable[4]+
						m_variable[5]*m_variable[5]);

	double vector_x=m_variable[3]/m_angle;
	double vector_y=m_variable[4]/m_angle;
	double vector_z=m_variable[5]/m_angle;
	
	CMatrix I(3,3);
	I.SetElement(0,0,1.0);
	I.SetElement(0,1,0.0);
	I.SetElement(0,2,0.0);
	I.SetElement(1,0,0.0);
	I.SetElement(1,1,1.0);
	I.SetElement(1,2,0.0);
	I.SetElement(2,0,0.0);
	I.SetElement(2,1,0.0);
	I.SetElement(2,2,1.0);

	CMatrix skew_matrix(3,3);
	skew_matrix.SetElement(0,0,0.0);
	skew_matrix.SetElement(0,1,-vector_z);
	skew_matrix.SetElement(0,2,vector_y);
	skew_matrix.SetElement(1,0,vector_z);
	skew_matrix.SetElement(1,1,0.0);
	skew_matrix.SetElement(1,2,-vector_x);
	skew_matrix.SetElement(2,0,-vector_y);
	skew_matrix.SetElement(2,1,vector_x);
	skew_matrix.SetElement(2,2,0.0);

	CMatrix r(3,1);
	r.SetElement(0,0,vector_x);
	r.SetElement(1,0,vector_y);
	r.SetElement(2,0,vector_z);

	CMatrix temp1,temp2;
	temp1=(r*r.Transpose())*(1-cos(m_angle));
	temp2=skew_matrix*sin(m_angle);
	
	CMatrix rotation;
	rotation=I*cos(m_angle)+temp1+temp2;
		
	double r11=rotation.GetElement(0,0);
	double r12=rotation.GetElement(0,1);
	double r13=rotation.GetElement(0,2);
	double r21=rotation.GetElement(1,0);
	double r22=rotation.GetElement(1,1);
	double r23=rotation.GetElement(1,2);
	double r31=rotation.GetElement(2,0);
	double r32=rotation.GetElement(2,1);
	double r33=rotation.GetElement(2,2);
	
	B.SetElement(0,0,r11);
	B.SetElement(1,0,r21);
	B.SetElement(2,0,r31);
	B.SetElement(0,1,r13);
	B.SetElement(1,1,r23);
	B.SetElement(2,1,r33);
	/////////////////////////////
	H=A*B;
	//////////////////////////
	double tempa, tempb;
	
	for(int i=0;i<*m;i++)
	{
		for(int j=0;j<MAX_E;j++)
		{
			if(i==elimination[j])
				break;
		}
		if(j<MAX_E)
		{
			tempa=0.0;
			tempb=0.0;
		}
		else
		{
			tempa=cali.m_pImgs->pts[i].x
					-(H.GetElement(0,0)*cali.m_pGcps[i].x+H.GetElement(0,1)*cali.m_pGcps[i].y+H.GetElement(0,2))
					/(H.GetElement(2,0)*cali.m_pGcps[i].x+H.GetElement(2,1)*cali.m_pGcps[i].y+H.GetElement(2,2));

			tempb=cali.m_pImgs->pts[i].y
					-(H.GetElement(1,0)*cali.m_pGcps[i].x+H.GetElement(1,1)*cali.m_pGcps[i].y+H.GetElement(1,2))
					/(H.GetElement(2,0)*cali.m_pGcps[i].x+H.GetElement(2,1)*cali.m_pGcps[i].y+H.GetElement(2,2));
		}
		m_result[i]=tempa*tempa+tempb*tempb;
	}
}

void Result_iteration(int* m, int* n, double* m_variable, double* m_result)//Result_iteration(FCN) the name of the user-supplied subroutine which calculates the functions.  FCN must be declared in an EXTERNAL statement in the user calling program, and should be written as follows.
//(*Result_iteration)(sum_function, sum_variable, &res[1], &fvec[1]);
{
	double temp1,temp2;

	for(int i=0; i<*m;i++)
	{
		for(int j=0;j<MAX_E;j++)
		{
			if(i==elimination[j])
				break;
		}
		
		if(j<MAX_E)
		{
			temp1=0.0;
			temp2=0.0;
		}
		else
		{
			temp1=cali.m_pImgs->pts[i].x
					-(m_variable[0]*cali.m_pGcps[i].x+m_variable[1]*cali.m_pGcps[i].y+m_variable[2])
					/(m_variable[6]*cali.m_pGcps[i].x+m_variable[7]*cali.m_pGcps[i].y+m_variable[8]);

			temp2=cali.m_pImgs->pts[i].y
					-(m_variable[3]*cali.m_pGcps[i].x+m_variable[4]*cali.m_pGcps[i].y+m_variable[5])
					/(m_variable[6]*cali.m_pGcps[i].x+m_variable[7]*cali.m_pGcps[i].y+m_variable[8]);
		}
		m_result[i]=temp1*temp1+temp2*temp2;
	}

	/////////////////
	double temp=0;
	for( i=0; i<*m;i++)
	{
		temp+=m_result[i];
	}
	
}

Calibration::Calibration()
{
	m_nGcpSum=0;
	m_nImgSum=0;
	m_pImgs=NULL;
	m_pGcps=NULL;

	m_surf=0;

	nWid=0;
	nHei=0;
	strFileName="";
}

Calibration::~Calibration()
{

}

void  Calibration::SetImgs(int nImgSum,phIMAGE *pImgs)
{

	m_nImgSum=nImgSum;
	m_pImgs=pImgs;
}


void Calibration::SetGcps(int nGcpSum,POINT3D *pGcps)
{
	m_nGcpSum=nGcpSum;
	m_pGcps=pGcps;
}


void Calibration::ComputeH()
{
	//计算H矩阵，
	//这里修改了谢东海的矩阵计算方法


	//1.进行解析求解

	//解线性方程组AX=0
	//其中,x是由H矩阵的9个参数形成的向量:{h11,h12,h13,h21,h22,h23,h31,h32,h33}
	//另外,A是系数矩阵.它的形式如下:
	//第1,3,5,7...行:{gx,gz,1,0,0,0,-x*gx,-x*gz,-x}
	//第2,4,6,8...行:{0,0,0,gx,gz,1,-y*gx,-y*gz,-y}
//?
	
	//计算方法:
	// --首先,计算ATA
	// --然后,计算ATA的最小特征值所对应的特征向量.
	
	double *A=new double[2*m_nGcpSum*9];

	 ///////此处还有问题，需要修改，以使之适用于三维标定场

	for(int i=0;i<m_nGcpSum;i++)
	{	
		double x,y,gX,gY,gZ;

		x=m_pImgs->pts[i].x;
		y=m_pImgs->pts[i].y;

		//////假设控制点处于xz平面上  
		gX=m_pGcps[i].x;
		gY=m_pGcps[i].y;
		gZ=m_pGcps[i].z;

		A[i*2*9+0]=gX;
		A[i*2*9+1]=gZ;
		A[i*2*9+2]=1;
		A[i*2*9+3]=0;
		A[i*2*9+4]=0;
		A[i*2*9+5]=0;
		A[i*2*9+6]=-x*gX;
		A[i*2*9+7]=-x*gZ;
		A[i*2*9+8]=-x;

		A[(i*2+1)*9+0]=0;
		A[(i*2+1)*9+1]=0;
		A[(i*2+1)*9+2]=0;
		A[(i*2+1)*9+3]=gX;
		A[(i*2+1)*9+4]=gZ;
		A[(i*2+1)*9+5]=1;
		A[(i*2+1)*9+6]=-y*gX;
		A[(i*2+1)*9+7]=-y*gZ;
		A[(i*2+1)*9+8]=-y;
	}
	
	double *res=NULL;
	ClosedFormH(A,2*m_nGcpSum,9,res);//ATA的最小特征值
	//res X initial estimate of parameters vector
	//an array of length N.  On input X must contain an initial estimate of the solution vector.  On output X contains the final estimate of the solution vector.


	//2.进行优化求解 LM算法
	int sum_function=m_nGcpSum;//M no. of observation variables 观察变量个数 控制点个数
	// a positive integer input variable set to the number of functions.
	int sum_variable=9;//N no. of parameters  参数个数 N<=M
	//a positive integer input variable set to the number of variables.  N must not exceed M.
	double *fvec=new double[sum_function];//no need to populate 控制点个数的运算数组（输出）
	//an output array of length M which contains the functions evaluated at the output X.
	double ftol=0.00000001; //tolerance 预计与真实平方和相对约简阈值 平方和相对误差
	double xtol=0.00000001; //连续两次迭代的相对误差阈值 近似解的相对误差
	double gtol=0.0; //FVEC与雅可比矩阵的列夹角的cos阈值 向量函数与雅可比矩阵的列的正交
	//a non-negative input variable. Termination occurs when the cosine of the angle between FVEC and any column of the Jacobian is at most GTOL in absolute value. Therefore, GTOL measures the orthogonality desired between the function vector and the columns of the Jacobian. 
	
	int    maxfev;//maximum function evaluations 最大运算次数
	maxfev=200*(sum_variable+1);
	//a positive integer input variable. Termination occurs when the number of calls to FCN is at least MAXFEV by the end of an iteration.
	
	double epsfcn=0.0000001;//tolerance 决定下一步差分逼近的参数
	//an input variable used in determining a suitable step for the forward-difference approximation. This approximation assumes that the relative errors in the functions are of the order of EPSFCN. If EPSFCN is less than the machine precision, it is assumed that the relative errors in the functions are of the order of the machine precision.
	double *diag=new double[sum_variable];//内部设定量级参数（mode=1）
	int mode=1;
	double factor=100.0;//首次运算边界
	//a positive input variable used in determining the initial step bound. This bound is set to the product of FACTOR and the Euclidean norm of DIAG*X if non-zero, or else to FACTOR itself. In most cases FACTOR should lie in the interval (0.1,100.0). 100.0 is a generally recommended value.

	int nprint=0;
	int info;//结束状态（输出）
	/*
	INFO = 0 Improper input parameters.
	INFO = 1 Both actual and predicted relative reductions in the  sum of squares are at most FTOL.
	INFO = 2 Relative error between two consecutive iterates is  at most XTOL.
	INFO = 3 Conditions for INFO = 1 and INFO = 2 both hold.
	INFO = 4 The cosine of the angle between FVEC and any column  of the Jacobian is at most GTOL in absolute value.
	INFO = 5 Number of calls to FCN has reached or exceeded  MAXFEV.
	INFO = 6 FTOL is too small. No further reduction in the sum  of squares is possible.
	INFO = 7 XTOL is too small. No further improvement in the  approximate solution X is possible.
	INFO = 8 GTOL is too small. FVEC is orthogonal to the  columns of the Jacobian to machine precision.
	*/
	int nfev;//Result_iteration运算次数（输出）
	
	double *fjac=new double[sum_function*sum_variable];//雅可比矩阵（输出）
	//an output M by N array.  The upper N by N sub-matrix of FJAC contains an upper triangular matrix R with diagonal elements of non-increasing magnitude such that
				//PT *(JACT *JAC)*P = RT*R,
	//where P is a permutation matrix and JAC is the final calculated J
	int ldfjac=sum_function;
	
	int *inpvt=new int[sum_variable];
	double *qtf=new double[sum_variable];
	double *wa1=new double[sum_variable];
	double *wa2=new double[sum_variable];
	double *wa3=new double[sum_variable];
	double *wa4=new double[sum_function];
#if 0
	minpack_non m_optimization;//该类的作用是用来介求非线型的最优化问题

	for(i=0;i<MAX_E;i++)
	{
		elimination[i]=-1;//30个-1?
	}
	for(i=0;i<m_nGcpSum*0.3;i++)//?
	{
		m_optimization.lmdif_(Result_iteration, &sum_function, &sum_variable,
							res, fvec, &ftol, &xtol, &gtol, &maxfev,&epsfcn,
							diag, &mode, &factor, &nprint, &info, &nfev,
							fjac, &ldfjac, inpvt, qtf, wa1, wa2, wa3, wa4);
		//Result_iteration(FCN) the name of the user-supplied subroutine which calculates the functions.  FCN must be declared in an EXTERNAL statement in the user calling program, and should be written as follows.
		//(*fcn)(m, n, &x[1], &fvec[1]);
		//(*Result_iteration)(sum_function, sum_variable, &res[1], &fvec[1]);
		double m_max_fvec=fvec[0];
		int index=0;
		for(int j=1;j<sum_function;j++)
		{
			if(fvec[j]>m_max_fvec)
			{
				m_max_fvec=fvec[j];
				index=j;
			}
		}
		elimination[i]=index; 
	}
#endif

	delete []fvec;
	delete []diag;
	delete []fjac;
	delete []inpvt;
	delete []qtf;
	delete []wa1;
	delete []wa2;
	delete []wa3;
	delete []wa4;


	///////////////////////////////////
	m_pImgs->H[0]=res[0];
	m_pImgs->H[1]=res[1];
	m_pImgs->H[2]=res[2];
	
	m_pImgs->H[3]=res[3];
	m_pImgs->H[4]=res[4];
	m_pImgs->H[5]=res[5];
	
	m_pImgs->H[6]=res[6];
	m_pImgs->H[7]=res[7];
	m_pImgs->H[8]=res[8];
	delete []res;
}



float Calibration::ComputeFlFromH(STRU_LFromH  &m_HP)
{
#if 0
	CMatrix rotation(3,3);
	CMatrix t(3,1);
	double f=1983.303792;
	rotation.SetElement(0,0,0.8742057841);
	rotation.SetElement(0,1,0.0331165639);
	rotation.SetElement(0,2,0.4844249583);
	rotation.SetElement(1,0,-0.3758850362);
	rotation.SetElement(1,1,-0.5853910621);
	rotation.SetElement(1,2,0.7183507110);
	rotation.SetElement(2,0,0.3073673481);
	rotation.SetElement(2,1,-0.8100744396);
	rotation.SetElement(2,2,-0.4993042316);
	t.SetElement(0,0,3.038670);
	t.SetElement(1,0,-0.657680);
	t.SetElement(2,0,99.640847);
	nWid=1600;
	nHei=1200;
	double r11=0,r21=0,r31=0;
	double r12=0,r22=0,r32=0;
	double r13=0,r23=0,r33=0;
	double Tx=0,Ty=0,Tz=0;
#else
	ASSERT(m_nImgSum>=4);	
	ASSERT(m_nGcpSum>=4);
	ComputeH();

	///////////////////////////////////////////////////////////////
	double h11,h12,h13,h21,h22,h23,h31,h32,h33;
	
	h11=m_pImgs->H[0];
	h12=m_pImgs->H[1];
	h13=m_pImgs->H[2];
	
	h21=m_pImgs->H[3];
	h22=m_pImgs->H[4];
	h23=m_pImgs->H[5];
	
	h31=m_pImgs->H[6];
	h32=m_pImgs->H[7];
	h33=m_pImgs->H[8];

	///////////////////////////////////////////////////////////////
	//首先，求解封闭解
	double f,f1,f2;
	
	////////////////////////
	//此处的问题在于，两个开方函数的参数均可能为负，此时将导致错误
	////////////////////////
	if((h11*h12+h21*h22)/(h31*h32) < 0)
		f1=sqrt( -(h11*h12+h21*h22)/(h31*h32) );
	else
		f1=sqrt( (h11*h12+h21*h22)/(h31*h32) );

	f2=sqrt( (h11*h11+h21*h21-h12*h12-h22*h22)/(h32*h32-h31*h31));
	
	if(f2<=0)
	{
		f=f1;
	}
	else
	{
		f=f2;
	}
	
	double r11=0,r21=0,r31=0;
	double r12=0,r22=0,r32=0;
	double r13=0,r23=0,r33=0;
	double Tx=0,Ty=0,Tz=0;
	
	r11=h11/f;
	r21=h21/f;
	r31=h31;

	r13=h12/f;
	r23=h22/f;
	r33=h32;
	
	double m=sqrt(r11*r11+r21*r21+r31*r31);
	r11=r11/m;
	r21=r21/m;
	r31=r31/m;

	m=sqrt(r13*r13+r23*r23+r33*r33);
	r13=r13/m;
	r23=r23/m;
	r33=r33/m;
	
    r12=(-r21*r33+r23*r31);
	r22=(-r13*r31+r11*r33);
	r32=(-r23*r11+r13*r21);
	
	double m1=sqrt(r12*r12+r22*r22+r32*r32);
	r12=r12/m1;
	r22=r22/m1;
	r32=r32/m1;

	Tx=(1.0/m)*h13/f;
	Ty=(1.0/m)*h23/f;
	Tz=(1.0/m)*h33;
	

	//然后，进行优化处理
	
	//a. 将旋转矩阵转化为rodrigues formula
		//a.1计算一个旋转矩阵
	CMatrix rotation(3,3);
	CMatrix U;
	CMatrix V;

	rotation.SetElement(0,0,r11);
	rotation.SetElement(0,1,r12);
	rotation.SetElement(0,2,r13);
	rotation.SetElement(1,0,r21);
	rotation.SetElement(1,1,r22);
	rotation.SetElement(1,2,r23);
	rotation.SetElement(2,0,r31);
	rotation.SetElement(2,1,r32);
	rotation.SetElement(2,2,r33);

	rotation.SplitUV(U,V);//奇异值分解
	rotation=U*V;
	
		//a.2将旋转矩阵进行变换
	CMatrix skew_matrix=(rotation-rotation.Transpose());
	skew_matrix = skew_matrix*0.5;//扭曲
	
	double vector_1=skew_matrix.GetElement(2,1);
	double vector_2=skew_matrix.GetElement(0,2);
	double vector_3=skew_matrix.GetElement(1,0);

	double s=sqrt(vector_1*vector_1+vector_2*vector_2+vector_3*vector_3);
	vector_1 /= s;
	vector_2 /= s;
	vector_3 /= s;
	
	
	CMatrix sum_rotation;
	sum_rotation=(rotation+rotation.Transpose())*0.5;

	CMatrix mul_skew;
	mul_skew=(skew_matrix*(1.0/s))*(skew_matrix*(1.0/s));

	double c=1-(sum_rotation.GetElement(0,0)-1.0)/mul_skew.GetElement(0,0);
	double m_alpha=asin(s);
	if(c<0)
	{
		m_alpha=PI-m_alpha;
	}
	
	//b. 优化处理
	int sum_function=m_nGcpSum;
	int sum_variable=7;
	
	double* variable=new double[sum_variable];
	variable[0]=Tx;
	variable[1]=Ty;
	variable[2]=Tz;
	variable[3]=m_alpha*vector_1;
	variable[4]=m_alpha*vector_2;
	variable[5]=m_alpha*vector_3;
	variable[6]=f;
	
	double *fvec=new double[sum_function];
	double ftol=0.0000001;
	double xtol=0.0000001;
	double gtol=0.0;
	
	int    maxfev;
	maxfev=200*(sum_variable+1);

	double epsfcn=0.0000001;
	double *diag=new double[sum_variable];
	int mode=1;
	double factor=100.0;

	int nprint=0;
	int info;
	int nfev;

	double *fjac=new double[sum_function*sum_variable];
	int ldfjac=sum_function;

	int *inpvt=new int[sum_variable];
	double *qtf=new double[sum_variable];
	double *wa1=new double[sum_variable];
	double *wa2=new double[sum_variable];
	double *wa3=new double[sum_variable];
	double *wa4=new double[sum_function];
#if 0
	//b.1 优化运算
	minpack_non m_optimization;

	m_optimization.lmdif_(compute_function, &sum_function, &sum_variable,
						variable, fvec, &ftol, &xtol, &gtol, &maxfev,&epsfcn,
						diag, &mode, &factor, &nprint, &info, &nfev,
						fjac, &ldfjac, inpvt, qtf, wa1, wa2, wa3, wa4);
#endif
	//b.2 返回结果
	f=variable[6];

	CMatrix t(3,1);
	t.SetElement(0,0,variable[0]);
	t.SetElement(1,0,variable[1]);
	t.SetElement(2,0,variable[2]);
	
	m_alpha=sqrt(variable[3]*variable[3]+variable[4]*variable[4]+variable[5]*variable[5]);
	vector_1 = variable[3]/m_alpha;
	vector_2 = variable[4]/m_alpha;
	vector_3 = variable[5]/m_alpha;
	
	delete []variable;
	delete []fvec;
	delete []diag;
	delete []fjac;
	delete []inpvt;
	delete []qtf;
	delete []wa1;
	delete []wa2;
	delete []wa3;
	delete []wa4;
	
	CMatrix I(3,3);
	I.SetElement(0,0,1.0);
	I.SetElement(0,1,0.0);
	I.SetElement(0,2,0.0);
	I.SetElement(1,0,0.0);
	I.SetElement(1,1,1.0);
	I.SetElement(1,2,0.0);
	I.SetElement(2,0,0.0);
	I.SetElement(2,1,0.0);
	I.SetElement(2,2,1.0);

	skew_matrix.SetElement(0,0,0.0);
	skew_matrix.SetElement(0,1,-vector_3);
	skew_matrix.SetElement(0,2,vector_2);
	skew_matrix.SetElement(1,0,vector_3);
	skew_matrix.SetElement(1,1,0.0);
	skew_matrix.SetElement(1,2,-vector_1);
	skew_matrix.SetElement(2,0,-vector_2);
	skew_matrix.SetElement(2,1,vector_1);
	skew_matrix.SetElement(2,2,0.0);

	CMatrix r(3,1);
	r.SetElement(0,0,vector_1);
	r.SetElement(1,0,vector_2);
	r.SetElement(2,0,vector_3);
	
	CMatrix temp1,temp2;
	temp1=(r*r.Transpose())*(1-cos(m_alpha));
	temp2=skew_matrix*sin(m_alpha);
	
	rotation=I*cos(m_alpha)+temp1+temp2;
	
#endif
	//统一坐标系
	TransformFromBasicCamera(rotation,t,29.5);//cubeline=29.5

	r11=rotation.GetElement(0,0);
	r12=rotation.GetElement(0,1);
	r13=rotation.GetElement(0,2);
	r21=rotation.GetElement(1,0);
	r22=rotation.GetElement(1,1);
	r23=rotation.GetElement(1,2);
	r31=rotation.GetElement(2,0);
	r32=rotation.GetElement(2,1);
	r33=rotation.GetElement(2,2);

	Tx=t.GetElement(0,0);
	Ty=t.GetElement(1,0);
	Tz=t.GetElement(2,0);


	//computer the unit projection matrix:
	//interior matrix:
	double A[3][3];
	for(int j=0;j<3;j++)
		for(int i=0;i<3;i++)
			A[j][i]=0;

	A[0][0]=f;	 A[1][1]=f;  A[2][2]=1;
	A[0][2]=nWid/2;  A[1][2]=nHei/2;
	
	double p1[3][4];
	p1[0][0]=r11; p1[0][1]=r12; p1[0][2]=r13; 
	p1[1][0]=r21; p1[1][1]=r22; p1[1][2]=r23; 
	p1[2][0]=r31; p1[2][1]=r32; p1[2][2]=r33;

	p1[0][3]=Tx;
	p1[1][3]=Ty;
	p1[2][3]=Tz;
	double p[3][4];
	mult(*A,*p1,*p,3,3,4);	  	

	double R[3][3];
	R[0][0]=r11;		R[0][1]=r12;		R[0][2]=r13;
	R[1][0]=r21;		R[1][1]=r22;		R[1][2]=r23;
	R[2][0]=r31;		R[2][1]=r32;		R[2][2]=r33;
	double T[3];
	T[0]=-Tx;
	T[1]=-Ty;
	T[2]=-Tz;
	double T1[3];
	invers_matrix(*R,3);//求逆
	mult(*R,T,T1,3,3,1);     //T1 = inv(R)(-T) = - inv(R)T	
	
	///////////////////////////////
	double  faita,omiga,kapa;
	faita = atan(-R[2][0]/R[2][2]);
	omiga = asin(-R[2][1]);
	kapa  = atan(R[0][1]/R[1][1] );
	
	faita=(faita/PI)*180;
	omiga=(omiga/PI)*180;
	kapa= (kapa/PI)*180;

	///////////////////////////////
	m_HP.f=f;
	m_HP.k=1.0;

	m_HP.faita=faita;
	m_HP.kapa=kapa;
	m_HP.omiga=omiga;
	
	m_HP.P[0][0]=p[0][0];
	m_HP.P[0][1]=p[0][1];
	m_HP.P[0][2]=p[0][2];
	m_HP.P[0][3]=p[0][3];
	m_HP.P[1][0]=p[1][0];
	m_HP.P[1][1]=p[1][1];
	m_HP.P[1][2]=p[1][2];
	m_HP.P[1][3]=p[1][3];
	m_HP.P[2][0]=p[2][0];
	m_HP.P[2][1]=p[2][1];
	m_HP.P[2][2]=p[2][2];
	m_HP.P[2][3]=p[2][3];
	
	m_HP.r11=r11;
	m_HP.r12=r12;
	m_HP.r13=r13;
	m_HP.r21=r21;
	m_HP.r22=r22;
	m_HP.r23=r23;
	m_HP.r31=r31;
	m_HP.r32=r32;
	m_HP.r33=r33;
	
	m_HP.tx=Tx;
	m_HP.ty=Ty;
	m_HP.tz=Tz;
	
	m_HP.hei=nHei;
	m_HP.wid=nWid;

	m_HP.Xs=T1[0];
	m_HP.Ys=T1[1];
	m_HP.Zs=T1[2];

	return (float)f;
}


void Calibration::TransformFromBasicCamera(CMatrix &RP, CMatrix &TP, double cubeline)
{
	//Xc=RP*XwP+TP  XwP=RXw+T
	//Xc=(RP*R)Xw+(RP*T+TP)
	double r[9]={0};
	double t[3]={0};
	switch(m_surf){
		case 0:
			return;
		case 1:
			r[1]=1.0;
			r[5]=1.0;
			r[6]=1.0;
			t[1]=-cubeline;
			break;
		case 2:
			r[1]=1.0;
			r[3]=-1.0;
			r[8]=1.0;
			break;
		case 3:
			r[1]=1.0;
			r[5]=-1.0;
			r[6]=-1.0;
			t[2]=cubeline;
			break;
		case 4:
			r[1]=1.0;
			r[3]=1.0;
			r[8]=-1.0;
			t[1]=-cubeline;
			t[2]=cubeline;
			break;
		case 5:
			r[0]=1.0;
			r[4]=1.0;
			r[8]=1.0;
			t[1]=-cubeline;
			break;
	}
	CMatrix R(3,3,r);
	CMatrix T(3,1,t);

	TP = RP * T + TP;
	RP = RP * R;
	return;
}



//2007,07,30  宋本聪
//依据真实相机参数计算相应的虚拟相机参数
//参数之间的对应关系见《会议报告_070718》
void Calibration::ComputeVirtualCamera(STRU_LFromH &trueCamera,STRU_LFromH &virtualCamera)
{
	
	//计算虚拟相机的内参矩阵
	double A[3][3];
	for(int j=0;j<3;j++)
		for(int i=0;i<3;i++)
			A[j][i]=0;

	A[0][0] = trueCamera.f;	A[1][1] = trueCamera.f;	A[2][2] = 1;
	A[0][2] = trueCamera.wid/2;  A[1][2] = trueCamera.hei/2;
	
	//计算虚拟相机的外参矩阵(旋转矩阵&平移矩阵)
	double p1[3][4];
	p1[0][0] = trueCamera.r11; p1[0][1] = -trueCamera.r12; p1[0][2] = trueCamera.r13; 
	p1[1][0] = -trueCamera.r21; p1[1][1] = trueCamera.r22; p1[1][2] = -trueCamera.r23; 
	p1[2][0] = trueCamera.r31; p1[2][1] = -trueCamera.r32; p1[2][2] = trueCamera.r33;

	p1[0][3] = trueCamera.tx;
	p1[1][3] = -trueCamera.ty;
	p1[2][3] = trueCamera.tz;

	//计算虚拟相机的投影矩阵
	double p[3][4];
	mult(*A,*p1,*p,3,3,4);		

	double R[3][3];
	R[0][0] = trueCamera.r11; R[0][1] = -trueCamera.r12; R[0][2] = trueCamera.r13; 
	R[1][0] = -trueCamera.r21; R[1][1] = trueCamera.r22; R[1][2] = -trueCamera.r23; 
	R[2][0] = trueCamera.r31; R[2][1] = -trueCamera.r32; R[2][2] = trueCamera.r33;
	
	//虚拟相机光心的位置C与真实相机光心位置C'关于镜面对称
	//且C = - inv(R)*T = inv(R) * (-T)
	//现在，已知虚拟相机光心位置C，反推出平移矩阵T = -R*C
	double T1[3];
	T1[0] = -trueCamera.Xs;
	T1[1] = trueCamera.Ys;
	T1[2] = -trueCamera.Zs;        //T1 = -C

	double T[3];
	mult(*R,T1,T,3,3,1);           //T = R*T1 = R*(-C) = -R*C	
	
	//计算虚拟相机坐标系相对于物体坐标系的旋转角度
	//只是一组模拟解(不准确，因为并无唯一确定解)
	//真正利用时，用到的是旋转矩阵，而非旋转角度
	double  faita,omiga,kapa;
	faita = atan(-R[2][0]/R[2][2]);
	omiga = asin(-R[2][1]);
	kapa  = atan(R[0][1]/R[1][1] );
	
	faita=(faita/PI)*180;
	omiga=(omiga/PI)*180;
	kapa= (kapa/PI)*180;

	///////////////////////////////
	//保存计算结果至输出参数

	//记录虚拟相机的内参数
	virtualCamera.f = trueCamera.f;          //有效焦距 
	virtualCamera.k = 1.0;                   //假定像素尺度因子为1.0

	//记录虚拟相机坐标系的三个旋转角度
	virtualCamera.faita = faita;             
	virtualCamera.kapa = kapa;
	virtualCamera.omiga = omiga;
	
	//记录虚拟相机的投影矩阵
	virtualCamera.P[0][0] = p[0][0];
	virtualCamera.P[0][1] = p[0][1];
	virtualCamera.P[0][2] = p[0][2];
	virtualCamera.P[0][3] = p[0][3];
	virtualCamera.P[1][0] = p[1][0];
	virtualCamera.P[1][1] = p[1][1];
	virtualCamera.P[1][2] = p[1][2];
	virtualCamera.P[1][3] = p[1][3];
	virtualCamera.P[2][0] = p[2][0];
	virtualCamera.P[2][1] = p[2][1];
	virtualCamera.P[2][2] = p[2][2];
	virtualCamera.P[2][3] = p[2][3];
	
	//记录虚拟相机的旋转矩阵
	virtualCamera.r11 = trueCamera.r11;
	virtualCamera.r12 = -trueCamera.r12;
	virtualCamera.r13 = trueCamera.r13;
	virtualCamera.r21 = -trueCamera.r21;
	virtualCamera.r22 = trueCamera.r22;
	virtualCamera.r23 = -trueCamera.r23;
	virtualCamera.r31 = trueCamera.r31;
	virtualCamera.r32 = -trueCamera.r32;
	virtualCamera.r33 = trueCamera.r33;
	
	//虚拟相机的平移矩阵,是根据相机光心的镜面对称关系反推出来的
	virtualCamera.tx = T[0];
	virtualCamera.ty = T[1];
	virtualCamera.tz = T[2];
	
	
	virtualCamera.hei = trueCamera.hei;
	virtualCamera.wid = trueCamera.wid;

	virtualCamera.Xs = trueCamera.Xs;
	virtualCamera.Ys = -trueCamera.Ys;
	virtualCamera.Zs = trueCamera.Zs;
}




