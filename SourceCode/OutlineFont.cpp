// OutlineFont.cpp: implementation of the COutlineFont class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OutlineFont.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COutlineFont::COutlineFont(string FontName)
{
	base = glGenLists(256);								// Storage For 256 Characters

	font = CreateFont(	-20,							// Height Of Font
						0,								// Width Of Font
						0,								// Angle Of Escapement
						0,								// Orientation Angle
						FW_BOLD,						// Font Weight
						FALSE,							// Italic
						FALSE,							// Underline
						FALSE,							// Strikeout
						ANSI_CHARSET,					// Character Set Identifier
						OUT_TT_PRECIS,					// Output Precision
						CLIP_DEFAULT_PRECIS,			// Clipping Precision
						ANTIALIASED_QUALITY,			// Output Quality
						FF_DONTCARE|DEFAULT_PITCH,		// Family And Pitch
//						FontName.c_str());				// Font Name
						"Comic Sans MS");

	SelectObject(CApp::GetInstance()->GetHDC(), font);							// Selects The Font We Created

	wglUseFontOutlines(	CApp::GetInstance()->GetHDC(),							// Select The Current DC
						0,								// Starting Character
						255,							// Number Of Display Lists To Build
						base,							// Starting Display Lists
						0.0f,							// Deviation From The True Outlines
						0.2f,							// Font Thickness In The Z Direction
						WGL_FONT_POLYGONS,				// Use Polygons, Not Lines
						gmf);
}

COutlineFont::~COutlineFont()
{
	glDeleteLists(base, 256);
}

void COutlineFont::Print(int x, int y, const char *fmt, ...)
{
//	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();

	glRasterPos2d(x,y);

	float		length=0;								// Used To Find The Length Of The Text
	char		text[256];								// Holds Our String
	va_list		ap;										// Pointer To List Of Arguments

	if (fmt == NULL)									// If There's No Text
		return;											// Do Nothing

	va_start(ap, fmt);									// Parses The String For Variables
	    vsprintf(text, fmt, ap);						// And Converts Symbols To Actual Numbers
	va_end(ap);											// Results Are Stored In Text

//	for (unsigned int loop=0;loop<(strlen(text));loop++)	// Loop To Find Text Length
//	{
//		length+=gmf[text[loop]].gmfCellIncX;			// Increase Length By Each Characters Width
//	}

	glColor3f(1.0f,1.0f,1.0f);

	glPushAttrib(GL_LIST_BIT);							// Pushes The Display List Bits
	glListBase(base);									// Sets The Base Character to 0
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);	// Draws The Display List Text
	glPopAttrib();		 								// Pops The Display List Bits

//	glMatrixMode(GL_PROJECTION);							// Select The Modelview Matrix
	glLoadIdentity();
}