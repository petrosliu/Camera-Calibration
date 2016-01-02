 


 
  #include "stdio.h"
  #include "math.h"
  //#include "stdlib.h"

//#include"MatrixApp.h"
//static void ppp(double a[],double e[],double s[],double v[],int m,int n)

void  eig(double a[],double q[],double b[],int n);

void  cstrq(double a[],int n,double q[],double b[],double c[]);

 int csstq(int n,double b[],double c[],double q[] ,double eps,int l);

 int  cjcbi(long double a[],int n,long double v[],long double eps,int jt);
 
 void cjcbj( double a[],int n, double v[], double eps);
 
 int  agmiv(double a[],int m,int n,double b[],double x[],double aa[],double eps,double u[],double v[],int ka);
 
 int  bginv(double a[],int m,int n,double aa[],double eps,double u[],double v[],int ka);
 
 int  bmuav(double a[],int m,int n,double u[],double v[],double eps,int ka);
 
 void ppp(double a[],double e[],double s[],double v[],int m,int n);

