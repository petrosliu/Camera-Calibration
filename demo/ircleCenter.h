// ircleCenter.h: interface for the CircleCenter class.
// author:mu_yadong
// first written:2004-2-17
// last modified:2004-2-17
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IRCLECENTER_H__3F90BF1B_ED28_4402_9218_40EC017EE4EB__INCLUDED_)
#define AFX_IRCLECENTER_H__3F90BF1B_ED28_4402_9218_40EC017EE4EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CircleCenter  
{
public:
	CircleCenter();
	virtual ~CircleCenter();
	inline COLORREF GetColor() {return m_color;}
	inline CPoint GetPosition() {return m_position;}
	inline bool SetColor(COLORREF color) {m_color = color; return true;}
	inline bool SetPosition(CPoint position) {m_position = position; return true;}
protected:
	COLORREF m_color;
	CPoint m_position;

};

#endif // !defined(AFX_IRCLECENTER_H__3F90BF1B_ED28_4402_9218_40EC017EE4EB__INCLUDED_)
