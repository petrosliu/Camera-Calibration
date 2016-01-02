//-------------------------------------------------------------------=72
//
// Copyright (C) Columbia University, 1998-1999. All Rights Reserved.
//
//-------------------------------------------------------------------=72
//
// RRArray.h
//
//-------------------------------------------------------------------=72
//
// Author:				 Tomoo Mitsunaga
//
// Version:              1.1
//
// Modification History:
//  Oct/27/1998:	Copied from eldArray.h
//  Jan/26/1999:	Added RRArray(const int numelem,const T initval)
//
// Bugs:
//
//-------------------------------------------------------------------=72
//
// This file defines RRArray class.
// RRArray class is a resizable array class template.
//
//-------------------------------------------------------------------=72

#ifndef _RR_ARRAY_H
#define _RR_ARRAY_H
#include <stdlib.h>
#include <assert.h>

template <class T>
class RRArray
{
  private:

    T* mElements;
    int mNumElements;

  public:

	// SIZE INQUIRIES
	
    void setSize(int size);
    // Resizes the array and returns RRSUCCESS.
    // When it cannot resize, it is aborted by assert().
    
    int getSize()const{return mNumElements;}
	// returns the current size. 
		
    // ARRAY LIKE INTERFACES
	
 	T& operator[](int index) {return mElements[index];}
    // When index is over the range, it is aborted by assert().
    
    const T& operator[](int index)const {return mElements[index];}
	// When index is over the range, it is aborted by assert(). 
	
    // LIST LIKE INTERFACES
    
    int insert(int index, const T& x);
    // inserts x as array[index] and returns mNumElements.
    // When index<0 or index>mNumElements, it is aborted by assert().
    
    int remove(int index);
    // remove array[index] and returns mNumElements.
    // When index<0 or index>=mNumElements, it is aborted by assert().

    // CUT, COPY & PASTE
    
    int cut(int idx0, int idx1, RRArray<T>& x);
    // cut mElements array[idx0] - array[idx1], then the mElements are copyed to x.
    // returns mNumElements.
    // When idx0>idx1 or indecies are over the range, it is aborted by assert().
     
    int copy(int idx0, int idx1, RRArray<T>& x) const;
    // copys mElements array[idx0] - array[idx1] to x,
    // and returns mNumElements (mNumElements does not change).
    // When idx0>idx1 or indecies are over the range, it is aborted by assert().
     
    int paste(int idx0, int idx1, const RRArray<T>& x);
    // replace mElements array[idx0] - array[idx1] with x,
    // and returns mNumElements.
    // When idx0>idx1 or indecies are over the range, it is aborted by assert().     

    const RRArray<T>& operator=(const RRArray<T>& x);
    // operator=.

    RRArray(const int numelem=0);
    RRArray(const int numelem,const T initval);
    RRArray(const RRArray<T>& x);
    ~RRArray(){delete[] mElements;} 

	friend bool operator==(const RRArray<T>& a, const RRArray<T>& b);
	friend bool operator!=(const RRArray<T>& a, const RRArray<T>& b);
};

template <class T>
void RRArray<T>::setSize(int newsize)
{
    if(newsize!=getSize())
    {
		T* newelem= new T[newsize];
		assert(newelem!=0);

		int numcopy=(newsize<getSize())?newsize:getSize();
		for(int i=0;i<numcopy;i++)
	    	newelem[i]=mElements[i];
		delete[] mElements;
		mElements=newelem;
		mNumElements=newsize;
    }
}

template <class T>
const RRArray<T>& RRArray<T>::operator=(const RRArray<T>& x)
{
    if(this== &x)
	return *this;

    mNumElements=x.mNumElements;

    delete[] mElements;
    mElements=(mNumElements==0)?NULL:(new T[mNumElements]);
    //assert(mElements!=0);

    for(int i=0;i<mNumElements;i++)
	mElements[i]=x.mElements[i];
    
    return *this;
}

template <class T>
RRArray<T>::RRArray(
			const int numelem)
: mNumElements(numelem)
{
    mElements=(mNumElements==0)?NULL:(new T[mNumElements]);
}

template <class T>
RRArray<T>::RRArray(
			const int numelem,
			const T initval)
: mNumElements(numelem)
{
    mElements=(mNumElements==0)?NULL:(new T[mNumElements]);
	
    for(int i=0;i<mNumElements;i++)
		mElements[i]=initval;    
}

template <class T>
RRArray<T>::RRArray(const RRArray<T>& x)
: mNumElements(x.mNumElements)
{
    mElements=(mNumElements==0)?NULL:(new T[mNumElements]);

    for(int i=0;i<mNumElements;i++)
		mElements[i]=x.mElements[i];    
}

//-------------------------------------------------------------------
// list like interfaces

//template <class T>
//void RRArray<T>::append(const RRArray<T>& x)
//{
//    int newsize=mNumElements+x.mNumElements;
//    
//    // T* newelem= new T[newsize];
//    // passert(int(newelem),ASSERT_MALLOC);
//    T* newelem;
//    newelem=(newsize==0)?NULL:(new T[newsize]);
//
//    int j=0;
//
//    int numcopy=mNumElements;
//    for(int i=0;i<numcopy;i++)
//    {
//	newelem[j]=mElements[i];
//	j++;
//    }
//
//    numcopy=x.mNumElements;	
//    for(i=0;i<numcopy;i++)
//    {
//	newelem[j]=x.mElements[i];
//	j++;
//    }
//
//    delete[] mElements;
//    mElements=newelem;
//    mNumElements=newsize;
//
//    return;
//}

