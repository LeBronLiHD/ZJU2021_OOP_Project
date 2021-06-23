#define _SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING
// #define GTEST_USE_OWN_TR1_TUPLE

#include "pch.h"
#include "Factory.h"
#include <iostream>


Canvas_Sky* Factory::getCanvas_Sky() {
	return new Canvas_Sky();
}

CHouse* Factory::getModel_House() {
	return new CHouse();
}

CPlane* Factory::getModel_Plane() {
	return new CPlane();
}

GlobalMap* Factory::getGlobalMap() {
	return new GlobalMap();
}

CWater* Factory::getTexture_water() {
	return new CWater();
}

Model* Factory::getModel() {
	return new Model();
}

void Factory::standardFactoryUse() {
	// std::tr1::shared_ptr<Canvas_Sky> Sky(getCanvas_Sky());
	// std::tr1::shared_ptr<GlobalMap> Map(getGlobalMap());
	// std::tr1::shared_ptr<CPlane> Plane(getModel_Plane());
	// std::tr1::shared_ptr<CHouse> House(getModel_House());
	// std::tr1::shared_ptr<CWater> Water(getTexture_water());
	// std::tr1::shared_ptr<Model> Model(getModel());

	// TODO
	// add reasonable process of these opjects
}
