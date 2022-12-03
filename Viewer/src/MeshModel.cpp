#include "MeshModel.h"
#include "math.h"

MeshModel::MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, const std::string& model_name) :
	faces(faces),
	vertices(vertices),
	normals(normals)
{


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
glm::vec3 MeshModel::GetVertex(int i) {
	return vertices[i];
}
const std::string& MeshModel::GetModelName() const
{
	return model_name;
}
void MeshModel::translateLocal(float x, float y, float z)
{
	float temp = x;
	if (x > 0)
	{
		x = (x > lastLocalT.x) ? x : x - lastLocalT.x;
	}
	else
	{
		x = (x > lastLocalT.x) ? x - lastLocalT.x : x;
	}
	lastLocalT.x = temp;

	localTransformMat = glm::translate(localTransformMat, glm::vec3(x, y, z));
	updateLocal();
}

void MeshModel::scale(float x,float y)
{
	float temp = x;

	if (x > 1)
	{
		x = (x > lastLocalS.x) ? x : x/lastLocalS.x;
	}
	else if (x == 1)
	{
		x =  lastLocalS.x;
	}
	else
	{
	}

	lastLocalS.x = temp;

	localScaleMat = glm::scale(localScaleMat, glm::vec3(x, x, x));
	updateLocal();
}


void MeshModel::rotate(float x)
{


}

void MeshModel::updateLocal()
{

	localTransformMat = localScaleMat * localTransformMat;

}


