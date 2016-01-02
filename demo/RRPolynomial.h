//-------------------------------------------------------------------=72
//
// Copyright (C) Columbia University, 1998-1999. All Rights Reserved.
//
//-------------------------------------------------------------------=72
//
// RRPolynomial.h
//
//-------------------------------------------------------------------=72
//
// Author:				 Tomoo Mitsunaga
//
// Version:              1.0
//
// Modification History:
//  Oct/31/1998:	Copied from eldPolynomial.h
//
// Bugs:
//
//-------------------------------------------------------------------=72
//
// RRPolynomial class is a 1-dimenstional polynomial function.
//
// y = f(x) = c0 + c1*x + c2*xx + c3*xxx + ...
//
//-------------------------------------------------------------------=72

#ifndef _RR_POLYNOMIAL_H
#define _RR_POLYNOMIAL_H

#include "RRArray.h"

extern const float RRPLY_D_PRECISION; // default computing precision
									   // for getX() and getDerivative().	

class RRPolynomial
{
  private:

	RRArray< float > mCoeff;

  private:

	float mfGetY(
			const float x)const;
	RRArray< float > mfGetX(
			const float y,
			const float precision = RRPLY_D_PRECISION)const;
	RRPolynomial mfGetDerivative(
			const int order,
			const float precision = RRPLY_D_PRECISION)const;
	float mfFindIntersection(
			const float x0,
			const float x1,
			const float y,
			const float precision = RRPLY_D_PRECISION)const;

  public:

	  RRPolynomial():mCoeff(0){}
	RRPolynomial(const RRArray< float >& coeff):mCoeff(coeff){}
	// create polynomial with given coefficients.
	RRPolynomial(const RRPolynomial& x):mCoeff(x.mCoeff){}
	~RRPolynomial(){}

	const RRPolynomial& operator=(const RRPolynomial& x)
	{
		mCoeff=x.mCoeff;
		return *this;
	}

	int getOrder()const
	{return mCoeff.getSize()-1;}
	
	const RRArray< float >& getCoeff()const
	{return mCoeff;}

	float getY(const float x)const
	{return mfGetY(x);}
	// return y = f(x)

	RRArray< float > getX(const float y,const float precision = RRPLY_D_PRECISION)const
	{return mfGetX(y,precision);}
	// return x = f_inverse(y).
	// x = f_inverce(y) may have multiple solution. return values are
	// aligned with increasing order.

	RRPolynomial getDerivative(const int order,const float precision = RRPLY_D_PRECISION)const
	{return mfGetDerivative(order);}
	// return (order)-th derivative of the polynomial.

	friend bool operator==(const RRPolynomial& p1,const RRPolynomial& p2);
	friend bool operator!=(const RRPolynomial& p1,const RRPolynomial& p2);
};

#endif // _RR_POLYNOMIAL_H

//-------------------------------------------------------------------=72
// End of RRPolynomial.h
//-------------------------------------------------------------------=72

