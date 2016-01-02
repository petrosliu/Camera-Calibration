#include"stdafx.h"
#include"MatrixApp.h"


//奇异值分解：
  void ppp(double a[],double e[],double s[],double v[],int m,int n)
  //int m,n;
  //double a[],e[],s[],v[];
  { int i,j,p,q;
    double d;
    if (m>=n) i=n;
    else i=m;
    for (j=1; j<=i-1; j++)
      { a[(j-1)*n+j-1]=s[j-1];
        a[(j-1)*n+j]=e[j-1];
      }
    a[(i-1)*n+i-1]=s[i-1];
    if (m<n) a[(i-1)*n+i]=e[i-1];
    for (i=1; i<=n-1; i++)
    for (j=i+1; j<=n; j++)
      { p=(i-1)*n+j-1; q=(j-1)*n+i-1;
        d=v[p]; v[p]=v[q]; v[q]=d;
      }
    return;
  }

   void sss(double fg[2],double cs[2])
 // double cs[2],fg[2];
  { double r,d;
    if ((fabs(fg[0])+fabs(fg[1]))==0.0)
      { cs[0]=1.0; cs[1]=0.0; d=0.0;}
    else 
      { d=sqrt(fg[0]*fg[0]+fg[1]*fg[1]);
        if (fabs(fg[0])>fabs(fg[1]))
          { d=fabs(d);
            if (fg[0]<0.0) d=-d;
          }
        if (fabs(fg[1])>=fabs(fg[0]))
          { d=fabs(d);
            if (fg[1]<0.0) d=-d;
          }
        cs[0]=fg[0]/d; cs[1]=fg[1]/d;
      }
    r=1.0;
    if (fabs(fg[0])>fabs(fg[1])) r=cs[1];
    else
      if (cs[0]!=0.0) r=1.0/cs[0];
    fg[0]=d; fg[1]=r;
    return;
  }


 int bmuav(double a[],int m,int n,double u[],double v[],double eps,int ka)
 //int m,n,ka;
  //double eps,a[],u[],v[];
  { int i,j,k,l,it,ll,kk,ix,iy,mm,nn,iz,m1,ks;
    double d,dd,t,sm,sm1,em1,sk,ek,b,c,shh,fg[2],cs[2];
    double *s,*e,*w;
    //void ppp();
    //void sss();
    s=(double *)malloc(ka*sizeof(double));
    e=(double *)malloc(ka*sizeof(double));
    w=(double *)malloc(ka*sizeof(double));
    it=60; k=n;
    if (m-1<n) k=m-1;
    l=m;
    if (n-2<m) l=n-2;
    if (l<0) l=0;
    ll=k;
    if (l>k) ll=l;
    if (ll>=1)
      { for (kk=1; kk<=ll; kk++)
          { if (kk<=k)
              { d=0.0;
                for (i=kk; i<=m; i++)
                  { ix=(i-1)*n+kk-1; d=d+a[ix]*a[ix];}
                s[kk-1]=sqrt(d);
                if (s[kk-1]!=0.0)
                  { ix=(kk-1)*n+kk-1;
                    if (a[ix]!=0.0)
                      { s[kk-1]=fabs(s[kk-1]);
                        if (a[ix]<0.0) s[kk-1]=-s[kk-1];
                      }
                    for (i=kk; i<=m; i++)
                      { iy=(i-1)*n+kk-1;
                        a[iy]=a[iy]/s[kk-1];
                      }
                    a[ix]=1.0+a[ix];
                  }
                s[kk-1]=-s[kk-1];
              }
            if (n>=kk+1)
              { for (j=kk+1; j<=n; j++)
                  { if ((kk<=k)&&(s[kk-1]!=0.0))
                      { d=0.0;
                        for (i=kk; i<=m; i++)
                          { ix=(i-1)*n+kk-1;
                            iy=(i-1)*n+j-1;
                            d=d+a[ix]*a[iy];
                          }
                        d=-d/a[(kk-1)*n+kk-1];
                        for (i=kk; i<=m; i++)
                          { ix=(i-1)*n+j-1;
                            iy=(i-1)*n+kk-1;
                            a[ix]=a[ix]+d*a[iy];
                          }
                      }
                    e[j-1]=a[(kk-1)*n+j-1];
                  }
              }
            if (kk<=k)
              { for (i=kk; i<=m; i++)
                  { ix=(i-1)*m+kk-1; iy=(i-1)*n+kk-1;
                    u[ix]=a[iy];
                  }
              }
            if (kk<=l)
              { d=0.0;
                for (i=kk+1; i<=n; i++)
                  d=d+e[i-1]*e[i-1];
                e[kk-1]=sqrt(d);
                if (e[kk-1]!=0.0)
                  { if (e[kk]!=0.0)
                      { e[kk-1]=fabs(e[kk-1]);
                        if (e[kk]<0.0) e[kk-1]=-e[kk-1];
                      }
                    for (i=kk+1; i<=n; i++)
                      e[i-1]=e[i-1]/e[kk-1];
                    e[kk]=1.0+e[kk];
                  }
                e[kk-1]=-e[kk-1];
                if ((kk+1<=m)&&(e[kk-1]!=0.0))
                  { for (i=kk+1; i<=m; i++) w[i-1]=0.0;
                    for (j=kk+1; j<=n; j++)
                      for (i=kk+1; i<=m; i++)
                        w[i-1]=w[i-1]+e[j-1]*a[(i-1)*n+j-1];
                    for (j=kk+1; j<=n; j++)
                      for (i=kk+1; i<=m; i++)
                        { ix=(i-1)*n+j-1;
                          a[ix]=a[ix]-w[i-1]*e[j-1]/e[kk];
                        }
                  }
                for (i=kk+1; i<=n; i++)
                  v[(i-1)*n+kk-1]=e[i-1];
              }
          }
      }
    mm=n;
    if (m+1<n) mm=m+1;
    if (k<n) s[k]=a[k*n+k];
    if (m<mm) s[mm-1]=0.0;
    if (l+1<mm) e[l]=a[l*n+mm-1];
    e[mm-1]=0.0;
    nn=m;
    if (m>n) nn=n;
    if (nn>=k+1)
      { for (j=k+1; j<=nn; j++)
          { for (i=1; i<=m; i++)
              u[(i-1)*m+j-1]=0.0;
            u[(j-1)*m+j-1]=1.0;
          }
      }
    if (k>=1)
      { for (ll=1; ll<=k; ll++)
          { kk=k-ll+1; iz=(kk-1)*m+kk-1;
            if (s[kk-1]!=0.0)
              { if (nn>=kk+1)
                  for (j=kk+1; j<=nn; j++)
                    { d=0.0;
                      for (i=kk; i<=m; i++)
                        { ix=(i-1)*m+kk-1;
                          iy=(i-1)*m+j-1;
                          d=d+u[ix]*u[iy]/u[iz];
                        }
                      d=-d;
                      for (i=kk; i<=m; i++)
                        { ix=(i-1)*m+j-1;
                          iy=(i-1)*m+kk-1;
                          u[ix]=u[ix]+d*u[iy];
                        }
                    }
                  for (i=kk; i<=m; i++)
                    { ix=(i-1)*m+kk-1; u[ix]=-u[ix];}
                  u[iz]=1.0+u[iz];
                  if (kk-1>=1)
                    for (i=1; i<=kk-1; i++)
                      u[(i-1)*m+kk-1]=0.0;
              }
            else
              { for (i=1; i<=m; i++)
                  u[(i-1)*m+kk-1]=0.0;
                u[(kk-1)*m+kk-1]=1.0;
              }
          }
      }
    for (ll=1; ll<=n; ll++)
      { kk=n-ll+1; iz=kk*n+kk-1;
        if ((kk<=l)&&(e[kk-1]!=0.0))
          { for (j=kk+1; j<=n; j++)
              { d=0.0;
                for (i=kk+1; i<=n; i++)
                  { ix=(i-1)*n+kk-1; iy=(i-1)*n+j-1;
                    d=d+v[ix]*v[iy]/v[iz];
                  }
                d=-d;
                for (i=kk+1; i<=n; i++)
                  { ix=(i-1)*n+j-1; iy=(i-1)*n+kk-1;
                    v[ix]=v[ix]+d*v[iy];
                  }
              }
          }
        for (i=1; i<=n; i++)
          v[(i-1)*n+kk-1]=0.0;
        v[iz-n]=1.0;
      }
    for (i=1; i<=m; i++)
    for (j=1; j<=n; j++)
      a[(i-1)*n+j-1]=0.0;
    m1=mm; it=60;
    while (1==1)
      { if (mm==0)
          { ppp(a,e,s,v,m,n);
            free(s); free(e); free(w); return(1);
          }
        if (it==0)
          { ppp(a,e,s,v,m,n);
            free(s); free(e); free(w); return(-1);
          }
        kk=mm-1;
	while ((kk!=0)&&(fabs(e[kk-1])!=0.0))
          { d=fabs(s[kk-1])+fabs(s[kk]);
            dd=fabs(e[kk-1]);
            if (dd>eps*d) kk=kk-1;
            else e[kk-1]=0.0;
          }
        if (kk==mm-1)
          { kk=kk+1;
            if (s[kk-1]<0.0)
              { s[kk-1]=-s[kk-1];
                for (i=1; i<=n; i++)
                  { ix=(i-1)*n+kk-1; v[ix]=-v[ix];}
              }
            while ((kk!=m1)&&(s[kk-1]<s[kk]))
              { d=s[kk-1]; s[kk-1]=s[kk]; s[kk]=d;
                if (kk<n)
                  for (i=1; i<=n; i++)
                    { ix=(i-1)*n+kk-1; iy=(i-1)*n+kk;
                      d=v[ix]; v[ix]=v[iy]; v[iy]=d;
                    }
                if (kk<m)
                  for (i=1; i<=m; i++)
                    { ix=(i-1)*m+kk-1; iy=(i-1)*m+kk;
                      d=u[ix]; u[ix]=u[iy]; u[iy]=d;
                    }
                kk=kk+1;
              }
            it=60;
            mm=mm-1;
          }
        else
          { ks=mm;
            while ((ks>kk)&&(fabs(s[ks-1])!=0.0))
              { d=0.0;
                if (ks!=mm) d=d+fabs(e[ks-1]);
                if (ks!=kk+1) d=d+fabs(e[ks-2]);
                dd=fabs(s[ks-1]);
                if (dd>eps*d) ks=ks-1;
                else s[ks-1]=0.0;
              }
            if (ks==kk)
              { kk=kk+1;
                d=fabs(s[mm-1]);
                t=fabs(s[mm-2]);
                if (t>d) d=t;
                t=fabs(e[mm-2]);
                if (t>d) d=t;
                t=fabs(s[kk-1]);
                if (t>d) d=t;
                t=fabs(e[kk-1]);
                if (t>d) d=t;
                sm=s[mm-1]/d; sm1=s[mm-2]/d;
                em1=e[mm-2]/d;
                sk=s[kk-1]/d; ek=e[kk-1]/d;
                b=((sm1+sm)*(sm1-sm)+em1*em1)/2.0;
                c=sm*em1; c=c*c; shh=0.0;
                if ((b!=0.0)||(c!=0.0))
                  { shh=sqrt(b*b+c);
                    if (b<0.0) shh=-shh;
                    shh=c/(b+shh);
                  }
                fg[0]=(sk+sm)*(sk-sm)-shh;
                fg[1]=sk*ek;
                for (i=kk; i<=mm-1; i++)
                  { sss(fg,cs);
                    if (i!=kk) e[i-2]=fg[0];
                    fg[0]=cs[0]*s[i-1]+cs[1]*e[i-1];
                    e[i-1]=cs[0]*e[i-1]-cs[1]*s[i-1];
                    fg[1]=cs[1]*s[i];
                    s[i]=cs[0]*s[i];
                    if ((cs[0]!=1.0)||(cs[1]!=0.0))
                      for (j=1; j<=n; j++)
                        { ix=(j-1)*n+i-1;
                          iy=(j-1)*n+i;
                          d=cs[0]*v[ix]+cs[1]*v[iy];
                          v[iy]=-cs[1]*v[ix]+cs[0]*v[iy];
                          v[ix]=d;
                        }
                    sss(fg,cs);
                    s[i-1]=fg[0];
                    fg[0]=cs[0]*e[i-1]+cs[1]*s[i];
                    s[i]=-cs[1]*e[i-1]+cs[0]*s[i];
                    fg[1]=cs[1]*e[i];
                    e[i]=cs[0]*e[i];
                    if (i<m)
                      if ((cs[0]!=1.0)||(cs[1]!=0.0))
                        for (j=1; j<=m; j++)
                          { ix=(j-1)*m+i-1;
                            iy=(j-1)*m+i;
                            d=cs[0]*u[ix]+cs[1]*u[iy];
                            u[iy]=-cs[1]*u[ix]+cs[0]*u[iy];
                            u[ix]=d;
                          }
                  }
                e[mm-2]=fg[0];
                it=it-1;
              }
            else
              { if (ks==mm)
                  { kk=kk+1;
                    fg[1]=e[mm-2]; e[mm-2]=0.0;
                    for (ll=kk; ll<=mm-1; ll++)
                      { i=mm+kk-ll-1;
                        fg[0]=s[i-1];
                        sss(fg,cs);
                        s[i-1]=fg[0];
                        if (i!=kk)
                          { fg[1]=-cs[1]*e[i-2];
                            e[i-2]=cs[0]*e[i-2];
                          }
                        if ((cs[0]!=1.0)||(cs[1]!=0.0))
                          for (j=1; j<=n; j++)
                            { ix=(j-1)*n+i-1;
                              iy=(j-1)*n+mm-1;
                              d=cs[0]*v[ix]+cs[1]*v[iy];
                              v[iy]=-cs[1]*v[ix]+cs[0]*v[iy];
                              v[ix]=d;
                            }
                      }
                  }
                else
                  { kk=ks+1;
                    fg[1]=e[kk-2];
                    e[kk-2]=0.0;
                    for (i=kk; i<=mm; i++)
                      { fg[0]=s[i-1];
                        sss(fg,cs);
                        s[i-1]=fg[0];
                        fg[1]=-cs[1]*e[i-1];
                        e[i-1]=cs[0]*e[i-1];
                        if ((cs[0]!=1.0)||(cs[1]!=0.0))
                          for (j=1; j<=m; j++)
                            { ix=(j-1)*m+i-1;
                              iy=(j-1)*m+kk-2;
                              d=cs[0]*u[ix]+cs[1]*u[iy];
                              u[iy]=-cs[1]*u[ix]+cs[0]*u[iy];
                              u[ix]=d;
                            }
                      }
                  }
              }
          }
      }
    return(1);
  }

 
