// Sprite.h: interface for the CSprite class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPRITE_H__780455D2_30ED_4A90_9F37_B00E133F8899__INCLUDED_)
#define AFX_SPRITE_H__780455D2_30ED_4A90_9F37_B00E133F8899__INCLUDED_

#include "Texture.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSprite  
{
public:
	void End();
	void Begin();
	void Blit(int x, int y);
	CSprite(string filename, int file_type, int dx,int dy);
	virtual ~CSprite();

protected:

	int width, height;
	CTexture *texture;
};

#endif // !defined(AFX_SPRITE_H__780455D2_30ED_4A90_9F37_B00E133F8899__INCLUDED_)
