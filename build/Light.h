#pragma once
#include <glm/glm.hpp>
#include <string>
#include "MeshModel.h"

class Light
{
public:
	glm::vec3 ambient = glm::vec3(0, 0, 1);
	glm::vec3 diffuse = glm::vec3(1, 0, 0);
	glm::vec3 specular = glm::vec3(1, 0, 0);
	glm::vec3 pos = glm::vec3(500, 500, -100);
	float intensity = 10;
	glm::vec3 computeColor(MeshModel& model, glm::vec3 fragPos, glm::vec3 normal, glm::vec3 cameraPos);
};