//求广义逆的奇异值分解法：

  int bginv(double a[],int m,int n,double aa[],double eps,double u[],double v[],int ka)
  //int m,n,ka;
  //double a[],aa[],u[],v[],eps;
  {
	 int i,j,k,l,t,p,q,f;
    // int bmuav();
	//int bmuav(double a[],int m,int n,double u[],double v[],double eps,int ka);

    i=bmuav(a,m,n,u,v,eps,ka);
    if (i<0) return(-1);
    j=n;
    if (m<n) j=m;
    j=j-1;
    k=0;
    while ((k<=j)&&(a[k*n+k]!=0.0)) k=k+1;
    k=k-1;
    for (i=0; i<=n-1; i++)
    for (j=0; j<=m-1; j++)
      { t=i*m+j; aa[t]=0.0;
        for (l=0; l<=k; l++)
          { f=l*n+i; p=j*m+l; q=l*n+l;
            aa[t]=aa[t]+v[f]*u[p]/a[q];
          }
      }
    return(1);
  }

//求解线性最小二乘问题的广义逆法：

 int agmiv(double a[],int m,int n,double b[],double x[],double aa[],double eps,double u[],double v[],int ka)
  //int m,n,ka;
  //double a[],b[],x[],aa[],u[],v[],eps;
  { int i,j;
    
    //int bginv( );
   //int bginv(double a[],int m,int n,double aa[],double eps,double u[],double v[],int ka);
    i=bginv(a,m,n,aa,eps,u,v,ka);
    if (i<0) return(-1);
    for (i=0; i<=n-1; i++)
      { x[i]=0.0;
        for (j=0; j<=m-1; j++)
          x[i]=x[i]+aa[i*m+j]*b[j];
      }
    return(1);
  }



  void cjcbj( double a[],int n,double v[], double eps)
  //int n;
  //double a[],v[],eps;
  { int i,j,p,q,u,w,t,s;
    double ff,fm,cn,sn,omega,x,y,d;
    for (i=0; i<=n-1; i++)
      { v[i*n+i]=1.0;
        for (j=0; j<=n-1; j++)
          if (i!=j) v[i*n+j]=0.0;
      }
    ff=0.0;
    for (i=1; i<=n-1; i++)
    for (j=0; j<=i-1; j++)
      { d=a[i*n+j]; ff=ff+d*d; }
    ff=sqrt(2.0*ff);
    loop0:
    ff=ff/(1.0*n);
    loop1:
        for (i=1; i<=n-1; i++)
        for (j=0; j<=i-1; j++)
          { d=fabs(a[i*n+j]);
            if (d>ff)
              { p=i; q=j;
                goto loop;
              }
          }
        if (ff<eps) return;
        goto loop0;
  loop: u=p*n+q; w=p*n+p; t=q*n+p; s=q*n+q;
        x=-a[u]; y=(a[s]-a[w])/2.0;
        omega=x/sqrt(x*x+y*y);
        if (y<0.0) omega=-omega;
        sn=1.0+sqrt(1.0-omega*omega);
        sn=omega/sqrt(2.0*sn);
        cn=sqrt(1.0-sn*sn);
        fm=a[w];
        a[w]=fm*cn*cn+a[s]*sn*sn+a[u]*omega;
        a[s]=fm*sn*sn+a[s]*cn*cn-a[u]*omega;
        a[u]=0.0; a[t]=0.0;
        for (j=0; j<=n-1; j++)
        if ((j!=p)&&(j!=q))
          { u=p*n+j; w=q*n+j;
            fm=a[u];
            a[u]=fm*cn+a[w]*sn;
            a[w]=-fm*sn+a[w]*cn;
          }
        for (i=0; i<=n-1; i++)
          if ((i!=p)&&(i!=q))
            { u=i*n+p; w=i*n+q;
              fm=a[u];
              a[u]=fm*cn+a[w]*sn;
              a[w]=-fm*sn+a[w]*cn;
            }
        for (i=0; i<=n-1; i++)
          { u=i*n+p; w=i*n+q;
            fm=v[u];
            v[u]=fm*cn+v[w]*sn;
            v[w]=-fm*sn+v[w]*cn;
          }
       goto loop1;
  }


  int cjcbi(long double a[],int n,long double v[],long double eps,int jt)
  //int n,jt;
  //double a[],v[],eps;
  { int i,j,p,q,u,w,t,s,l;
    double fm,cn,sn,omega,x,y,d;
    l=1;
    for (i=0; i<=n-1; i++)
      { v[i*n+i]=1.0;
        for (j=0; j<=n-1; j++)
          if (i!=j) v[i*n+j]=0.0;
      }
    while (1==1)
      { fm=0.0;
        for (i=1; i<=n-1; i++)
        for (j=0; j<=i-1; j++)
          { d=fabs(a[i*n+j]);
            if ((i!=j)&&(d>fm))
              { fm=d; p=i; q=j;}
          }
        if (fm<eps)  return(1);
        if (l>jt)  return(-1);
        l=l+1;
        u=p*n+q; w=p*n+p; t=q*n+p; s=q*n+q;
        x=-a[u]; y=(a[s]-a[w])/2.0;
        omega=x/sqrt(x*x+y*y);
        if (y<0.0) omega=-omega;
        sn=1.0+sqrt(1.0-omega*omega);
        sn=omega/sqrt(2.0*sn);
        cn=sqrt(1.0-sn*sn);
        fm=a[w];
        a[w]=fm*cn*cn+a[s]*sn*sn+a[u]*omega;
        a[s]=fm*sn*sn+a[s]*cn*cn-a[u]*omega;
        a[u]=0.0; a[t]=0.0;
        for (j=0; j<=n-1; j++)
        if ((j!=p)&&(j!=q))
          { u=p*n+j; w=q*n+j;
            fm=a[u];
            a[u]=fm*cn+a[w]*sn;
            a[w]=-fm*sn+a[w]*cn;
          }
        for (i=0; i<=n-1; i++)
          if ((i!=p)&&(i!=q))
            { u=i*n+p; w=i*n+q;
              fm=a[u];
              a[u]=fm*cn+a[w]*sn;
              a[w]=-fm*sn+a[w]*cn;
            }
        for (i=0; i<=n-1; i++)
          { u=i*n+p; w=i*n+q;
            fm=v[u];
            v[u]=fm*cn+v[w]*sn;
            v[w]=-fm*sn+v[w]*cn;
          }
      }
    return(1);
  }

 int csstq(int n,double b[],double c[],double q[] ,double eps,int l)
  { int i,j,k,m,it,u,v;
    double d,f,h,g,p,r,e,s;
    c[n-1]=0.0; d=0.0; f=0.0;
    for (j=0; j<=n-1; j++)
      { it=0;
        h=eps*(fabs(b[j])+fabs(c[j]));
        if (h>d) d=h;
        m=j;
        while ((m<=n-1)&&(fabs(c[m])>d)) m=m+1;
        if (m!=j)
          { do
              { if (it==l)
                  { printf("fail\n");
                    return(-1);
                  }
                it=it+1;
                g=b[j];
                p=(b[j+1]-g)/(2.0*c[j]);
                r=sqrt(p*p+1.0);
                if (p>=0.0) b[j]=c[j]/(p+r);
                else b[j]=c[j]/(p-r);
                h=g-b[j];
                for (i=j+1; i<=n-1; i++)
                  b[i]=b[i]-h;
                f=f+h; p=b[m]; e=1.0; s=0.0;
                for (i=m-1; i>=j; i--)
                  { g=e*c[i]; h=e*p;
                    if (fabs(p)>=fabs(c[i]))
                      { e=c[i]/p; r=sqrt(e*e+1.0);
                        c[i+1]=s*p*r; s=e/r; e=1.0/r;
                      }
                    else
		      { e=p/c[i]; r=sqrt(e*e+1.0);
                        c[i+1]=s*c[i]*r;
                        s=1.0/r; e=e/r;
                      }
                    p=e*b[i]-s*g;
                    b[i+1]=h+s*(e*g+s*b[i]);
                    for (k=0; k<=n-1; k++)
                      { u=k*n+i+1; v=u-1;
                        h=q[u]; q[u]=s*q[v]+e*h;
                        q[v]=e*q[v]-s*h;
                      }
                  }
                c[j]=s*p; b[j]=e*p;
              }
            while (fabs(c[j])>d);
          }
        b[j]=b[j]+f;
      }
    for (i=0; i<=n-1; i++)
      { k=i; p=b[i];
        if (i+1<=n-1)
          { j=i+1;
            while ((j<=n-1)&&(b[j]<=p))
              { k=j; p=b[j]; j=j+1;}
          }
        if (k!=i)
          { b[k]=b[i]; b[i]=p;
            for (j=0; j<=n-1; j++)
              { u=j*n+i; v=j*n+k;
                p=q[u]; q[u]=q[v]; q[v]=p;
              }
          }
      }
    return(1);
  }





