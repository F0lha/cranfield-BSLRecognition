#include "Model.h"



Model::Model() {
}


Model::~Model() {
}

void Model::save(std::string name) {
	model->save(name.c_str());
}

void Model::load(std::string name) {
	model->load(name.c_str());
}