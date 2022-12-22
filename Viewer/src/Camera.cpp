#include "Camera.h"

Camera::Camera(){}

Camera::~Camera(){}



const glm::mat4x4& Camera::getProjectionTransformation() const
{
	return projection_transformation;
}

const glm::mat4x4& Camera::getViewTransformation() const
{
	return view_transformation;
}

void Camera::setCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up)
{
	lookAt = glm::lookAt(eye, at, up);
	CamworldTranslateMat = glm::mat4(1.0f);
	CamworldRotationMat = glm::mat4(1.0f);
	setViewTransformation();
}


void Camera::camTranslateLocal(float x, float y, float z)
{
	camLocalTranslateMat = glm::translate( glm::vec3(x, y, z));
	camUpdate();
}

void Camera::camScaleLocal(float x, float y, float z)
{
	camLocalScaleMat = glm::scale(glm::vec3(x, y, z));
	camUpdate();
}

void Camera::camRotateLocal(float x, float y, float z)
{
	CamlocalRotationMat = glm::rotate(CamlocalRotationMat, glm::radians(x), glm::vec3(1, 0, 0));
	CamlocalRotationMat = glm::rotate(CamlocalRotationMat, glm::radians(y), glm::vec3(0, 1, 0));
	CamlocalRotationMat = glm::rotate(CamlocalRotationMat, glm::radians(z), glm::vec3(0, 0, 1));
	camUpdate();
}


void Camera::camTranslateWorld(float x, float y, float z)
{
	CamworldTranslateMat = glm::translate(CamworldTranslateMat, glm::vec3(x, y, z));
	camUpdate();
}

void Camera::camScaleWorld(float x, float y, float z)
{
	camWorldScaleMat = glm::scale(camWorldScaleMat,glm::vec3(x, y, z));
	camUpdate();
}

void Camera::camRotateWorld(float x, float y, float z)
{
	CamworldRotationMat = glm::rotate(CamworldRotationMat,glm::radians(x), glm::vec3(1, 0, 0));
	CamworldRotationMat = glm::rotate(CamworldRotationMat, glm::radians(y), glm::vec3(0, 1, 0));
	CamworldRotationMat = glm::rotate(CamworldRotationMat,glm::radians(z), glm::vec3(0, 0, 1));
	camUpdate();
}

void Camera::setViewTransformation()
{
	view_transformation = glm::inverse(Camworld * Camlocal) * lookAt;
}
void Camera::camUpdate()
{
	Camworld = CamworldTranslateMat * CamworldRotationMat * camWorldScaleMat;
	Camlocal = camLocalTranslateMat * CamlocalRotationMat * camLocalScaleMat;
	setViewTransformation();
}

void Camera::setProjection(float left, float right, float bottom, float top, float near, float far,bool isOrtho = true)
{
	if (isOrtho)
	{
		this->top = top;
		this->bottom = bottom;
		projection_transformation = glm::ortho(left, right, bottom, top, near, far);
	}
	else
	{
		projection_transformation = glm::perspective(left, right, bottom, top);
	}	
}

void Camera::setViewPort(float width, float height)
{
	float scale = height / (top - bottom);
	//float translatex = width / (right - left);

	view_port = glm::translate(glm::vec3(width/2,height/2,0)) * glm::scale(glm::vec3(scale/2, scale/2, scale/2));
}

glm::mat4x4 Camera::getViewPort()
{
	return view_port;
}

glm::mat4x4 Camera::getProjectionTransformation()
{
	return projection_transformation;
}
