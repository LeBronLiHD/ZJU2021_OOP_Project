#pragma comment(lib, "glaux.lib")
#include <GL\glaux.h>
#include <GL\glut.h>

#pragma once
class CPlane
{
	struct Head
	{
		char partName[80];	//文件名称  
		int  faceNum;		//面的数目  
	};
	//法线  
	struct Normal
	{
		float i;
		float j;
		float k;
	};
	//点，三个float类型的，大小为12字节  
	struct Point
	{
		float x;
		float y;
		float z;
	};
	//三角面，由一个法线，三个点，和一个两字节的保留项，一共50字节  
	struct Face
	{
		Normal normal;
		Point  p1;
		Point  p2;
		Point  p3;
		char  info[2];//保留数据，可以不用管  
	};
	Face *faces;
	bool isreadstl;		//与读取stl模型相关变量
	int faceNum;
	bool isLoadTexture;
	GLuint texture;		//绑定纹理用
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


