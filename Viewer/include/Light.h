#pragma once
#include <glm/glm.hpp>
#include <string>
#include "MeshModel.h"

class Light
{
public:
	glm::vec3 ambient = glm::vec3(20, 10, 20);
	glm::vec3 diffuse = glm::vec3(90, 80, 46);
	glm::vec3 specular = glm::vec3(255, 255, 255);
	glm::vec3 pos = glm::vec3(500, 500, -200);
	float intensity = 20;
	glm::vec3 computeColor(MeshModel& model, glm::vec3 fragPos, glm::vec3 normal, glm::vec3 cameraPos);
};
