#include <memory>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include "Utils.h"
#include "Light.h"

glm::vec3 Light::computeColor(MeshModel& model, glm::vec3 fragPos, glm::vec3 normal, glm::vec3 cameraPos)
{
	glm::vec3 _ambient, _diffuse, _specular;
	_ambient = model.ambient*ambient;  //*ambient
	normal = glm::normalize(normal);
	glm::vec3 lightDir = glm::normalize(pos - fragPos);
	float diff = fmax(glm::dot(normal,lightDir),0.0);
	_diffuse = model.diffuse * diffuse * diff;
	glm::vec3 viewDir = glm::normalize(cameraPos - fragPos);
	glm::vec3 reflect = glm::normalize(glm::reflect(-lightDir, normal));
	float spec = pow(glm::dot(reflect, viewDir), intensity);
	_specular = model.specular * specular * spec;
	return (_ambient+ _diffuse+_specular);
}
