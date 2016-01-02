#ifndef TYPE_DEF
#define TYPE_DEF

//表示复数的结构体：
struct STRU_COMPLEX
{
	double x;//实部
	double y;//虚部
	double norm;
};


struct IMGINFO{
	int  nImgWid;
	int  nImgHei;
	int  nColors;
};

struct Str_DPt
{
	double x;
	double y;
};

///for Wong-Trinder circle center detector:
struct W_TPOINT{
	float x,y;
	float roundness;
	bool  bIsDel;
};

struct W_TDetectStru{
	CSize		winSize;	//the size of search window;
	int			nSum;       //the sum of detected points;
	W_TPOINT    *pInterestPts;//the detected points;
};

/////////////////////////////////////////////


///for detect the center:
struct fCenP{
	float xSum;
    float ySum;
	int   nSum;
	int   index;
};

struct  CircleDetectStru{
	int    nSum;
	fCenP *pCen;
};


////for Matching :
//
struct RIGSINFO{

	BYTE* pLData;
	BYTE* pRData;
	int   lWid;
	int   lHei;
	int   rWid;
	int   rHei;
	int   nColors;//1--8 bits; 3--24 bits;
};

struct MATCHPT {
	float x,y;
	float fWeight;
    bool  bFlag;
	int   index;
	float fCompItem;//for compare with other item,for sort..
};

struct INTERESTPT {
	float   x,y;
	int     nIndex;
	int     nPbMatchPtsSum;
	MATCHPT *matchPts;
};

struct MATCHRESULT{
	int        nLPtSum;   
	int		   nRPtSum;
	INTERESTPT *pLFeaturePts;
	INTERESTPT *pRFeaturePts;
};

struct MATCHPAIR{
	float lx,ly;
	float rx,ry;
	int   nLIndex;
	int   nRIndex;
};

typedef struct  structSort{
	int   nIndex;
	float fSortV;
}STRUCTSORT;
/////////////////////////////////////////////
//for optimization:
typedef struct structdfIPt{
    int    nIndex;
	double x;
	double y;
}dfIPt;

typedef struct structdfGPt{
    int    nIndex;
  	double x;
	double y;
	double z;
}dfGPt;


typedef struct structCtrlPts{
	int    nSum;
    dfGPt  *pCtrlPts;
}CtrlData;

typedef struct structBundle{

	double dfIx;
	double dfIy;
	double dfGx;
	double dfGy;
	double dfGz;
	int    nIndex;
	bool   bIsCtrlP;
}StruOneBundle;

typedef struct structInnerParas{
	float f;
	float x0;
	float y0;
}InnerData;

typedef struct structOuterParas{
	float  faita;
	float  omiga;
	float  kapa;
	float  Xs;
	float  Ys;
	float  Zs;
	double R[9];
}OuterData;

struct STRU_Rotation
{
	float a1,a2,a3;
    float b1,b2,b3;
	float c1,c2,c3;
	float Bx,By,Bz;
};

typedef struct structOptimizationData{
	//内方位元素：
    InnerData innerData;
	//外方位元素:
    OuterData outerData;
	//对称径向畸变系数：
	float     k1,k2;
	//切向畸变系数：
	float     p1,p2;
	//比例尺不一系数：
	float     ds;
	//x,y轴间的不正交系数：
	float     db;
	//点数据：
	int				nBundleSum;   //单张相片中的光线束的数目；
	StruOneBundle   *pStruBundles;//光线束对应的像点和大地点的坐标：
}StruOptiData;

typedef struct snakeVex{
	float  x,y;
	float  ox,oy;
	float  Eint;
	float  Eext;
	struct snakeVex *pre;
	struct snakeVex *next;
   	/*
	struct snakeVex * operator=(struct snakeVex *r){
		//memcpy(l,r,sizeof(struct snakeVex));
		return r;
	};
	*/
}SNAKENODE;

//added by xdh,to collect good match points:
typedef struct{
     double v;
	 int    nSum;
}GMP;


struct POINT2D	
{
	char name[10];
	double x,y;
};

struct POINT3D 
{
	char name[10];
	double x,y,z;
	double ex,ey,ez;		//?
};

struct	phCAMERA	{
	double fx,fy;			// with lens
	double ds, dBeta;		// with CCD
	double sindBeta, cosdBeta;	
	double x0, y0;			// in pixel, witb CCD and lens
	double k1, k2;			// with lens
	double p1,p2;			// with lens
	double c;
	double landa;
};


struct	phIMAGE	{
	
	double Xs,Ys,Zs;	//?
	double a,b,c;		//?
	double R[9];
	double H[9];                //同形矩阵；
	
	double faita,omiga,kapa;	//?
	
	CString fileName;

	int	numOfPts;
	POINT2D *pts;	//点坐标

};

struct strLINE{

	float   k,b;
	POINT2D *pts;
	int     nPtSum;
	POINT2D *ptsTrue;
};

//for feature point:
typedef struct STRU_FP
{
	int   nId;
	float fX;
	float fY;
    STRU_FP *next;
}FP;

//for hough transform:
typedef struct STRU_EdgePt
{
	float         x;
	float         y;
	float       grad;
    STRU_EdgePt *pNext;
}EP;

struct LP
{
	double L[11];
	double sx,sy,sz;
};

struct DecisionMatrix
{
	BYTE *dsnVl;
};

//Voxel Coloring: opaque,homogenous point
struct  STRT_Voxel
{
	double X,Y,Z;
	int    r,g,b;
};

struct  STRT_Pixel
{
	int x,y;     //column ,row index;
	int r,g,b;   //
	int nIndex;  //the index of image 
};


struct STRU_LFromH
{
	//interior elements:
	double f;
	double k;
	
	//exterior elements for computer vision:
	double r11,r12,r13;
	double r21,r22,r23;
	double r31,r32,r33;
	double tx,ty,tz;
	double P[3][4];

	//exterior elements for photogrammetry:
	double faita,omiga,kapa;
	double Xs,Ys,Zs;

	//image info:
	int   wid,hei;
};




#endif