void cstrq(double a[],int n,double q[],double b[],double c[])
{ int i,j,k,u,v;
    double h,f,g,h2;
    for (i=0; i<=n-1; i++)
    for (j=0; j<=n-1; j++)
      { u=i*n+j; q[u]=a[u];}
    for (i=n-1; i>=1; i--)
      { h=0.0;
        if (i>1)
          for (k=0; k<=i-1; k++)
            { u=i*n+k; h=h+q[u]*q[u];}
        if (h+1.0==1.0)
          { c[i]=0.0;
            if (i==1) c[i]=q[i*n+i-1];
            b[i]=0.0;
          }
        else
          { c[i]=sqrt(h);
            u=i*n+i-1;
            if (q[u]>0.0) c[i]=-c[i];
            h=h-q[u]*c[i];
            q[u]=q[u]-c[i];
            f=0.0;
            for (j=0; j<=i-1; j++)
              { q[j*n+i]=q[i*n+j]/h;
                g=0.0;
                for (k=0; k<=j; k++)
                  g=g+q[j*n+k]*q[i*n+k];
                if (j+1<=i-1)
                  for (k=j+1; k<=i-1; k++)
                    g=g+q[k*n+j]*q[i*n+k];
                c[j]=g/h;
                f=f+g*q[j*n+i];
              }
            h2=f/(h+h);
            for (j=0; j<=i-1; j++)
              { f=q[i*n+j];
                g=c[j]-h2*f;
                c[j]=g;
                for (k=0; k<=j; k++)
                  { u=j*n+k;
                    q[u]=q[u]-f*c[k]-g*q[i*n+k];
                  }
              }
            b[i]=h;
          }
      }
    for (i=0; i<=n-2; i++) c[i]=c[i+1];
    c[n-1]=0.0;
    b[0]=0.0;
    for (i=0; i<=n-1; i++)
      { if ((b[i]!=0.0)&&(i-1>=0))
          for (j=0; j<=i-1; j++)
            { g=0.0;
              for (k=0; k<=i-1; k++)
                g=g+q[i*n+k]*q[k*n+j];
              for (k=0; k<=i-1; k++)
                { u=k*n+j;
                  q[u]=q[u]-g*q[k*n+i];
                }
            }
        u=i*n+i;
        b[i]=q[u]; q[u]=1.0;
        if (i-1>=0)
          for (j=0; j<=i-1; j++)
            { q[i*n+j]=0.0; q[j*n+i]=0.0;}
      }
    return;
  
}




