#include "pch.h"
#include "Model_Plane.h"
#include "LoadPmbImageFile.h"
#include "Factory.h"


CPlane::CPlane()
{
	isreadstl = false;
	isLoadTexture = false;
}

CPlane::CPlane(const CPlane& rhs) {
	this->faces = rhs.faces;
	this->isreadstl = rhs.isreadstl;
	this->faceNum = rhs.faceNum;
	this->isLoadTexture = rhs.isLoadTexture;
	this->texture = rhs.texture;
}

CPlane::~CPlane()
{
}

void CPlane::swap(CPlane& rhs) {
	global_swap(this->faces, rhs.faces);
	global_swap(this->isreadstl, rhs.isreadstl);
	global_swap(this->faceNum, rhs.faceNum);
	global_swap(this->isLoadTexture, rhs.isLoadTexture);
	global_swap(this->texture, rhs.texture);
}

CPlane& CPlane::operator=(const CPlane& rhs) {
	CPlane temp(rhs);
	swap(temp);
	return *this;
}

bool CPlane::Readstl(char *filename)
{
	Head head;
	FILE *fp;
	fopen_s(&fp,filename, "rb");

	if (fp != NULL)
	{
		fread(head.partName, 80, 1, fp);	//获取部件名  
		fread(&head.faceNum, 4, 1, fp);		//获取三角面片的数目  
	}
	faceNum = head.faceNum;
	faces = (Face*)malloc(head.faceNum * sizeof(Face));
	//根据三角面片的数目，创建一个Face类型的数组  

	//循环读取三角面片数据
	for (int i = 0; i < head.faceNum; i++)
	{
		fread(&faces[i].normal, 12, 1, fp);		//读取法线数据  
		fread(&faces[i].p1, 12, 1, fp);			//读取顶点1的数据  
		fread(&faces[i].p2, 12, 1, fp);			//读取顶点2的数据  
		fread(&faces[i].p3, 12, 1, fp);			//读取顶点3的数据  
		fread(&faces[i].info, 2, 1, fp);		//读取保留项数据，这一项一般没什么用，这里选择读取是为了移动文件指针  

	}
	isreadstl = true;
	if(fp != NULL) fclose(fp);
	return isreadstl;
}

bool CPlane::LoadGLTexture(char *filename)
{
	// AUX_RGBImageRec *pimage;
	// pimage = LoadBmp(filename);
	ImageFileDate* pimage;
	pimage = NULL;
	pimage = LoadPmbImageFileIn::Instance()->LoadImageFile(filename);
	if (pimage->data&&pimage)
	{
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pimage->sizeX, pimage->sizeY, GL_RGB, GL_UNSIGNED_BYTE, pimage->data);
		isLoadTexture = true;
	}
	delete pimage;
	return isLoadTexture;
}

int CPlane::CreatPlane()
{
	if (isLoadTexture == false)
		LoadGLTexture("./Data/plane.bmp");
	if (isreadstl == false)
		Readstl("./Data/plane.stl");

	int List = glGenLists(1);		//创建一个显示列表，为了提高执行效率
	glNewList(List, GL_COMPILE);	//开始装入，装入后不立即执行

	glBindTexture(GL_TEXTURE_2D, texture);
	//此行必须装进显示列表，如果没装进去，在RenderScence中只调用了glcalllist，重绘时
	//就没有执行此行，即未绑定纹理，绑定的可能就是其他（比如天空的）纹理了
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < faceNum; i++)
	{
		float a = 2500;		//下面除以a是为了缩小飞机
		glNormal3f(faces[i].normal.i, faces[i].normal.j, faces[i].normal.k);
		glTexCoord2f(0,0); glVertex3f(faces[i].p1.x / a, faces[i].p1.y / a, faces[i].p1.z / a);
		glTexCoord2f(0,1); glVertex3f(faces[i].p2.x / a, faces[i].p2.y / a, faces[i].p2.z / a);
		glTexCoord2f(0.5,1); glVertex3f(faces[i].p3.x / a, faces[i].p3.y / a, faces[i].p3.z / a);
	}
	glEnd();
	glEndList();
	return List;
}
