#include "pch.h"
#include "LoadPmbImageFile.h"

PmbImage* LoadPmbImageFile::LoadImageFile(char* filename) {
	PmbImage* image = NULL;
	image = (struct ImageFileDate*)malloc(sizeof(struct ImageFileDate));
	image->sizeY = 0;
	image->sizeX = 0;
	image->data = NULL;
	FILE* imageFile = NULL;		//文件指针
	unsigned long size = 0;		//文件的数据部分

	fopen_s(&imageFile, filename, "rb"); 
	//二进制的方式打开文件
	//bmp文件的数据格式，头部有54个字节文件信息，剩下的是文件的数据部分（包括像素信息和调色板信息），是我们要读取的数据部分
	//头部的54个字节文件信息，包括文件的大小，类型，宽，高等信息
	//前18个字节我们不管，接下来，4个字节是文件的宽，4个字节是文件的高
	if (imageFile == NULL) return NULL;
	
	fseek(imageFile, 18, SEEK_SET);				//跳过18个字节
	if (imageFile == NULL) return NULL;
	fread(&(image->sizeX), 4, 1, imageFile);	//文件的宽
	if (imageFile == NULL) return NULL;
	fread(&(image->sizeY), 4, 1, imageFile);	//文件的高
	if (imageFile == NULL) return NULL;
	fseek(imageFile, 0, SEEK_END);				//跳至文件开头

	size = ftell(imageFile) - 54;				//算出文件数据部分的长度
	if (size <= 0) return NULL;
	image->data = (unsigned char*)malloc(size);
	memset(image->data, 0, size);

	fseek(imageFile, 54, SEEK_SET);				//跳至文件的数据部分

	fread(image->data, size, 1, imageFile);		//读取文件的数据部分

	fclose(imageFile);

	return image;
}
