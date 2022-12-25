#include "Camera.h"
#define _USE_MATH_DEFINES
#include "math.h"
#include <cmath>
#include <algorithm>
using namespace std;

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
	CamlocalRotationMat = glm::rotate( (float)(x * (M_PI / 180)), glm::vec3(1, 0, 0));
	CamlocalRotationMat *= glm::rotate( (float)(y * (M_PI / 180)), glm::vec3(0, 1, 0));
	CamlocalRotationMat *= glm::rotate((float)(z * (M_PI / 180)), glm::vec3(0, 0, 1));
	camUpdate();
}


void Camera::camTranslateWorld(float x, float y, float z)
{
	CamworldTranslateMat = glm::translate( glm::vec3(x, y, z));
	camUpdate();
}

void Camera::camScaleWorld(float x, float y, float z)
{
	camWorldScaleMat = glm::scale(glm::vec3(x, y, z));
	camUpdate();
}

void Camera::camRotateWorld(float x, float y, float z)
{
	CamworldRotationMat = glm::rotate((float)(x * (M_PI / 180)), glm::vec3(1, 0, 0));
	CamworldRotationMat *= glm::rotate((float)(y * (M_PI / 180)), glm::vec3(0, 1, 0));
	CamworldRotationMat *= glm::rotate((float)(z * (M_PI / 180)), glm::vec3(0, 0, 1));

	camUpdate();
}

void Camera::setViewTransformation()
{
	view_transformation = glm::inverse(Camworld * Camlocal * lookAt);
}

void Camera::camUpdate()
{
	Camworld = CamworldTranslateMat * CamworldRotationMat * camWorldScaleMat;
	Camlocal = camLocalTranslateMat * CamlocalRotationMat * camLocalScaleMat ;
	setViewTransformation();
}

void Camera::setProjection(float left, float right, float bottom, float top, float near, float far,bool isOrtho = true)
{
	if (isOrtho)
	{
		this->top = top;
		this->bottom = bottom;
		projection_transformation = glm::ortho(left, right, bottom, top, near, far);

		projection_transformation = glm::scale(glm::vec3((2 / (right - left)), (2 / (top - bottom)), (2 / (near - far)))) * glm::translate(glm::vec3(-((right + left) / 2), -((bottom + top) / 2), -((near + far) / 2))) * projection_transformation;

	}
	else
	{
		float angle = 45.0f;
		float aspectRatio = (right - left) / (top - bottom);

		glm::mat4 proj = glm::perspective((float)(M_PI/4), aspectRatio, 1.5f, -10.0f);
	    

		projection_transformation = glm::mat4(0.0f);
		projection_transformation[0][0] = ((2 * near) / (right - left));
		projection_transformation[0][2] = ((right+left) / (right - left));
		projection_transformation[1][1] = ((2 * near) / (top - bottom));
		projection_transformation[1][2] = ((top + bottom) / (top - bottom));
		projection_transformation[2][2] = -((far+near) / (far-near));
		projection_transformation[2][3] = (-((2*far*near) / (far - near)));
		projection_transformation[3][2] = -1;

		projection_transformation = projection_transformation * proj;
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
