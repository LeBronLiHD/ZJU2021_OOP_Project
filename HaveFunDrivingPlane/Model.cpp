#include "pch.h"
#include "Model.h"
#include "Factory.h"

Model::Model(const Model& rhs) {
	this->faces = rhs.faces;
	this->isreadstl = rhs.isreadstl;
	this->faceNum = rhs.faceNum;
	this->isLoadTexture = rhs.isLoadTexture;
	this->texture = rhs.texture;
}

void Model::swap(Model& rhs) {
	global_swap(this->faces, rhs.faces);
	global_swap(this->isreadstl, rhs.isreadstl);
	global_swap(this->faceNum, rhs.faceNum);
	global_swap(this->isLoadTexture, rhs.isLoadTexture);
	global_swap(this->texture, rhs.texture);
}

Model& Model::operator=(const Model& rhs) {
	Model temp(rhs);
	swap(temp);
	return *this;
}
