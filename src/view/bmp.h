#ifndef _BMP_H_
#define _BMP_H_

//#include <windows.h>
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef unsigned char BYTE;
typedef long LONG;

#pragma pack(1)

class bmpHeader
{
private:
	WORD	bfType;				//文件类型，Windows为BM
	DWORD	bfSize;				//文件大小，以字节为单位
	WORD	bfReserved1;		//保留字，必须为0
	WORD	bfReserved2;		//保留字，必须为0
	DWORD	bfOffBits;			//图像数据起始位置，以字节为单位
public:
	DWORD getOffBits() { return bfOffBits;}
	void setFileSize(DWORD size) { bfSize = size; }
	void setOffBits(DWORD bits) { bfOffBits = bits; }
};

class bmpInfoHeader
{
private:
	DWORD 	biSize;				//本结构字节数
	LONG	biWidth;			//图像宽度，以像素为单位
	LONG	biHeight;			//图像高度，以像素为单位
	WORD	biPlanes;			//平面数，必须为1
	WORD	biBitCounts;		//每个像素需要的位数：1,4,8,16,24
	DWORD	biCompression;		//压缩类型
	DWORD	biSizeImage;		//图像大小，以字节为单位
	LONG	biXPelsPerMeter;	//水平每米像素个数
	LONG	biYPelsPerMeter;	//垂直每米像素个数
	DWORD	biClrUsed;			//颜色表中的颜色数
	DWORD	biClrImportant;		//重要的颜色数
public:
	bool havePalette() { return biBitCounts < 24; }
	int getBits() { return biBitCounts; }
	int getWidth() { return biWidth; }		//返回一行的像素数
	int getHeight() { return biHeight; }
	int getLineBytes(){		//返回一行的字节数
		int bits = biWidth * biBitCounts;
		return bits/8 + (bits%8 != 0);
	}
	int getDataSize() {
		return getLineBytes() * biHeight;
	}
	int getZeros(){
		return getLineBytes()%4? 4 - getLineBytes()%4 : 0;
	}
public:
	void setBits(WORD bits) { biBitCounts = bits; }
	void setImageSize(DWORD size) { biSizeImage = size; }
	void setClrNums(DWORD num) { biClrUsed = num; }
};

class bmpColor
{
private:
	BYTE 	rgbBlue;			//蓝色0-255
	BYTE 	rgbGreen;			//绿色0-255
	BYTE 	rgbRed;				//红色0-255
	BYTE 	rgbReserved;		//保留字，必须为0
public:
	bmpColor() {}
	bmpColor(BYTE r, BYTE g, BYTE b) {
		rgbRed = r;
		rgbGreen = g;
		rgbBlue = b;
	}
	int getR() { return rgbRed; }
	int getB() { return rgbBlue; }
	int getG() { return rgbGreen; }
};

class bmpInfo
{
private:
	bmpInfoHeader	bmiHeader;		//位图信息头
	bmpColor		bmiColors[1];	//颜色表
};

#pragma pack()

typedef unsigned char uchar;
typedef unsigned int uint;

class bmpImage
{
private:
	bmpHeader header;
	bmpInfoHeader infoHeader;
	uchar* RGBdata;				// not BGR in bmp image
public:
	bmpImage(const char* filename);
	~bmpImage();

	// get rgb data array
	uchar* getRGBdata() { return RGBdata; }
	// combine two texture
	void combine(const bmpImage& image);

	// init texture for this image
	void initTexture(uint textureID);
	// static method to init texture
	static void initTexture(uchar* RGBdata, uint textureID, int width, int height);
};

#endif
