#pragma comment(lib, "glaux.lib")
#include <GL\glaux.h>
#include <GL\glut.h>

#pragma once
class CHouse
{
	struct Head
	{
		char partName[80];//�ļ�����  
		int  faceNum;//�����Ŀ  
	};
	//����  
	struct Normal
	{
		float i;
		float j;
		float k;
	};
	//�㣬����float���͵ģ���СΪ12�ֽ�  
	struct Point
	{
		float x;
		float y;
		float z;
	};
	//�����棬��һ�����ߣ������㣬��һ�����ֽڵı����һ��50�ֽ�  
	struct Face
	{
		Normal normal;
		Point  p1;
		Point  p2;
		Point  p3;
		char  info[2];//�������ݣ����Բ��ù�  
	};
	Face *faces;
	//���ȡstlģ����ر���
	bool isreadstl;
	int faceNum;
	bool isLoadTexture;
	GLuint texture;//��������
public:
	CHouse();
	~CHouse();

	bool Readstl(char *filename);
	// AUX_RGBImageRec* LoadBmp(char *filename);
	bool LoadGLTexture(char *filename);
	int CreatHouse();
};

