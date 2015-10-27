// Sprite.cpp: implementation of the CSprite class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Sprite.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CSprite::CSprite(string filename, int file_type, int dx, int dy)
{
	width = dx; height = dy;

	texture = new CTexture(filename, file_type);
}

CSprite::~CSprite()
{
	delete texture;
}

void CSprite::Blit(int x, int y)
{
	glTexCoord2f(0, 0);
	glVertex2i(x, y);
	glTexCoord2f(1.0f, 0);
	glVertex2i(x+width, y);
	glTexCoord2f(1.0f, 1.0f);
	glVertex2i(x+width, y+height);
	glTexCoord2f(0, 1.0f);
	glVertex2i(x, y+height);
}

void CSprite::Begin()
{
	glEnable(GL_TEXTURE_2D);
	texture->Bind();
	glBegin(GL_QUADS);
}

void CSprite::End()
{
	glEnd();
	glDisable(GL_TEXTURE_2D);
}
