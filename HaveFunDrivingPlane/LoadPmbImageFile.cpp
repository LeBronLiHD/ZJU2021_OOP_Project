#include "pch.h"
#include "LoadPmbImageFile.h"

PmbImage* LoadPmbImageFile::LoadImageFile(char* filename) {
	PmbImage* image = NULL;
	image = (struct ImageFileDate*)malloc(sizeof(struct ImageFileDate));
	image->sizeY = 0;
	image->sizeX = 0;
	image->data = NULL;
	FILE* imageFile = NULL; //�ļ�ָ��
	unsigned long size = 0; //�ļ������ݲ���

	fopen_s(&imageFile, filename, "rb"); //�����Ƶķ�ʽ���ļ�
	//�򵥵Ľ���һ�£�bmp�ļ������ݸ�ʽ��ͷ����54���ֽ��ļ���Ϣ��ʣ�µ����ļ������ݲ��֣�����������Ϣ�͵�ɫ����Ϣ����������Ҫ��ȡ�����ݲ���
	//ͷ����54���ֽ��ļ���Ϣ�������ļ��Ĵ�С�����ͣ����ߵ���Ϣ
	//�ļ��Ĵ�С�����͵���Ϣ�����ǲ���ע����ǰ18���ֽ����ǲ��ܣ���������4���ֽ����ļ��Ŀ�4���ֽ����ļ��ĸ�
	fseek(imageFile, 18, SEEK_SET); //����18���ֽ�

	fread(&(image->sizeX), 4, 1, imageFile); //�ļ��Ŀ�

	fread(&(image->sizeY), 4, 1, imageFile); //�ļ��ĸ�

	fseek(imageFile, 0, SEEK_END); //�����ļ���ͷ

	size = ftell(imageFile) - 54;  //����ļ����ݲ��ֵĳ���

	image->data = (unsigned char*)malloc(size);
	memset(image->data, 0, size);

	fseek(imageFile, 54, SEEK_SET); //�����ļ������ݲ���

	fread(image->data, size, 1, imageFile); //��ȡ�ļ������ݲ���

	fclose(imageFile);

	return image;
}
