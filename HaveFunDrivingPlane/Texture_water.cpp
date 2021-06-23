#include "pch.h"
#include "Texture_water.h"
#include "LoadPmbImageFile.h"
#include "Factory.h"

CWater::CWater()
{
	isLoadWater = false;
}

CWater::CWater(const CWater& rhs) {
	this->isLoadWater = rhs.isLoadWater;
	this->textureWater = rhs.textureWater;
}

CWater::~CWater()
{

}

void CWater::swap(CWater& rhs) {
	global_swap(this->isLoadWater, rhs.isLoadWater);
	global_swap(this->textureWater, rhs.textureWater);
}

CWater& CWater::operator=(const CWater& rhs) {
	CWater temp(rhs);
	swap(temp);
	return *this;
}

bool CWater::LoadGLTexture(char *filename)
{

	// AUX_RGBImageRec *pImage;									
	// pImage = loadbmp(filename);	
	ImageFileDate* pImage;
	pImage = NULL;
	pImage = LoadPmbImageFileIn::Instance()->LoadImageFile(filename);

	// Load The Bitmap, Check For Errors, If Bitmap's Not Found Quit
	if (pImage != NULL && pImage->data != NULL)		
	{
		glGenTextures(1, &textureWater);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textureWater);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, pImage->sizeX, pImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pImage->data);//不能用这个贴球
		glDisable(GL_TEXTURE_2D);
	}
	delete pImage;
	isLoadWater = true;
	return isLoadWater;
}

int CWater::CreatWater(int a)
{
	if (!isLoadWater)
		LoadGLTexture("./Data/yellow_water.bmp");
	//int List = glGenLists(1);//创建一个显示列表，为了提高执行效率
	//glNewList(List, GL_COMPILE);//开始装入，装入后不立即执行

	glBindTexture(GL_TEXTURE_2D, textureWater);
	int /*a = 0,*/ step=16;
	for (float i = 0; i < 256; i += step)
	{
		
		glBegin(GL_TRIANGLE_STRIP);
		for (float j = 0; j <= 256; j += step)
		{
			
			float wawe = a*PARAM::PI_PARAM / 4;
			float c = (i + step) / 128, d = j / 128;
			glTexCoord2f((i + step) / 32, j / 32);
			if ((((int)i+step)/step)%2==0)
				glVertex3f(i + step, 0.5*sin(wawe), j);
			else
				glVertex3f(i + step, 0.5*cos(wawe), j);
			glTexCoord2f(i / 32, j / 32);
			if (((int)i / step) % 2 == 0)
				glVertex3f(i, 0.5*sin(wawe), j);
			else
				glVertex3f(i, 0.5*cos(wawe), j);

			a++;
			if (a>8)a = 0;
		}
		glEnd();
	}
	//glEndList();
	//return List;
	return 0;

}