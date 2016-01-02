




 #include "stdio.h"
 #include "math.h"

 #define LIM  3.1415926/(1800*3600)

void mult(double *m1,double *m2,double *result,int i_1,int j_12,int j_2);

int  invers_matrix(double *m1,int n);

void transpose(double *m1,double *m2,int m,int n);


void add(double *m1,double *m2,double *result,int i_1,int j_1);
void print(double *matrix_p,int x,int y);
void f_print(FILE *p,double *matrix_p,int x,int y);
void inv(double *a,int n);

int cjcbi(double *a,int n,double *v,double eps,int jt);
//int bmuav(double *a,int m,int n,double *u,double *v,double eps,int ka);
//void ppp(double *a,double *e,double *s,double *v,int m,int n);
double bsdet(double *a,int n);
int brank(double *a,int m,int n);
int agaus(double *a,double *b,int n);
int brinv(double *a,int n);



//为了求解广义逆而加入：
int agmiv(double *a,int m, int n,double *b,double *x,
		  double *aa,double eps,double *u,double *v,int  ka);

int bginv(double *a,int m,int n,double *aa,double eps,double *u,double *v,int ka);

int bmuav(double *a,int m,int n,double *u,double *v,double eps,int ka);

void ppp(double *a,double *e,double *s,double *v,int m,int n);

void sss(double *fg,double *cs);





