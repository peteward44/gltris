// Texture.cpp: implementation of the CTexture class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Exception.h"
#include "Texture.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTexture::CTexture(string szFileName, int file_type)
{
	textureID = 0; file = NULL; data = NULL;
	fileName = szFileName;

	try {
		// what file are we loading?
		switch (file_type) {
		case P_BMP:
			LoadBMP();
			break;
		case P_TGA:
			LoadTGA();
			break;
		default:
			throw(CTextureException("Invalid file type specified"));
			break;
		}
	}
	catch (CTextureException &e) {
		destroy();
		throw(e);
	}
}

CTexture::~CTexture()
{
	destroy();
}

void CTexture::destroy()
{
	if (file) {
		fclose(file);
		file = NULL;
	}

	if (textureID) {
		glDeleteTextures(1, &textureID);
		textureID = 0;
	}

	if (data) {
		delete data;
		data = NULL;
	}
}

void CTexture::LoadBMP()
{
	BITMAPFILEHEADER	file_header;
	BITMAPINFOHEADER	info_header;
	int					imageIdx = 0;
	unsigned char		tempRGB;

	file = fopen(fileName.c_str(), "rb");
	if(file==NULL)
		throw(CTextureException("Could not load "+fileName));

	fread(&file_header, sizeof(BITMAPFILEHEADER), 1, file);

	if(file_header.bfType != BITMAP_ID)
		throw(CTextureException(fileName+" is not a BMP"));

	// Read the bitmap information header in
	fread(&info_header, sizeof(BITMAPINFOHEADER), 1, file);

	// Advance the file pointer to the beginning of the bitmap data
	fseek(file, file_header.bfOffBits, SEEK_SET);

	data = new unsigned char [info_header.biSizeImage];
	if(!data)
		throw(CTextureException("Could not allocate enough memory for "+fileName));

	// Read in the bitmap image data
	fread(data, 1, info_header.biSizeImage, file);

	//Make sure bitmap image data was read
	if(data==NULL)
		throw(CTextureException("Bitmap information could not be read for "+fileName));

	width = info_header.biWidth;
	height= info_header.biWidth;
	bpp	  = info_header.biBitCount;

	//Swap the R and B values to get RGB since the bitmap color format is in BGR
	for(imageIdx = 0; imageIdx<(int)info_header.biSizeImage; imageIdx+=3)
	{
		tempRGB			= data[imageIdx];
		data[imageIdx]  = data[imageIdx + 2];
		data[imageIdx+2]= tempRGB;
	}

	//Close the file
	fclose(file); file = NULL;

	//Build A Texture From The Data
	glGenTextures(1, &textureID);

	glBindTexture(GL_TEXTURE_2D, textureID);			//Bind the texture to a texture object 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//Filtering for if texture is bigger than should be
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	//Filtering for if texture is smaller than it should be

	int type;
	type = ((bpp==32) ? GL_RGBA : GL_RGB);

	glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, data);
}

void CTexture::LoadTGA()
{
	GLubyte		TGAheader[12]={0,0,2,0,0,0,0,0,0,0,0,0};	// Uncompressed TGA Header
	GLubyte		TGAcompare[12];								// Used To Compare TGA Header
	GLubyte		header[6];									// First 6 Useful Bytes From The Header
	GLuint		bytesPerPixel;								// Holds Number Of Bytes Per Pixel Used In The TGA File
	GLuint		imageSize;									// Used To Store The Image Size When Setting Aside Ram
	GLuint		temp;										// Temporary Variable
	GLuint		type=GL_RGBA;								// Set The Default GL Mode To RBGA (32 BPP)

	file = fopen(fileName.c_str(), "rb");						// Open The TGA File

	if(	file==NULL )
		throw(CTextureException("Could not load "+fileName));

	fread(TGAcompare,1,sizeof(TGAcompare),file);

	if (memcmp(TGAheader, TGAcompare, sizeof(TGAheader)) != 0)
		throw(CTextureException(fileName+" is not a valid uncompressed TGA file"));

	fread(header,1,sizeof(header),file);				// If So Read Next 6 Header Bytes


	width  = header[1] * 256 + header[0];			// Determine The TGA Width	(highbyte*256+lowbyte)
	height = header[3] * 256 + header[2];			// Determine The TGA Height	(highbyte*256+lowbyte)
    
 	if(	width	<=0	||								// Is The Width Less Than Or Equal To Zero
		height	<=0	||								// Is The Height Less Than Or Equal To Zero
		(header[4]!=24 && header[4]!=32))					// Is The TGA 24 or 32 Bit?
	{
		throw(CTextureException("Width and height of TGA less than zero for "+fileName));
	}

	bpp	= header[4];							// Grab The TGA's Bits Per Pixel (24 or 32)
	bytesPerPixel	= bpp/8;						// Divide By 8 To Get The Bytes Per Pixel
	imageSize		= width*height*bytesPerPixel;	// Calculate The Memory Required For The TGA Data

	data = new unsigned char[imageSize];

	if(	data==NULL )
		throw(CTextureException("Error allocating memory for "+fileName));

	fread(data, 1, imageSize, file);

	for(GLuint i=0; i<int(imageSize); i+=bytesPerPixel)		// Loop Through The Image Data
	{														// Swaps The 1st And 3rd Bytes ('R'ed and 'B'lue)
		temp=data[i];							// Temporarily Store The Value At Image Data 'i'
		data[i] = data[i + 2];	// Set The 1st Byte To The Value Of The 3rd Byte
		data[i + 2] = temp;					// Set The 3rd Byte To The Value In 'temp' (1st Byte Value)
	}

	fclose (file);											// Close The File
	file = NULL;

	// Build A Texture From The Data
	glGenTextures(1, &textureID);					// Generate OpenGL texture IDs

	glBindTexture(GL_TEXTURE_2D, textureID);			// Bind Our Texture
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// Linear Filtered
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// Linear Filtered
	
	if (bpp==24)									// Was The TGA 24 Bits
	{
		type=GL_RGB;										// If So Set The 'type' To GL_RGB
	}

	glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, data);

}

void CTexture::Bind()
{
	glBindTexture(GL_TEXTURE_2D, textureID);
}

int CTexture::GetWidth()
{
	return width;
}

int CTexture::GetHeight()
{
	return height;
}

int CTexture::GetBPP()
{
	return bpp;
}
