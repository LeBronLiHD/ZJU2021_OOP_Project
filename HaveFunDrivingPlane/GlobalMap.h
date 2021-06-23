#pragma once
#pragma comment(lib, "glaux.lib")

#include <gl/glew.h>
#include <gl/glut.h>
#include <gl/glaux.h>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>

#include "params.h"

class GlobalMap
{
private:
	int StepSizeInAxis;
	bool RawFileReadOrNot;
	BYTE Map[PARAM::MapWidth][PARAM::MapHeight];
	GLfloat xTexture_1, xTexture_2, yTexture_1, ytexture_2;
	glm::vec3 MapNormal[PARAM::MapWidth][PARAM::MapHeight];

public:
	GlobalMap();
	GlobalMap(const GlobalMap& rhs);
	~GlobalMap();

	void swap(GlobalMap& rhs);
	GlobalMap& operator=(const GlobalMap& rhs);

	bool ReadRawFile(char* filePath);
	BYTE GetGlobalMapHeight(int PosX, int PosZ);
	// AUX_RGBImageRec* LoadBmpImage(char* filePath);
	bool LoadGlobalMapTexture();
	int ValueOfGlobalMapEstablishment();
	void GetGlobalMapNormalValue();
	int ValueOfGridEstablishment();

	virtual int getSizeOfGlobalMap() { return SizeOfGlobalMap; }
	virtual void setSizeOfGlobalMap(int curSizeOfGlobalMap) { SizeOfGlobalMap = curSizeOfGlobalMap; }
	virtual GLuint getTextureValue() { return TextureValue; }
	virtual void setTextureValue(GLuint curTextureValue) { TextureValue = curTextureValue; }
	virtual bool getIsCalculatedNormal() { return IsCalculatedNormal; }
	virtual void setIsCalculatedNormal(bool curIsCalculatedNormal) { IsCalculatedNormal = curIsCalculatedNormal; }
	virtual bool getIsMapLoadTexture() { return IsMapLoadTexture; }
	virtual void setIsMapLoadTexture(bool curIsMapLoadTexture) { IsMapLoadTexture = curIsMapLoadTexture; }

	virtual int getStepSizeInAxis() { return StepSizeInAxis; }
	virtual void setStepSizeInAxis(int curStepSizeInAxis) { StepSizeInAxis = curStepSizeInAxis; }
	virtual bool getRawFileReadOrNot() { return RawFileReadOrNot; }
	virtual void setRawFileReadOrNot(bool curRawFileReadOrNot) { RawFileReadOrNot = curRawFileReadOrNot; }

public:
	int SizeOfGlobalMap;
	GLuint TextureValue;
	bool IsMapLoadTexture;
	bool IsCalculatedNormal;
};

