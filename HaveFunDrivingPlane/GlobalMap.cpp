#include "pch.h"
#include "GlobalMap.h"
#include "LoadPmbImageFile.h"
#include "Factory.h"

GlobalMap::GlobalMap(){
	IsMapLoadTexture = false;
	RawFileReadOrNot = false;
	IsCalculatedNormal = false;
}

/*
	int StepSizeInAxis;
	bool RawFileReadOrNot;
	BYTE Map[PARAM::MapWidth][PARAM::MapHeight];
	GLfloat xTexture_1, xTexture_2, yTexture_1, ytexture_2;
	glm::vec3 MapNormal[PARAM::MapWidth][PARAM::MapHeight];
	int SizeOfGlobalMap;
	GLuint TextureValue;
	bool IsMapLoadTexture;
	bool IsCalculatedNormal;
*/

GlobalMap::GlobalMap(const GlobalMap& rhs) {
	this->StepSizeInAxis = rhs.StepSizeInAxis;
	this->RawFileReadOrNot = rhs.RawFileReadOrNot;
	//this->Map[0][0] = rhs.Map[0][0];
	for (int i = 0; i < PARAM::MapWidth; i++) {
		for (int j = 0; j < PARAM::MapHeight; j++) {
			this->Map[i][j] = rhs.Map[i][j];
			this->MapNormal[i][j] = rhs.MapNormal[i][j];
		}
	}
	this->xTexture_1 = rhs.xTexture_1;
	this->xTexture_2 = rhs.xTexture_2;
	this->yTexture_1 = rhs.yTexture_1;
	this->ytexture_2 = rhs.ytexture_2;
	this->SizeOfGlobalMap = rhs.SizeOfGlobalMap;
	this->TextureValue = rhs.TextureValue;
	this->IsMapLoadTexture = rhs.IsMapLoadTexture;
	this->IsCalculatedNormal = rhs.IsCalculatedNormal;
}

GlobalMap::~GlobalMap() {

}

void GlobalMap::swap(GlobalMap& rhs) {
	global_swap(this->StepSizeInAxis, rhs.StepSizeInAxis);
	global_swap(this->RawFileReadOrNot, rhs.RawFileReadOrNot);
	for (int i = 0; i < PARAM::MapWidth; i++) {
		for (int j = 0; j < PARAM::MapHeight; j++) {
			global_swap(this->Map[i][j], rhs.Map[i][j]);
			global_swap(this->MapNormal[i][j], rhs.MapNormal[i][j]);
		}
	}
	global_swap(this->xTexture_1, rhs.xTexture_1);
	global_swap(this->xTexture_2, rhs.xTexture_2);
	global_swap(this->yTexture_1, rhs.yTexture_1);
	global_swap(this->ytexture_2, rhs.ytexture_2);
	global_swap(this->SizeOfGlobalMap, rhs.SizeOfGlobalMap);
	global_swap(this->TextureValue, rhs.TextureValue);
	global_swap(this->IsMapLoadTexture, rhs.IsMapLoadTexture);
	global_swap(this->IsCalculatedNormal, rhs.IsCalculatedNormal);
}

GlobalMap& GlobalMap::operator=(const GlobalMap& rhs) {
	GlobalMap temp(rhs);
	swap(temp);
	return *this;
}

bool GlobalMap::ReadRawFile(char* filePath) {
	FILE *filePtr;
	if (fopen_s(&filePtr, filePath, "rb") != 0) {
		MessageBox(NULL, reinterpret_cast<LPCWSTR>("Image(raw) Loading Failed!"), reinterpret_cast<LPCWSTR>("Error"), MB_OK);
		return false;
	}
	fread(Map, 1, PARAM::MapHeight * PARAM::MapWidth, filePtr);
	if (ferror(filePtr)) return false;
	if (filePtr != NULL) fclose(filePtr);
	RawFileReadOrNot = true;
}

BYTE GlobalMap::GetGlobalMapHeight(int PosX, int PosZ) {
	if (!RawFileReadOrNot) {
		ReadRawFile("./Data/Terrain.raw");
	}
	int x = (PosX + 512) / StepSizeInAxis;
	int z = (PosZ + 512) / StepSizeInAxis;
	return Map[x][z];
}

bool GlobalMap::LoadGlobalMapTexture() {
	// AUX_RGBImageRec *ImagePtr;
	char *bmpname = "Data/terrain.bmp";
	// ImagePtr = LoadBmpImage(bmpname);
	ImageFileDate* ImagePtr;
	ImagePtr = NULL;
	ImagePtr = LoadPmbImageFileIn::Instance()->LoadImageFile(bmpname);
	if (ImagePtr->data&&ImagePtr)
	{
		glGenTextures(1, &TextureValue);
		glBindTexture(GL_TEXTURE_2D, TextureValue);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, ImagePtr->sizeX, ImagePtr->sizeY, GL_RGB, GL_UNSIGNED_BYTE, ImagePtr->data);
		IsMapLoadTexture = true;
	}
	delete ImagePtr;
	return IsMapLoadTexture;
}