void  eig(double a[],double q[],double b[],int n)
 //a---matrix 
 //q---eigenvectors
 //b---eigenvalues
{   int i,j,k,l=60;
    //static double q[5][5],b[5],c[5];
    //static double a[5][5]={ {10.0,1.0,2.0,3.0,4.0},
    //  {1.0,9.0,-1.0,2.0,-3.0},{2.0,-1.0,7.0,3.0,-5.0},
    //  {3.0,2.0,3.0,12.0,-1.0},{4.0,-3.0,-5.0,-1.0,15.0}};
    //static double  c[9];
    double *c=(double *)malloc(n*sizeof(double));
    double eps=0.0000000001;
    cstrq(a,n,q,b,c);
    k=csstq(n,b,c,q,eps,l);
	
	/*
    printf("MAT A IS:\n");
    for (i=0; i<=4; i++)
      { for (j=0; j<=4; j++)
          printf("%13.7e ",a[i][j]);
        printf("\n");
      }
    printf("\n");
    if (k>0)
      { printf("MAT B IS:\n");
        for (i=0; i<=4; i++)
          printf("%13.7e ",b[i]);
        printf("\n\n");
        printf("MAT Q IS:\n");
        for (i=0; i<=4; i++)
          { for (j=0; j<=4; j++)
              printf("%13.7e ",q[i][j]);
            printf("\n");
          }
        printf("\n");
      }
*/
    
  }

//不使用计算特征值的办法，直接解方程：

