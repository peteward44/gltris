// Text.cpp: implementation of the CText class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Text.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CText::CText(string FontName)
{
	text = NULL;
	text = new CTexture("images\\font.bmp", P_BMP);


	base_display_list=glGenLists(256);						// Creating 256 Display Lists
	for (int loop1=0; loop1<256; loop1++)					// Loop Through All 256 Lists
	{
		float cx=float(loop1%16)/16.0f;						// X Position Of Current Character
		float cy=float(loop1/16)/16.0f;						// Y Position Of Current Character

		glNewList(base_display_list+loop1,GL_COMPILE);					// Start Building A List
		glEnable(GL_TEXTURE_2D);
		text->Bind();
			glBegin(GL_QUADS);								// Use A Quad For Each Character

				glTexCoord2f(cx,1.0f-cy-0.001f);			// Texture Coord (Top Left)
				glVertex2i(0,16);							// Vertex Coord (Top Left)
				glTexCoord2f(cx+0.0625f,1.0f-cy-0.001f);	// Texture Coord (Top Right)
				glVertex2i(16,16);							// Vertex Coord (Top Right)
				glTexCoord2f(cx+0.0625f,1.0f-cy-0.0625f);	// Texture Coord (Bottom Right)
				glVertex2i(16,0);							// Vertex Coord (Bottom Right)
				glTexCoord2f(cx,1.0f-cy-0.0625f);			// Texture Coord (Bottom Left)
				glVertex2d(0,0);							// Vertex Coord (Bottom Left)

			glEnd();										// Done Building Our Quad (Character)
			glTranslated(14,0,0);							// Move To The Right Of The Character
		glDisable(GL_TEXTURE_2D);
		glEndList();										// Done Building The Display List
	}														// Loop Until All 256 Are Built
}

CText::~CText()
{
	glDeleteLists(base_display_list,256);
	if (text)
		delete text;
}

void CText::Print(int x, int y, const char *fmt, ...)
{
	int set = 1;
	char		text1[1024];									// Holds Our String
	va_list		ap;											// Pointer To List Of Arguments

	if (fmt == NULL)										// If There's No Text
		return;												// Do Nothing

	va_start(ap, fmt);										// Parses The String For Variables
	    vsprintf(text1, fmt, ap);							// And Converts Symbols To Actual Numbers
	va_end(ap);												// Results Are Stored In Text

	if (set>1)												// Did User Choose An Invalid Character Set?
	{
		set=1;												// If So, Select Set 1 (Italic)
	}

	glLoadIdentity();										// Reset The Modelview Matrix
	glTranslated(x,y,0);									// Position The Text (0,0 - Top Left)
	glListBase(base_display_list-32+(128*set));							// Choose The Font Set (0 or 1)

	glCallLists(strlen(text1),GL_UNSIGNED_BYTE, text1);		// Write The Text To The Screen								// Disable Texture Mapping
}