//template <class T>
//void RRArray<T>::split(int index, RRArray<T>& x)
//{
//    int newsize=index;
//    int splsize=mNumElements-newsize;
//
//    T* newelem;
//    T* splelem;
//    newelem=(newsize==0)?NULL:(new T[newsize]);
//    splelem=(splsize==0)?NULL:(new T[splsize]);
//
//    int j=0;
//
//    for(int i=0;i<newsize;i++)
//    {
//	newelem[i]=mElements[j];
//	j++;
//    }
//
//    for(i=0;i<splsize;i++)
//    {
//	splelem[i]=mElements[j];
//	j++;
//    }
//
//    delete[] mElements;
//    delete[] x.mElements;
//    mElements=newelem;
//    x.mElements=splelem;
//    mNumElements=newsize;
//    x.mNumElements=splsize;
//}

template <class T>
int RRArray<T>::insert(int index, const T& x)
{
    assert(index>=0 && index<=mNumElements);

    int newsize=mNumElements+1;

    T* newelem;
    newelem=(newsize==0)?NULL:(new T[newsize]);

	int i;
    for(i=0;i<index;i++)
		newelem[i]=mElements[i];
    newelem[i]=x;
    for(i=index;i<mNumElements;i++)
		newelem[i+1]=mElements[i];

    delete[] mElements;
    mElements=newelem;
    mNumElements=newsize;

    return mNumElements;
}

template <class T>
int RRArray<T>::remove(int index)
{
    assert(index>=0 && index<mNumElements);

    int newsize=mNumElements-1;
    
    T* newelem;
    newelem=(newsize==0)?NULL:(new T[newsize]);

	int i;
    for(i=0;i<index;i++)
		newelem[i]=mElements[i];
    for(i=index+1;i<mNumElements;i++)
		newelem[i-1]=mElements[i];

    delete[] mElements;
    mElements=newelem;
    mNumElements=newsize;

    return mNumElements;
}

//-------------------------------------------------------------------
// cut, copy & paste

template <class T>
int RRArray<T>::cut(int idx0, int idx1, RRArray<T>& x)
{
    assert(idx0>=0 && idx0<mNumElements);
    assert(idx1>=0 && idx1<mNumElements);
    assert(idx1>=idx0);

    int cutsize=idx1-idx0+1;
    int newsize=mNumElements-cutsize;

    T* newelem;
    T* cutelem;
    newelem=(newsize==0)?NULL:(new T[newsize]);
    cutelem=(cutsize==0)?NULL:(new T[cutsize]);

    int i=0,j=0;

    for(i=0;i<idx0;i++)
    {
		newelem[i]=mElements[j];
		j++;
    }

    for(i=0;i<cutsize;i++)
    {
		assert(i<cutsize);
		cutelem[i]=mElements[j];
		j++;
    }

    for(i=idx0;i<newsize;i++)
    {
		assert(i<newsize);
		newelem[i]=mElements[j];
		j++;
    }
    assert(j==mNumElements);

    delete[] mElements;
    delete[] x.mElements;
    mElements=newelem;
    x.mElements=cutelem;
    mNumElements=newsize;
    x.mNumElements=cutsize;

    return mNumElements;
}

template <class T>
int RRArray<T>::copy(int idx0, int idx1, RRArray<T>& x) const
{
    assert(idx0>=0 && idx0<mNumElements);
    assert(idx1>=0 && idx1<mNumElements);
    assert(idx1>=idx0);

    int copysize=idx1-idx0+1;

    T* copyelem;
    copyelem=(copysize==0)?NULL:(new T[copysize]);

    int i=0;
    for(i=0;i<copysize;i++)
		copyelem[i]=mElements[idx0+i];

    delete[] x.mElements;
    x.mElements=copyelem;
    x.mNumElements=copysize;

    return mNumElements;
}

template <class T>
int RRArray<T>::paste(int idx0, int idx1, const RRArray<T>& x)
{
    assert(idx0>=0 && idx0<mNumElements);
    assert(idx1>=0 && idx1<mNumElements);
    assert(idx1>=idx0);

    int newsize=mNumElements-(idx1-idx0+1)+x.mNumElements;

    T* newelem;
    newelem=(newsize==0)?NULL:(new T[newsize]);

    int i=0,j=0;
    for(i=0;i<idx0;i++)
    {
		newelem[j]=mElements[i];
		j++;
    }

    for(i=0;i<x.mNumElements;i++)
    {
		newelem[j]=x.mElements[i];
		j++;
    }

    for(i=idx1+1;i<mNumElements;i++)
    {
		newelem[j]=mElements[i];
		j++;
    }

    delete[] mElements;
    mElements=newelem;
    mNumElements=newsize;

    return mNumElements;
}

template <class T>
bool operator==(const RRArray<T>& a, const RRArray<T>& b)
{
	if(a.mNumElements!=b.mNumElements)
		return false;

	for(int i=0;i<a.mNumElements;i++)
		if(!(a.mElements[i]==b.mElements[i]))
			return false;

	return true;
}
	
template <class T>
bool operator!=(const RRArray<T>& a, const RRArray<T>& b)
{
	if(a.mNumElements!=b.mNumElements)
		return true;

	for(int i=0;i<a.mNumElements;i++)
		if(a.mElements[i]!=b.mElements[i])
			return true;
	
	return false;
}

#endif // _RR_ARRAY_H 

//-------------------------------------------------------------------=72
// End of RRArray.h
//-------------------------------------------------------------------=72

