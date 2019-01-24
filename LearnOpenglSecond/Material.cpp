#include "pch.h"
#include "Material.h"

#include "Shader.h"


Material::Material(Shader* _shader, glm::vec3 _diffuse, glm::vec3 _specular, glm::vec3 _ambient, float _shininess)
{
	shader = _shader;
	diffuse=_diffuse;
	specular = _specular;
	ambient = _ambient;
	shininess = _shininess;
}

Material::~Material()
{
}
