// OutlineFont.h: interface for the COutlineFont class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OUTLINEFONT_H__4A9C11B7_43BB_4957_A7EF_5A2B82A935EA__INCLUDED_)
#define AFX_OUTLINEFONT_H__4A9C11B7_43BB_4957_A7EF_5A2B82A935EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class COutlineFont  
{
public:
	COutlineFont(string FontName);
	virtual ~COutlineFont();
	void Print(int x, int y, const char *fmt, ...);

protected:
	HFONT	font;
	GLuint	base;
	GLYPHMETRICSFLOAT gmf[256];
};

#endif // !defined(AFX_OUTLINEFONT_H__4A9C11B7_43BB_4957_A7EF_5A2B82A935EA__INCLUDED_)
