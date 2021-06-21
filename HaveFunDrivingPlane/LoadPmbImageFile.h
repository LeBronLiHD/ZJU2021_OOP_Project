#pragma once

#include "singleton.h"

typedef struct ImageFileDate {
	unsigned long sizeX;
	unsigned long sizeY;
	unsigned char* data;
} PmbImage;

class LoadPmbImageFile
{
public:
	PmbImage* LoadImageFile(char* filename);
};

typedef NormalSingleton< LoadPmbImageFile > LoadPmbImageFileIn;
