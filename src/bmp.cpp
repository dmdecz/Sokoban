#include "bmp.h"
#include "glut.h"
#include "gl.h"
#include <iostream>
#include <fstream>
using namespace std;

bmpImage::bmpImage(const char* filename)
{
	ifstream in(filename, ios::binary);
	if(!in)
		cout << "fail" << endl;

	in.read((char*)&header, sizeof(header));
	in.read((char*)&infoHeader, sizeof(infoHeader));

	int dataSize = infoHeader.getDataSize();
	cout << filename << endl;
	cout << infoHeader.getWidth() << "x" << infoHeader.getHeight() << endl;

	RGBdata = new uchar[dataSize];
	in.seekg(header.getOffBits());
	in.read((char*)RGBdata, dataSize);
	// swap r and b
	for(int i=0; i<dataSize; i+=3)
	{
		uchar t = RGBdata[i];
		RGBdata[i] = RGBdata[i+2];
		RGBdata[i+2] = t;
	}

	in.close();
}

bmpImage::bmpImage(int width, int height, uchar* data)
	: header(width*height*3 + sizeof(bmpHeader) + sizeof(bmpInfoHeader)),
	  infoHeader(width, height)
{
	RGBdata = new uchar[width * height * 3];
	memcpy(RGBdata, data, width * height * 3);
}

bmpImage::~bmpImage()
{
	delete[] RGBdata;
}

void bmpImage::write_back(const char* filename)
{
	ofstream fout(filename, ios::binary);

	fout.write((char*)& header, sizeof(header));
	fout.write((char*)& infoHeader, sizeof(infoHeader));

	int dataSize = infoHeader.getDataSize();
	cout << filename << endl;
	cout << infoHeader.getWidth() << "x" << infoHeader.getHeight() << endl;
	// swap r and b
	for (int i = 0; i < dataSize; i += 3)
	{
		uchar t = RGBdata[i];
		RGBdata[i] = RGBdata[i + 2];
		RGBdata[i + 2] = t;
	}

	fout.seekp(header.getOffBits());
	fout.write((char*)RGBdata, dataSize);
	// swap r and b
	for (int i = 0; i < dataSize; i += 3)
	{
		uchar t = RGBdata[i];
		RGBdata[i] = RGBdata[i + 2];
		RGBdata[i + 2] = t;
	}
	fout.close();
}

void bmpImage::initTexture(uint textureID)
{
	initTexture(RGBdata, textureID, infoHeader.getWidth(), infoHeader.getHeight());
}

void bmpImage::initTexture(uchar *RGBdata, uint textureID, int width, int height)
{
	glBindTexture(GL_TEXTURE_2D, textureID);

	// 指定当前纹理的放大/缩小过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(
		GL_TEXTURE_2D,
		0,							// mipmap层次(通常为，表示最上层)
		GL_RGB,						// 我们希望该纹理有红、绿、蓝数据
		width,						// 纹理宽带，必须是n，若有边框+2
		height,						// 纹理高度，必须是n，若有边框+2
		0,							// 边框(0=无边框, 1=有边框)
		GL_RGB,						// bitmap数据的格式
		GL_UNSIGNED_BYTE,			// 每个颜色数据的类型
		RGBdata						// bitmap数据指针
	);
}

void bmpImage::combine(const bmpImage &image)
{
	int dataSize = infoHeader.getDataSize();
	for(int i=0; i<dataSize; i++)
	{
		RGBdata[i] = uchar(RGBdata[i] / 2 + image.RGBdata[i] / 2);
	}
}
