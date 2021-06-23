#pragma once

#define _SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING

#include "Canvas_Sky.h"
#include "Model_House.h"
#include "Model_Plane.h"
#include "GlobalMap.h"
#include "Texture_water.h"
#include "singleton.h"


class Factory	// C++ factory mode
{
public:
	virtual Canvas_Sky* getCanvas_Sky();
	virtual CHouse* getModel_House();
	virtual CPlane* getModel_Plane();
	virtual GlobalMap* getGlobalMap();
	virtual CWater* getTexture_water();
	virtual Model* getModel();

public:
	void standardFactoryUse();
};

typedef NormalSingleton< Factory > FactoryIn;


template<typename T>
void global_swap(T& lhs, T& rhs) {
	T temp = lhs;
	lhs = rhs;
	rhs = temp;
}