int GlobalMap::ValueOfGlobalMapEstablishment() {
	if (!IsMapLoadTexture) LoadGlobalMapTexture();
	StepSizeInAxis = 1;
	if (IsCalculatedNormal == false) {
		GetGlobalMapNormalValue();
	}
	int List = glGenLists(1);
	glNewList(List, GL_COMPILE);
	glBindTexture(GL_TEXTURE_2D, TextureValue);

	for (int i = -512; i < 511; i++) {
		glBegin(GL_TRIANGLE_STRIP);
		for (int j = -512; j < 511; j++) {
			float a = 128.0;
			int I = i + 512, J = j + 512;
			//地图起始点在（-512，-512）不是原点,纹理坐标要从0开始所以要加512
			xTexture_1 = I / a;
			xTexture_2 = (I + 1) / a;
			yTexture_1 = J / a;
			ytexture_2 = (J + 1) / a;
			glNormal3f(MapNormal[I + 1][J].x, MapNormal[I + 1][J].y, MapNormal[I + 1][J].z);
			glTexCoord2f(xTexture_2, ytexture_2);   
			glVertex3f(i + 1, GetGlobalMapHeight(i + 1, j), j);

			glNormal3f(MapNormal[I][J].x, MapNormal[I][J].y, MapNormal[I][J].z);
			glTexCoord2f(xTexture_1, yTexture_1);   glVertex3f(i, GetGlobalMapHeight(i, j), j);
		}
		glEnd();
	}
	glEndList();
	return List;
}

void GlobalMap::GetGlobalMapNormalValue() {
	glm::vec3  va, vb, vc, vd, ve, vf,		//中心o到abc...f六个点的向量va，vb...vf
		norm1, norm2, norm3, norm4, norm5;	//与中心点相关的5个三角形发向量分别为norm1到6

	int x, z;
	for (x = -511; x < 511; x += StepSizeInAxis)
	{
		for (z = -511; z < 511; z += StepSizeInAxis)
		{
			va.x = 0; 
			va.y = GetGlobalMapHeight(x, z + StepSizeInAxis) - GetGlobalMapHeight(x, z); 
			va.z = StepSizeInAxis;
			vb.x = -StepSizeInAxis; vb.y = GetGlobalMapHeight(x - StepSizeInAxis, z) - GetGlobalMapHeight(x, z);
			vb.z = 0;
			vc.x = -StepSizeInAxis; vc.y = GetGlobalMapHeight(x - StepSizeInAxis, z - StepSizeInAxis) - GetGlobalMapHeight(x, z);
			vc.z = -StepSizeInAxis;
			vd.x = 0; 
			vd.y = GetGlobalMapHeight(x, z - StepSizeInAxis) - GetGlobalMapHeight(x, z); 
			vd.z = -StepSizeInAxis;
			ve.x = StepSizeInAxis; ve.y = GetGlobalMapHeight(x + StepSizeInAxis, z) - GetGlobalMapHeight(x, z); 
			ve.z = 0;
			vf.x = StepSizeInAxis; vf.y = GetGlobalMapHeight(x + StepSizeInAxis, z + StepSizeInAxis) - GetGlobalMapHeight(x, z); 
			vf.z = StepSizeInAxis;
			//除边缘外，每个顶点和5个三角形相关，下面计算这5个三角形的法向量
			//直接利用叉乘函数即可计算
			norm1 = glm::cross(va, vb);    
			norm2 = glm::cross(vb, vc);
			norm3 = glm::cross(vc, vd);
			norm4 = glm::cross(vd, ve);
			norm5 = glm::cross(ve, vf);
			int X = x + 512, Z = z + 512;
			//地图起始点在（-512，-512）不是原点,数组要从0开始所以要加512
			//X，Z的最小值为1，最大为1023，因为这个范围对应的顶点周围才有六个三角形，即
			//在这里没有专门为边缘顶点设置法向量，边源顶点相关联的三角形没有5个，本程序
			//其他地方也没有为边缘顶点设置法向量，边缘顶点对应的发向量如normal(0,0)没有
			//初始化不知道为啥运行也没出错，可能是系统有默认的初始化
			MapNormal[X][Z].x = -(norm1.x + norm2.x + norm3.x + norm4.x + norm5.x);
			MapNormal[X][Z].y = -(norm1.y + norm2.y + norm3.y + norm4.y + norm5.y);
			MapNormal[X][Z].z = -(norm1.z + norm2.z + norm3.z + norm4.z + norm5.z);

			glm::normalize(MapNormal[X][Z]);
		}
	}
	IsCalculatedNormal = true;
}

int GlobalMap::ValueOfGridEstablishment() {
	int List = glGenLists(1);			//创建一个显示列表，为了提高执行效率
	glNewList(List, GL_COMPILE);		//开始装入，装入后不立即执行
	for (int i = -512; i < 511; i++)	//1024是灰度图的宽
	{

		for (int j = -512; j < 511; j++)
		{
			glBegin(GL_LINE_LOOP);
			glVertex3f(i, GetGlobalMapHeight(i, j), j);
			glVertex3f(i + 1, GetGlobalMapHeight(i + 1, j), j);
			glVertex3f(i + 1, GetGlobalMapHeight(i + 1, j + 1), j + 1);
			glVertex3f(i, GetGlobalMapHeight(i, j + 1), j + 1);
			glVertex3f(i + 1, GetGlobalMapHeight(i + 1, j), j);
			glEnd();
		}

	}
	glEndList();
	return List;
}
