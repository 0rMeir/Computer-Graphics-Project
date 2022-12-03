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
	void translateLocal(float x, float y, float z);
	void translateWorld(float x, float y, float z);
	void updateLocal();
	void scaleLocal(float x,float y);
	void rotateLocalX(float x);
	void rotateLocalY(float x);
	glm::mat4 rotateMat = glm::mat4(1.0f);
	glm::vec3 modelCenter = glm::vec3(1880/2, 1320/2,0);
	glm::mat4 localTransformMat = glm::mat4(1.0f);
	glm::mat4 localScaleMat = glm::mat4(1.0f);
	glm::mat4 localTranslateMat = glm::mat4(1.0f);
	glm::mat4 localRotationMatX = glm::mat4(1.0f);
	glm::mat4 localRotationMatY = glm::mat4(1.0f);
	glm::mat4 localRotationMatZ = glm::mat4(1.0f);


private:
	
	std::vector<Face> faces;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::string model_name;
};
