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

{
	glm::vec3 ver = vertices[0];
	minX = ver.x;
	minY = ver.y;
	minZ = ver.z;
	maxX = ver.x;
	maxY = ver.y;
	maxZ = ver.z;
	
	for (int i = 0; i < vertices.size(); i++)
	{
		ver = vertices[i];
		minX = (ver.x < minX) ? ver.x : minX;
		minY = (ver.y < minY) ? ver.y : minY;
		minZ = (ver.z < minZ) ? ver.z : minZ;

		maxX = (ver.x > maxX) ? ver.x : maxX;
		maxY = (ver.y > maxY) ? ver.y : maxY;
		maxZ = (ver.z > maxZ) ? ver.z : maxZ;

	}

}

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

int MeshModel::GetVerticesNormalCount() const
{
	return normals.size();
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
	updateLocal();
}

void MeshModel::scaleLocal(float x,float y)
{
	localScaleMat = glm::scale(glm::vec3(x, x, x));
	updateLocal();
}

void MeshModel::rotateLocalX(float x)
{
	localRotationMatX = glm::rotate((float)(x * (M_PI / 180)), glm::vec3(1, 0 , 0));
}

void MeshModel::rotateLocalY(float x)
{
	localRotationMatY = glm::rotate((float)(x * (M_PI / 180)), glm::vec3(0, 1, 0));
}

void MeshModel::rotateLocalZ(float x)
{
	localRotationMatZ = glm::rotate((float)(x * (M_PI / 180)), glm::vec3(0,0, 1));
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
	worldScaleMat = glm::scale(glm::vec3(x, x, x));
	updateWorld();
}


//1880 / 2
void MeshModel::rotateWorldlX(float x)
{
	worldRotationMatX = glm::rotate((float)(x * (M_PI / 180)), glm::vec3(1, 0, 0));
	
}

//  1320 / 2
void MeshModel::rotateWorldY(float x)
{
	worldRotationMatY = glm::rotate((float)(x * (M_PI / 180)), glm::vec3(0, 1, 0));
	
}

void MeshModel::rotateWorldZ(float x)
{
	worldRotationMatZ = glm::rotate((float)(x * (M_PI / 180)), glm::vec3(0, 0, 1));
	
}

void MeshModel::updateWorld()
{
	worldTransformMat = worldScaleMat * worldTranslateMat;
}

glm::vec4 MeshModel::transform(glm::vec4 v)
{
	v = worldTranslateMat * worldRotationMatX * worldRotationMatY * worldRotationMatZ * worldScaleMat *
		localTranslateMat * localRotationMatX * localRotationMatY * localRotationMatZ * localScaleMat * v;
	return v;	
}

glm::mat4 MeshModel::getWorldTransform()
{
	return (worldScaleMat * worldRotationMatX * worldRotationMatY * worldRotationMatZ * worldTranslateMat);
}

glm::vec3 MeshModel::getVertexNormal(int i)
{
	return normals[i];
}


