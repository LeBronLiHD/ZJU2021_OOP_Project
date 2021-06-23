#include "pch.h"
#include "Canvas_Sky.h"
#include "LoadPmbImageFile.h"

Canvas_Sky::Canvas_Sky() {
	this->EarthTextureLoadOrNot = false;
	this->SunTextureLoadOrNot = false;
}

/*
	bool EarthTextureLoadOrNot;
	bool SunTextureLoadOrNot;
	GLuint NameOfEarthTeture;
	GLuint NameOfSunTexture;
*/

Canvas_Sky::Canvas_Sky(const Canvas_Sky& rhs) {
	this->EarthTextureLoadOrNot = rhs.EarthTextureLoadOrNot;
	this->SunTextureLoadOrNot = rhs.SunTextureLoadOrNot;
	this->NameOfEarthTeture = rhs.NameOfEarthTeture;
	this->NameOfSunTexture = rhs.NameOfSunTexture;
}

Canvas_Sky::~Canvas_Sky() {

}

void Canvas_Sky::swap(Canvas_Sky& rhs) {
	bool rhsEarthTextureLoadOrNot = rhs.EarthTextureLoadOrNot;
	rhs.EarthTextureLoadOrNot = getEarthTextureLoadStatus();
	setEarthTextureLoadStatus(rhsEarthTextureLoadOrNot);
	bool rhsSunTextureLoadOrNot = rhs.SunTextureLoadOrNot;
	rhs.SunTextureLoadOrNot = getSunTextureLoadStatus();
	setSuntextureLoadStatus(rhsSunTextureLoadOrNot);
	GLuint rhsNameOfEarthTeture = rhs.NameOfEarthTeture;
	rhs.NameOfEarthTeture = getNameEarthTexture();
	setNameEarthTexture(rhsNameOfEarthTeture);
	GLuint rhsNameOfSunTexture = rhs.NameOfSunTexture;
	rhs.NameOfSunTexture = getNameSunTexture();
	setNameSunTexture(rhsNameOfSunTexture);
}

Canvas_Sky& Canvas_Sky::operator=(const Canvas_Sky& rhs) {
	Canvas_Sky temp(rhs);
	swap(temp);
	return *this;
}

bool Canvas_Sky::getEarthTextureLoadStatus() {
	return this->EarthTextureLoadOrNot;
}

bool Canvas_Sky::getSunTextureLoadStatus() {
	return this->SunTextureLoadOrNot;
}

void Canvas_Sky::setEarthTextureLoadStatus(bool curEarthTextureLoadStatus) {
	this->EarthTextureLoadOrNot = curEarthTextureLoadStatus;
}

void Canvas_Sky::setSuntextureLoadStatus(bool curSunTextureLoadStatus) {
	this->SunTextureLoadOrNot = curSunTextureLoadStatus;
}

GLuint Canvas_Sky::getNameEarthTexture() {
	return this->NameOfEarthTeture;
}

GLuint Canvas_Sky::getNameSunTexture() {
	return this->NameOfSunTexture;
}

void Canvas_Sky::setNameEarthTexture(GLuint curNameEarthTexture) {
	this->NameOfEarthTeture = curNameEarthTexture;
}

void Canvas_Sky::setNameSunTexture(GLuint curNameSunTexture) {
	this->NameOfSunTexture = curNameSunTexture;
}

bool Canvas_Sky::LoadOpenGLTexture(char* filePath) {
	// AUX_RGBImageRec *ImagePtr;
	// ImagePtr = LoadImageData(filePath);
	ImageFileDate* ImagePtr;
	ImagePtr = NULL;
	ImagePtr = LoadPmbImageFileIn::Instance()->LoadImageFile(filePath);

	// load the bit map and check for errors
	if (ImagePtr != NULL && ImagePtr->data != NULL) {
		glGenTextures(1, &NameOfEarthTeture);
		glBindTexture(GL_TEXTURE_2D, NameOfEarthTeture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, ImagePtr->sizeX, ImagePtr->sizeY, GL_RGB, GL_UNSIGNED_BYTE, ImagePtr->data);
		//free(ImagePtr->data);
		//free(ImagePtr);
	}
	delete ImagePtr;
	this->EarthTextureLoadOrNot = true;
	this->SunTextureLoadOrNot = true;
	return this->EarthTextureLoadOrNot;
}

int Canvas_Sky::ValueOfSkyEstablishment(float Radius, float Longitude, float Latitude) {
	if (this->EarthTextureLoadOrNot == false) {
		LoadOpenGLTexture("./Data/SKY.bmp");
	}

	int List = glGenLists(1);	// create a list for display to improve efficiency
	glNewList(List, GL_COMPILE);
	glBindTexture(GL_TEXTURE_2D, NameOfEarthTeture);

	float angle = 360.0 / Longitude;
	float height = Radius / Latitude;
	for (float curHeight = 0; curHeight < Radius; curHeight += height) {
		glBegin(GL_TRIANGLE_STRIP);
		for (float curAngle = 0; curAngle < 360.0; curAngle += angle) {
			float R_Big = sqrt(pow(Radius, 2) - pow(curHeight + height, 2));
			float R_Small = sqrt(pow(Radius, 2) - pow(curHeight, 2));
			float x_Big = R_Big * cos(curAngle);
			float z_Big = R_Big * sin(curAngle);
			float y_Big = curHeight + height;
			float x_Small = R_Small * cos(curAngle);
			float z_Small = R_Small * sin(curAngle);
			float y_Small = curHeight;
			glTexCoord2f(0.5 + x_Big / (2 * Radius),
				0.5 + z_Big / (2 * Radius));
			glVertex3f(x_Big, y_Big, z_Big);
			glTexCoord2f(0.5 + x_Small / (2 * Radius),
				0.5 + z_Small / (2 * Radius));
			glVertex3f(x_Small, y_Small, z_Small);
		}
		glEnd();
	}

	glEndList();
	return List;
}

int Canvas_Sky::ValueOfSunEstablishment() {
	LoadOpenGLTexture("./Data/NEBULA.bmp");
	glColor4f(1, 0.5f, 0, 0.1);
	glBindTexture(GL_TEXTURE_2D, NameOfSunTexture);

	glBegin(GL_TRIANGLE_STRIP);
	glTexCoord2f(1, 1); 
	glVertex4f(2, 2, 0, 0.5);	 // Top Right
	glTexCoord2f(0, 1); 
	glVertex4f(-2, 2, 0, 0.5);	 // Top Left
	glTexCoord2f(1, 0); 
	glVertex4f(2, -2, 0, 0.5);	 // Bottom Right
	glTexCoord2f(0, 0); 
	glVertex4f(-2, -2, 0, 0.5);	 // Bottom Left
	glEnd();
	return 0;
}

