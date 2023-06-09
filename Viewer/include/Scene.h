#pragma once

#include <vector>
#include <memory>

#include "Camera.h"
#include "MeshModel.h"
#include "Light.h"

using namespace std;

class Scene {
public:
	Scene(int w,int h);
	int width;
	int height;
	bool showAxis=false;
	bool showBoundingBox=false;
	bool showNormals = false;
	bool drawRec = false;

	bool showAmbientProduct = false;

	void AddModel(const shared_ptr<MeshModel>& mesh_model);
	int GetModelCount() const;
	MeshModel& GetModel(int index) const;
	MeshModel& GetActiveModel() const;
	bool showRectangles = false;
	bool wireFrame = false;
	
	void AddCamera(const shared_ptr<Camera>& camera);
	int GetCameraCount() const;
	Camera& GetCamera(int index);
	Camera& GetActiveCamera();

	void SetActiveCameraIndex(int index);
	int GetActiveCameraIndex() const;

	void SetActiveModelIndex(int index);
	int GetActiveModelIndex() const;

	int getModelsNumber() const;
	void setModelsNumber(int amount);
	Camera& Scene::getActiveCamera() const;
	vector<Light*> lights;
	mutable Light sceneLight;

	bool ambient = false , diffuse=false , specular = false;

	bool isUseTexture = false;
	bool isNormalMap=false;
	float amount = 2.0f;
	bool isToonShading = false;

private:
	vector<shared_ptr<MeshModel>> mesh_models;
	vector<shared_ptr<Camera>> cameras;
	
	int active_camera_index;
	int active_model_index;
	int modelsNumber = 0;
};