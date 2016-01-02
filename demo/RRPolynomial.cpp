//-------------------------------------------------------------------=72
//
// Copyright (C) Columbia University, 1998-1999. All Rights Reserved.
//
//-------------------------------------------------------------------=72
//
// RRPolynomial.cpp
//
//-------------------------------------------------------------------=72
//
// Author:				 Tomoo Mitsunaga
//
// Version:              1.0
//
// Modification History:
//  Oct/31/1998:	Copied from eldPolynomial.cpp
//
// Bugs:
//
//-------------------------------------------------------------------=72

#include "RRPolynomial.h"

#include <math.h>

const float RRPLY_D_PRECISION = 1.e-12f;

float RRPolynomial::mfGetY(
			const float x)const
{
	float y=0.;
//	for(int i=0;i<mCoeff.getSize();i++)y+=mCoeff[i]*(float)pow(x,i);
	for(int i=mCoeff.getSize()-1;i>=0;i--)y=y*x+mCoeff[i];
	return y;
}

RRPolynomial RRPolynomial::mfGetDerivative(
			const int order,
			const float precision)const
{
	assert(order>=0);

	if(order==0)
		return *this;

	int numcoeff=mCoeff.getSize()-1;
	while(numcoeff>0 && fabs(mCoeff[numcoeff])<precision)
		numcoeff--;		

	RRPolynomial ret;
	ret.mCoeff.setSize(numcoeff);
	for(int i=0;i<numcoeff;i++)
		ret.mCoeff[i]=mCoeff[i+1]*(i+1);

	if(order==1)
		return ret;
	else
		return ret.mfGetDerivative(order-1);
}

float RRPolynomial::mfFindIntersection(
			const float x0,
            const float x1,
			const float y,
			const float precision)const
{
	float div;
	if(x0== -HUGE_VAL)
	{
		if(x1==HUGE_VAL)
			div=0.f;
		else
			div=x1-1.f;
	}
	else if(x1==HUGE_VAL)
		div=x0+1.f;
	else
		div=(x0+x1)*0.5f;

	float ty=mfGetY(div);

	if(fabs(ty-y)<precision)
		return div;
	else
	{
		int odd=((mCoeff.getSize()-1)%2==1);
		int plus=(mCoeff[mCoeff.getSize()-1]>0);
		int increasing=((x0==(-HUGE_VAL) && ((odd && plus) || (!odd && !plus))) ||
		                (x1==HUGE_VAL && ((odd && plus) || (!odd && plus))) || 
						(mfGetY(x1)-mfGetY(x0)>0));
	
		if((ty-y>0 && increasing) || (ty-y<0 && !increasing))
			return mfFindIntersection(x0,div,y,precision);
		else
			return mfFindIntersection(div,x1,y,precision);
	}
}

RRArray< float > RRPolynomial::mfGetX(
			const float y,
			const float precision)const
{
	RRArray< float > ret;

	if(mCoeff.getSize()==1)
	{
		ret.setSize(0);
	}
	else if(mCoeff.getSize()==2)
	{
		assert(mCoeff[1]!=0);
		ret.setSize(1);
		ret[0]=(y-mCoeff[0])/mCoeff[1];
	}
	else
	{
		int i;
		
		const RRPolynomial grad=mfGetDerivative(1);

		RRArray< float > minmax=grad.mfGetX(0,precision);
		RRArray< float > xx(minmax.getSize()+1);

		for(i=0;i<xx.getSize();i++)
		{
			int odd=((mCoeff.getSize()-1)%2==1);
			int plus=(mCoeff[mCoeff.getSize()-1]>0);

			float y0=(i==0)?(((odd && plus) || (!odd && !plus))?(-1):1):mfGetY(minmax[i-1])-y;
			float y1=(i==xx.getSize()-1)?(((odd && plus) || (!odd && plus))?1:(-1)):mfGetY(minmax[i])-y;
			if(fabs(y1)<precision)
			{
				if(fabs(y0)>=precision)
					xx[i]=minmax[i];
				else
					xx[i]=(float)HUGE_VAL;
			}
			else if(fabs(y0)<precision)
			{
				xx[i]=(float)HUGE_VAL;
			}
			else if(y0*y1<0)
			{
				float x0=(i==0)?(-(float)HUGE_VAL):minmax[i-1];
				float x1=(i==xx.getSize()-1)?((float)HUGE_VAL):minmax[i];
				xx[i]=mfFindIntersection(x0,x1,y,precision);
			}
			else
				xx[i]=(float)HUGE_VAL;
		}
		
		ret.setSize(xx.getSize());
		int count=0;
		for(i=0;i<xx.getSize();i++)
			if(xx[i]!=(float)HUGE_VAL)
			{
				ret[count]=xx[i];
				count++;
			}
		ret.setSize(count);			
	}
	
	return ret;
}
/*
RRPolynomial::RRPolynomial()
:mCoeff(0)
{}


RRPolynomial::RRPolynomial(
			const RRArray< float >& coeff)
:mCoeff(coeff)
{}

RRPolynomial::RRPolynomial(
			const RRPolynomial& x)
:mCoeff(x.mCoeff)
{}

RRPolynomial::~RRPolynomial()
{}

const RRPolynomial& RRPolynomial::operator=(
			const RRPolynomial& x)
{
	mCoeff=x.mCoeff;
	return *this;
}

int RRPolynomial::getOrder()const
{
	return mCoeff.getSize()-1;
}

const RRArray< float >& RRPolynomial::getCoeff()const
{
	return mCoeff;
}


float RRPolynomial::getY(
			const float x)const
{
	return mfGetY(x);
}


RRArray< float > RRPolynomial::getX(
			const float y,
			const float precision)const
{
	return mfGetX(y,precision);
}


RRPolynomial RRPolynomial::getDerivative(
			const int order,
			const float precision)const
{
	assert(order>=0);
	return mfGetDerivative(order);
}
*/

bool operator==(const RRPolynomial& p1,const RRPolynomial& p2)
{
	return (p1.mCoeff==p2.mCoeff)?true:false;
}

bool operator!=(const RRPolynomial& p1,const RRPolynomial& p2)
{
	return (p1.mCoeff!=p2.mCoeff)?true:false;
}

//-------------------------------------------------------------------=72
// End of RRPolynomial.cpp
//-------------------------------------------------------------------=72
