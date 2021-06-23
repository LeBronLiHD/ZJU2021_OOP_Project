#pragma once
#pragma comment(lib, "glaux.lib")
#include <gl/glew.h>
#include <gl/glaux.h>
#include <gl/glut.h>
#include "Model.h"

#pragma once
class CPlane: public Model
{
public:
	CPlane();
	CPlane(const CPlane& rhs);
	~CPlane();

	void swap(CPlane& rhs);
	CPlane& operator=(const CPlane& rhs);

	bool Readstl(char *filename);
	// AUX_RGBImageRec* LoadBmp(char *filename);
	bool LoadGLTexture(char *filename);
	int CreatPlane();
};


