#include "pch.h"
#include "Model_Plane.h"
#include "LoadPmbImageFile.h"

CPlane::CPlane()
{
	isreadstl = false;
	isLoadTexture = false;
}


CPlane::~CPlane()
{
}

bool CPlane::Readstl(char *filename)
{
	Head head;
	FILE *fp;
	fopen_s(&fp,filename, "rb");

	if (fp != NULL)
	{
		fread(head.partName, 80, 1, fp);//��ȡ������  
		fread(&head.faceNum, 4, 1, fp);//��ȡ������Ƭ����Ŀ  
	}
	faceNum = head.faceNum;
	faces = (Face*)malloc(head.faceNum * sizeof(Face));//����������Ƭ����Ŀ������һ��Face���͵�����  

	//ѭ����ȡ������Ƭ����
	for (int i = 0; i < head.faceNum; i++)
	{
		fread(&faces[i].normal, 12, 1, fp);//��ȡ��������  
		fread(&faces[i].p1, 12, 1, fp);//��ȡ����1������  
		fread(&faces[i].p2, 12, 1, fp);//��ȡ����2������  
		fread(&faces[i].p3, 12, 1, fp);//��ȡ����3������  
		fread(&faces[i].info, 2, 1, fp);//��ȡ���������ݣ���һ��һ��ûʲô�ã�����ѡ���ȡ��Ϊ���ƶ��ļ�ָ��  

	}
	isreadstl = true;
	fclose(fp);
	return isreadstl;
}

// AUX_RGBImageRec* CPlane::LoadBmp(char *filename)
// {
// 	FILE *fp;
// 	if (!filename)//δ�����ļ���
// 		return 0;
// 	fopen_s(&fp, filename, "r");
// 	if (fp)//�ļ��򿪳ɹ�
// 	{
// 		fclose(fp);
// 		WCHAR wfilename[256];
// 		memset(wfilename, 0, sizeof(wfilename));
// 		MultiByteToWideChar(CP_ACP, 0, filename, strlen(filename) + 1, wfilename, sizeof(wfilename) / sizeof(wfilename[0]));
// 		return auxDIBImageLoad(wfilename);
// 		free(fp);
// 	}
// 	return 0;
// }

bool CPlane::LoadGLTexture(char *filename)
{
	// AUX_RGBImageRec *pimage;
	//char *bmpname = { "data/Terrain.bmp" };
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
		//glTexImage2D(GL_TEXTURE_2D, 0, 3, pimage->sizeX, pimage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pimage->data);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pimage->sizeX, pimage->sizeY, GL_RGB, GL_UNSIGNED_BYTE, pimage->data);
		isLoadTexture = true;
		free(pimage->data);											// Free The Texture Image Memory
		free(pimage);
	}
	return isLoadTexture;
}

int CPlane::CreatPlane()
{
	if (isLoadTexture == false)
		LoadGLTexture("Data/Plane.bmp");
	if (isreadstl == false)
		Readstl("Data/plane.stl");

	int List = glGenLists(1);//����һ����ʾ�б�Ϊ�����ִ��Ч��
	glNewList(List, GL_COMPILE);//��ʼװ�룬װ�������ִ��
	////��������������
	//glBegin(GL_LINES);
	//glColor3f(1, 0, 0);
	//glVertex3f(-30, 0, 0);
	//glVertex3f(30, 0, 0);
	//glColor3f(0, 1, 0);
	//glVertex3f(0, -30, 0);
	//glVertex3f(0, 30, 0);
	//glColor3f(0, 0, 1);
	//glVertex3f(0, 0, -30);
	//glVertex3f(0, 0, 30);
	//glEnd();

	glBindTexture(GL_TEXTURE_2D, texture);//���б���װ����ʾ�б����ûװ��ȥ����RenderScence��ֻ������glcalllist���ػ�ʱ
	//��û��ִ�д��У���δ�������󶨵Ŀ��ܾ���������������յģ�������
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < faceNum; i++)
	{
		float a = 2500;//�������a��Ϊ����С�ɻ�
		glNormal3f(faces[i].normal.i, faces[i].normal.j, faces[i].normal.k);
		glTexCoord2f(0,0); glVertex3f(faces[i].p1.x / a, faces[i].p1.y / a, faces[i].p1.z / a);
		glTexCoord2f(0,1); glVertex3f(faces[i].p2.x / a, faces[i].p2.y / a, faces[i].p2.z / a);
		glTexCoord2f(0.5,1); glVertex3f(faces[i].p3.x / a, faces[i].p3.y / a, faces[i].p3.z / a);
	}
	glEnd();
	glEndList();
	return List;
}
