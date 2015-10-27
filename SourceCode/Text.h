// Text.h: interface for the CText class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEXT_H__41EC22A6_2257_4DE3_9F37_143B4E92F779__INCLUDED_)
#define AFX_TEXT_H__41EC22A6_2257_4DE3_9F37_143B4E92F779__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CText  
{
public:
	void Print(int x, int y, const char *fmt, ...);
	CText(string FontName);
	virtual ~CText();

protected:
	HFONT font;
	GLuint base_display_list;
	CTexture *text;
};

#endif // !defined(AFX_TEXT_H__41EC22A6_2257_4DE3_9F37_143B4E92F779__INCLUDED_)
