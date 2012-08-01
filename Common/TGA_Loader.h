#pragma once

#include <iostream>
#include <fstream>
#include <memory.h> 

#define IMG_OK 0x1
#define IMG_ERR_NO_FILE 0x2
#define IMG_ERR_MEM_FAIL 0x4
#define IMG_ERR_BAD_FORMAT 0x8
#define IMG_ERR_UNSUPPORTED 0x40 

class TGA_Loader
{
public:
	TGA_Loader();
	~TGA_Loader();
	int		Load( const char* filename );
	int		GetBPP();
	int		GetWidth();
	int		GetHeight();
	unsigned char* GetImg();       // Return a pointer to image data
	unsigned char* GetPalette();   // Return a pointer to VGA palette

private:
	short int		iWidth,	iHeight,	iBPP;
	unsigned long	lImageSize;
	char			bEnc;
	unsigned char *pImage, *pPalette, *pData;

	// Internal workers
	int		ReadHeader();
	int		LoadRawData();
	int		LoadTgaRLEData();
	int		LoadTgaPalette();
	void	BGRtoRGB();
	void	FlipImg();
};


    




