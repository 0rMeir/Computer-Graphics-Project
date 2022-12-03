#pragma once
#include <glm/glm.hpp>
#include <string>
#include "Face.h"
#include <glm/gtx/transform.hpp>
#include <iostream>

class MeshModel
{
public:
	MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, const std::string& model_name);
	virtual ~MeshModel();
	const Face& GetFace(int index) const;
	int GetFacesCount() const;
	int GetVerticesCount()const;
	glm::vec3 GetVertex(int i);
	const std::string& GetModelName() const;
	void TranslateLocal(float x, float y, float z);
	void scale(float x,float y);
	void rotate(float x);
	glm::mat4 Transform = glm::mat4(1.0f);
	glm::mat4 LTranslate = glm::mat4(1.0f);
	glm::mat4 scaleMat = glm::mat4(1.0f);
	glm::mat4 rotateMat = glm::mat4(1.0f);


private:
	
	std::vector<Face> faces;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::string model_name;
};
