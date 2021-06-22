#include "pch.h"
#include "Model_House.h"
#include "LoadPmbImageFile.h"


CHouse::CHouse()
{
	isreadstl = false;
	isLoadTexture = false;
}


CHouse::~CHouse()
{
}

bool CHouse::Readstl(char *filename)
{
	Head head;
	FILE *fp;
	fopen_s(&fp, filename, "rb");

	if (fp != NULL)
	{
		fread(head.partName, 80, 1, fp);	//��ȡ������  
		fread(&head.faceNum, 4, 1, fp);		//��ȡ������Ƭ����Ŀ  
	}
	faceNum = head.faceNum;
	faces = (Face*)malloc(head.faceNum * sizeof(Face));
	//����������Ƭ����Ŀ������һ��Face���͵�����  

	//ѭ����ȡ������Ƭ����
	for (int i = 0; i < head.faceNum; i++)
	{
		fread(&faces[i].normal, 12, 1, fp);		//��ȡ��������  
		fread(&faces[i].p1, 12, 1, fp);			//��ȡ����1������  
		fread(&faces[i].p2, 12, 1, fp);			//��ȡ����2������  
		fread(&faces[i].p3, 12, 1, fp);			//��ȡ����3������  
		fread(&faces[i].info, 2, 1, fp);		//��ȡ���������ݣ���һ��һ��ûʲô�ã�����ѡ���ȡ��Ϊ���ƶ��ļ�ָ��  

	}
	isreadstl = true;
	if(fp != NULL) fclose(fp);
	return isreadstl;
}

bool CHouse::LoadGLTexture(char *filename)
{
	//AUX_RGBImageRec *pimage;
	//char *bmpname = { "data/Terrain.bmp" };
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

int CHouse::CreatHouse()
{
	if (isLoadTexture == false)
		LoadGLTexture("./Data/House.bmp");
	if (isreadstl == false)
		Readstl("./Data/����.stl");
	int List = glGenLists(1);		//����һ����ʾ�б�Ϊ�����ִ��Ч��
	glNewList(List, GL_COMPILE);	//��ʼװ�룬װ�������ִ��

	glBindTexture(GL_TEXTURE_2D, texture);
	//���б���װ����ʾ�б����ûװ��ȥ����RenderScence��ֻ������glcalllist���ػ�ʱ
	//��û��ִ�д��У���δ�������󶨵Ŀ��ܾ���������������յģ�������
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < faceNum; i++)
	{
		glNormal3f(faces[i].normal.i, faces[i].normal.j, faces[i].normal.k);
		glTexCoord2f(0,0); glVertex3f(faces[i].p1.x, faces[i].p1.y, faces[i].p1.z);
		glTexCoord2f(0,1); glVertex3f(faces[i].p2.x, faces[i].p2.y, faces[i].p2.z);
		glTexCoord2f(0.5,1); glVertex3f(faces[i].p3.x, faces[i].p3.y, faces[i].p3.z);
	}
	glEnd();
	glEndList();
	return List;
}
