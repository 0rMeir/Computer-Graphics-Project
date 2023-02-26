#define _USE_MATH_DEFINES
#include <cmath>
#include "MeshModel.h"
#include "math.h"
#include <algorithm>
using namespace std;


MeshModel::MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, std::vector<glm::vec2> textures, const std::string& model_name) :
	faces(faces),
	vertices(vertices),
	normals(normals), textureCoords(textures)

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

	vertexim.reserve(3 * faces.size());
	for (int i = 0; i < faces.size(); i++)
	{
		//cout << "Face" << i << ": ";
		Face face = faces.at(i);
		for (int j = 0; j < 3; j++)
		{
			int v = face.GetVertexIndex(j) - 1;
			Vertex vertex;
			vertex.position = vertices[v];

			if (normals.size() > 0)
			{
				int n = face.GetNormalIndex(j) - 1;
				vertex.normal = normals[n];
			}

			if (textureCoords.size() > 0)
			{
				int t = face.GetTextureIndex(j) - 1;
				vertex.textureCoordinates = textureCoords[t];
			}

			//cout << "(" << vertex.position.x << "," << vertex.position.y << "," << vertex.position.z << ") | ";
			
			vertexim.push_back(vertex);

		}

		//cout << endl;
	}

	/*int i = 0;
	for (Vertex vertex : vertexim)
	{
		cout << "vertex" << i++ << ": " << "(" << vertex.position.x << "," << vertex.position.y << "," << vertex.position.z << ") " << endl;;
	}*/


	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertexim.size() * sizeof(Vertex), &vertexim[0], GL_STATIC_DRAW);

	// Vertex Positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Normals attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Vertex Texture Coords
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// unbind to make sure other code does not change it somewhere else
	glBindVertexArray(0);

}



MeshModel::~MeshModel()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
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
	worldTransformMat = worldTranslateMat * worldRotationMatX * worldRotationMatY * worldRotationMatZ * worldScaleMat;
}

glm::vec4 MeshModel::transform(glm::vec4 v)
{
	v = worldTranslateMat * worldRotationMatX * worldRotationMatY * worldRotationMatZ * worldScaleMat *
		localTranslateMat * localRotationMatX * localRotationMatY * localRotationMatZ * localScaleMat * v;
	return v;	
}

glm::mat4 MeshModel::getModelTransformation()
{
	return (worldTranslateMat * worldRotationMatX * worldRotationMatY * worldRotationMatZ * worldScaleMat *
		localTranslateMat * localRotationMatX * localRotationMatY * localRotationMatZ * localScaleMat);
}

glm::mat4 MeshModel::getWorldTransform()
{
	return (worldTransformMat);
}

glm::vec3 MeshModel::getVertexNormal(int i)
{
	return normals[i];
}


GLuint MeshModel::GetVAO() const
{
	return vao;
}

std::vector<Vertex>& MeshModel::GetVertices()
{
	return vertexim;
}

