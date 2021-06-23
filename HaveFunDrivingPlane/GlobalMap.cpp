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
			//��ͼ��ʼ���ڣ�-512��-512������ԭ��,��������Ҫ��0��ʼ����Ҫ��512
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
	glm::vec3  va, vb, vc, vd, ve, vf,		//����o��abc...f�����������va��vb...vf
		norm1, norm2, norm3, norm4, norm5;	//�����ĵ���ص�5�������η������ֱ�Ϊnorm1��6

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
			//����Ե�⣬ÿ�������5����������أ����������5�������εķ�����
			//ֱ�����ò�˺������ɼ���
			norm1 = glm::cross(va, vb);    
			norm2 = glm::cross(vb, vc);
			norm3 = glm::cross(vc, vd);
			norm4 = glm::cross(vd, ve);
			norm5 = glm::cross(ve, vf);
			int X = x + 512, Z = z + 512;
			//��ͼ��ʼ���ڣ�-512��-512������ԭ��,����Ҫ��0��ʼ����Ҫ��512
			//X��Z����СֵΪ1�����Ϊ1023����Ϊ�����Χ��Ӧ�Ķ�����Χ�������������Σ���
			//������û��ר��Ϊ��Ե�������÷���������Դ�����������������û��5����������
			//�����ط�Ҳû��Ϊ��Ե�������÷���������Ե�����Ӧ�ķ�������normal(0,0)û��
			//��ʼ����֪��Ϊɶ����Ҳû����������ϵͳ��Ĭ�ϵĳ�ʼ��
			MapNormal[X][Z].x = -(norm1.x + norm2.x + norm3.x + norm4.x + norm5.x);
			MapNormal[X][Z].y = -(norm1.y + norm2.y + norm3.y + norm4.y + norm5.y);
			MapNormal[X][Z].z = -(norm1.z + norm2.z + norm3.z + norm4.z + norm5.z);

			glm::normalize(MapNormal[X][Z]);
		}
	}
	IsCalculatedNormal = true;
}

int GlobalMap::ValueOfGridEstablishment() {
	int List = glGenLists(1);			//����һ����ʾ�б�Ϊ�����ִ��Ч��
	glNewList(List, GL_COMPILE);		//��ʼװ�룬װ�������ִ��
	for (int i = -512; i < 511; i++)	//1024�ǻҶ�ͼ�Ŀ�
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
