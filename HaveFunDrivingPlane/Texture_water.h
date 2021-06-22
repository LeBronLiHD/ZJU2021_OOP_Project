#pragma comment(lib, "glaux.lib")
#include <GL\glaux.h>
#include <GL\glut.h>
#include "math.h"
#include "params.h"

class CWater
{
private:
	bool isLoadWater;
	GLuint textureWater;
public:
	CWater ();
	~CWater ();

	virtual void set_isLoadWater(bool cur_isLoadWater) { this->isLoadWater = cur_isLoadWater; }
	virtual bool get_isLoadWater() { return this->isLoadWater; }
	virtual void set_textureWater(GLuint cur_textureWater) { this->textureWater = cur_textureWater; }
	virtual GLuint get_textureWater() { return this->textureWater; }

	// AUX_RGBImageRec* loadbmp(char *filename);
	bool LoadGLTexture(char *filename);
	int CreatWater(int a);
};