#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include "Face.h"
#include <glm/gtx/transform.hpp>
#include <iostream>
#include <memory>
#include "MeshModel.h"


struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 textureCoordinates;
};

class MeshModel
{
public:
	glm::mat4 MeshModel::getModelTransformation();
	GLuint GetVAO() const;
	std::vector<Vertex>& GetVertices();
	MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, std::vector<glm::vec2> textures, const std::string& model_name);
	virtual ~MeshModel();
	const Face& GetFace(int index) const;
	int GetFacesCount() const;
	int GetVerticesCount()const;
	glm::vec3 GetVertex(int i);
	glm::vec3 getVertexNormal(int i);
	int GetVerticesNormalCount() const;
	const std::string& GetModelName() const;
	void updateWorld();
	void translateWorld(float x, float y, float z);
	void rotateWorldlX(float x);
	void rotateWorldY(float x);
	void rotateWorldZ(float x);
	void scaleWorld(float x, float y);
	void updateLocal();
	void translateLocal(float x, float y, float z);
	void scaleLocal(float x,float y);
	void rotateLocalX(float x);
	void rotateLocalY(float x);
	void rotateLocalZ(float x);

	//ambient, diffuse and specular
	glm::vec3 ambient = glm::vec3(0,0,0);
	glm::vec3 diffuse=glm::vec3(1, 0, 0);
	glm::vec3 specular= glm::vec3(1, 1, 1);

	glm::mat4 MeshModel::getWorldTransform();

	glm::vec4 transform(glm::vec4 v);

	glm::mat4 worldTransformMat = glm::mat4(1.0f);
	glm::mat4 localTransformMat = glm::mat4(1.0f);
	glm::vec3 modelCenter = glm::vec3(1880 / 2, 1320 / 2, 0);
	glm::vec3 newModelCenter = glm::vec3(1880 / 2, 1320 / 2,0);

	float minX;
	float minY;
	float minZ;
	float maxX;
	float maxY;
	float maxZ;

	std::vector<Face> faces;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> textureCoords;
	std::vector<Vertex> vertexim;


protected:

	glm::mat4 localScaleMat = glm::mat4(1.0f);
	glm::mat4 localTranslateMat = glm::mat4(1.0f);
	glm::mat4 localRotationMatX = glm::mat4(1.0f);
	glm::mat4 localRotationMatY = glm::mat4(1.0f);
	glm::mat4 localRotationMatZ = glm::mat4(1.0f);
	glm::mat4 worldScaleMat = glm::mat4(1.0f);
	glm::mat4 worldTranslateMat = glm::mat4(1.0f);
	glm::mat4 worldRotationMatX = glm::mat4(1.0f);
	glm::mat4 worldRotationMatY = glm::mat4(1.0f);
	glm::mat4 worldRotationMatZ = glm::mat4(1.0f);
	std::string model_name;

private:

	GLuint vbo;
	GLuint vao;

};
