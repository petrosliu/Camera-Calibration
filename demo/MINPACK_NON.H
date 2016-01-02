//避免被重定义：
#ifndef _H_MINPACK_NON_
#define _H_MINPACK_NON_

//该类的作用是用来介求非线型的最优化问题：
//##ModelId=4296C0F70316
 class minpack_non
 {
 public:
	//##ModelId=4296C0F70317
    minpack_non();
	//##ModelId=4296C0F70354
	~minpack_non();

	//##ModelId=4296C0F70383
    int lmdif_(void (*fcn)(int* ,int*,double*,double*), int* m, int* n, double* x, double* fvec,
		double* ftol, double* xtol, double* gtol, int* maxfev, 
		double* epsfcn, double* diag, int* mode, double* factor, int* nprint, int* info,
		int* nfev, double* fjac, int* ldfjac, int* ipvt, 
		double* qtf, double* wa1, double* wa2, double* wa3, double* wa4);
    
    
	//##ModelId=4296C0F80009
	double enorm_(int* n, double* x);
	

	//##ModelId=4296C0F80028
	int fdjac2_(void (*fcn)(int *,int *,double *,double *), int* m, int* n, double* x, double*fvec,
	    double* fjac, int* ldfjac, int* iflag, double* epsfcn,double* wa);

	//##ModelId=4296C0F80057
	double dpmpar_(int* i);

	//##ModelId=4296C0F80076
	int qrfac_(int* m, int* n, double* a, int* lda,
	    int* pivot, int* ipvt, int* lipvt, double* rdiag, double* acnorm, 
	    double* wa);

	//##ModelId=4296C0F80095
/* Subroutine */ int lmpar_(int *n, double* r, int *ldr, int *ipvt, double *diag,
	double *qtb, double *delta, double *par, double *x, double *sdiag, 
	double *wa1, double *wa2);

	//##ModelId=4296C0F800C4
/* Subroutine */ int qrsolv_(int *n, double *r, int *ldr, int *ipvt,
	double *diag, double *qtb, double *x, double *sdiag, double *wa);

	//*****************************************************

 // void   cc_full_optimization_error_h(int *m_ptr,int *n_ptr, double *params, double *err);


 };

#endif //_H_MINPACK_NON_
 
