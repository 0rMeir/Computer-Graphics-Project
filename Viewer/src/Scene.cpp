#include "Scene.h"
#include "MeshModel.h"
#include <string>

Scene::Scene(int theWidth, int theHeight) :width(theWidth), height(theHeight),
	active_camera_index(0),
	active_model_index(0)
{
	std::shared_ptr<Camera> cam = std::make_shared<Camera>();
	cam->setProjection(-1, 1, -1, 1 , -1, 1,true);
	cam->setViewPort(width, height);
	cameras.push_back(cam);
}

void Scene::AddModel(const std::shared_ptr<MeshModel>& mesh_model)
{
	mesh_models.push_back(mesh_model);
	setModelsNumber(getModelsNumber()+1);
}

int Scene::getModelsNumber() const
{
	return modelsNumber;
}

void Scene::setModelsNumber(int num)
{
	modelsNumber = num;
}

int Scene::GetModelCount() const
{
	return mesh_models.size();
}

MeshModel& Scene::GetModel(int index) const
{
	return *mesh_models[index];
}

MeshModel& Scene::GetActiveModel() const
{
	return *mesh_models[active_model_index];
}

void Scene::AddCamera(const std::shared_ptr<Camera>& camera)
{
	cameras.push_back(camera);
}

int Scene::GetCameraCount() const
{
	return cameras.size();
}

Camera& Scene::GetCamera(int index)
{
	return *cameras[index];
}

Camera& Scene::GetActiveCamera()
{
	return *cameras[active_camera_index];
}

void Scene::SetActiveCameraIndex(int index)
{
	active_camera_index = index;
}

int Scene::GetActiveCameraIndex() const
{
	return active_camera_index;
}

void Scene::SetActiveModelIndex(int index)
{
	active_model_index = index;
}

int Scene::GetActiveModelIndex() const
{
	return active_model_index;
}

Camera& Scene::getActiveCamera() const 
{
	return *cameras[0];
}

