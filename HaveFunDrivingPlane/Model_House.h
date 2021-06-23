#pragma once
#pragma comment(lib, "glaux.lib")
#include <gl/glew.h>
#include <gl/glaux.h>
#include <gl/glut.h>
#include "Model.h"

#pragma once
class CHouse:public Model
{
public:
	CHouse();
	CHouse(const CHouse& rhs);
	~CHouse();

	void swap(CHouse& rhs);
	CHouse& operator=(const CHouse& rhs);

	bool Readstl(char *filename);
	// AUX_RGBImageRec* LoadBmp(char *filename);
	bool LoadGLTexture(char *filename);
	int CreatHouse();
};

