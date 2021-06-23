
#pragma once
#pragma comment(lib, "glaux.lib")

#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/glaux.h>
#include <gl/glut.h>
#include <cmath>

class Canvas_Sky
{
private:
	bool EarthTextureLoadOrNot;
	bool SunTextureLoadOrNot;
	GLuint NameOfEarthTeture;
	GLuint NameOfSunTexture;

public:
	Canvas_Sky();
	Canvas_Sky(const Canvas_Sky& rhs);
	~Canvas_Sky();
	
	void swap(Canvas_Sky& rhs);	// swap all data between this and rhs
	Canvas_Sky& operator=(const Canvas_Sky& rhs);
	
	virtual bool getEarthTextureLoadStatus();
	virtual bool getSunTextureLoadStatus();
	virtual void setEarthTextureLoadStatus(bool curEarthTextureLoadOrNot);
	virtual void setSuntextureLoadStatus(bool curSunTextureLoadOrNot);
	virtual GLuint getNameEarthTexture();
	virtual GLuint getNameSunTexture();
	virtual void setNameEarthTexture(GLuint curNameEarthTexture);
	virtual void setNameSunTexture(GLuint curNameSunTexture);

	// AUX_RGBImageRec* LoadImageData(char* filePath);
	bool LoadOpenGLTexture(char* filePath);
	int ValueOfSkyEstablishment(float Radius, float Longitude, float Latitude);
	int ValueOfSunEstablishment();
};

