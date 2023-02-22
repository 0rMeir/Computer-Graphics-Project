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
	glm::vec3 _ambient = glm::vec3(0.0f), _diffuse = glm::vec3(0.0f), _specular = glm::vec3(0.0f);



	_ambient = model.ambient * ambient;


	normal = glm::normalize(normal);


	glm::vec3 lightDir = glm::normalize(pos - fragPos);
	float diff = fmax(glm::dot(normal, lightDir), 0.0);
	_diffuse = diff * (model.diffuse * diffuse);
	if (_diffuse.x > 1) { _diffuse.x = 1; }
	if (_diffuse.x < 0) { _diffuse.x = 0; }
	if (_diffuse.y > 1) { _diffuse.y = 1; }
	if (_diffuse.y < 0) { _diffuse.y = 0; }
	if (_diffuse.z > 1) { _diffuse.z = 1; }
	if (_diffuse.z < 0) { _diffuse.z = 0; }




	glm::vec3 viewDir = glm::normalize(cameraPos - fragPos);
	glm::vec3 reflect = glm::normalize(glm::reflect(-lightDir, normal));
	float spec = pow(std::max((glm::dot(reflect, viewDir)), 0.0f), intensity);
	_specular = specular * spec * model.specular;




	if (ambient == glm::vec3(0, 0, 0))
	{
		_ambient = glm::vec3(0, 0, 0);
	}
	if (diffuse == glm::vec3(0, 0, 0))
	{
		_diffuse = glm::vec3(0, 0, 0);
	}
	if (specular == glm::vec3(0, 0, 0))
	{
		_specular = glm::vec3(0, 0, 0);
	}

	glm::vec3 ret = _ambient + _diffuse + _specular;
	if (ret.x > 1) { ret.x = 1; }
	if (ret.x < 0) { ret.x = 0; }
	if (ret.y > 1) { ret.y = 1; }
	if (ret.y < 0) { ret.y = 0; }
	if (ret.z > 1) { ret.z = 1; }
	if (ret.z < 0) { ret.z = 0; }


	return (_ambient + _diffuse + _specular);
}