#pragma comment(lib, "glaux.lib")
#include <GL\glaux.h>
#include <GL\glut.h>

#pragma once
class CPlane
{
	struct Head
	{
		char partName[80];	//�ļ�����  
		int  faceNum;		//�����Ŀ  
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
	bool isreadstl;		//���ȡstlģ����ر���
	int faceNum;
	bool isLoadTexture;
	GLuint texture;		//��������
public:
	CPlane();
	~CPlane();

	virtual void set_texture(GLuint cur_texture) { this->texture = cur_texture; }
	virtual GLuint get_texture() { return this->texture; }
	virtual void set_isLoadTexture(bool cur_isLoadTexture) { this->isLoadTexture = cur_isLoadTexture; }
	virtual bool get_isLoadTexture() { return this->isLoadTexture; }
	virtual void set_faceNum(int cur_faceNum) { this->faceNum = cur_faceNum; }
	virtual int get_faceNum() { return this->faceNum; }
	virtual void set_isreadstl(bool cur_isreadstl) { this->isreadstl = cur_isreadstl; }
	virtual bool get_isreadstl() { return this->isreadstl; }

	bool Readstl(char *filename);
	// AUX_RGBImageRec* LoadBmp(char *filename);
	bool LoadGLTexture(char *filename);
	int CreatPlane();
};


