// Texture.h: interface for the CTexture class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEXTURE_H__145FB12E_5AFB_4DE2_93C3_7F125D384A93__INCLUDED_)
#define AFX_TEXTURE_H__145FB12E_5AFB_4DE2_93C3_7F125D384A93__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define P_BMP	0
#define P_TGA	1

#define BITMAP_ID 0x4D42


class CTexture  
{
public:
	int GetBPP();
	int GetHeight();
	int GetWidth();
	void Bind();
	CTexture(string szFileName, int file_type);
	virtual ~CTexture();

protected:
	void LoadTGA();
	void LoadBMP();
	void destroy();

	string fileName;
	FILE *file;
	unsigned char *data;
	int width, height, bpp;
	GLuint textureID;
};

#endif // !defined(AFX_TEXTURE_H__145FB12E_5AFB_4DE2_93C3_7F125D384A93__INCLUDED_)
