#include "Model.h"



Model::Model()
{
}


Model::~Model()
{
}


void Model::save(std::string name) {
	this->model->save(("models/" + name).c_str());
}

void Model::load(std::string name)
{
	this->model->load(("models/" + name).c_str());
}