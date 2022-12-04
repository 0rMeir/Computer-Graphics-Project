#define _USE_MATH_DEFINES
#include <cmath>
#include "MeshModel.h"
#include "math.h"
#include <algorithm>
using namespace std;

MeshModel::MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, const std::string& model_name) :
	faces(faces),
	vertices(vertices),
	normals(normals)
{}

MeshModel::~MeshModel()
{
}

const Face& MeshModel::GetFace(int index) const
{
	return faces[index];
}

int MeshModel::GetFacesCount() const
{
	return faces.size();
}

int MeshModel::GetVerticesCount() const
{
	return vertices.size();
}

glm::vec3 MeshModel::GetVertex(int i) {
	return vertices[i];
}

const std::string& MeshModel::GetModelName() const
{
	return model_name;
}

void MeshModel::translateLocal(float x, float y, float z)
{
	localTranslateMat = glm::translate(glm::vec3(x, y, z));
	modelCenter.x += x;
	modelCenter.y += y;
	updateLocal();
}

void MeshModel::scaleLocal(float x,float y)
{
	localScaleMat = glm::scale(glm::vec3(x, x, 1));
	updateLocal();
}

void MeshModel::rotateLocalX(float x)
{
	localRotationMatX = glm::rotate((float)(x * (M_PI / 180)), glm::vec3(1, 0 , 0));
	updateLocal();	
}

void MeshModel::rotateLocalY(float x)
{
	localRotationMatY = glm::rotate((float)(x * (M_PI / 180)), glm::vec3(0, 1, 0));
	updateLocal();
}

void MeshModel::updateLocal()
{
	localTransformMat = localScaleMat * localTranslateMat;
}

void MeshModel::translateWorld(float x, float y, float z)
{
	worldTranslateMat = glm::translate(glm::vec3(x, y, z));
	updateWorld();
}

void MeshModel::scaleWorld(float x, float y)
{
	worldScaleMat = glm::scale(glm::vec3(x, x, 1));
	updateWorld();
}

void MeshModel::rotateWorldlX(float x)
{
	worldRotationMatX = glm::rotate((float)(x * (M_PI / 180)), glm::vec3(1880 / 2, 0, 0));
	updateWorld();
}

void MeshModel::rotateWorldY(float x)
{
	worldRotationMatY = glm::rotate((float)(x * (M_PI / 180)), glm::vec3(0, 1320 / 2, 0));
	updateWorld();
}

void MeshModel::updateWorld()
{
	worldTransformMat = worldScaleMat * worldTranslateMat * worldRotationMatX*worldRotationMatY;
}


glm::vec4 MeshModel::transform(glm::vec4 v)
{
	v = worldTransformMat * worldRotationMatX * worldRotationMatY * localTransformMat * localRotationMatX * localRotationMatY * v;
	return v;	
}


