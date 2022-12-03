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
void MeshModel::TranslateLocal(float x, float y, float z)
{
	LTranslate = glm::translate(LTranslate, glm::vec3(x, y, z));
	Transform = scaleMat * LTranslate;
	
}

void MeshModel::scale(float x,float y)
{
	scaleMat = glm::scale(scaleMat, glm::vec3(x, y, 1));
	Transform = scaleMat * LTranslate;

}



void MeshModel::rotate(float x)
{


}