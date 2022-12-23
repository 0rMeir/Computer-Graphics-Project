#include "Camera.h"
#define _USE_MATH_DEFINES
#include "math.h"
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
	camLocalTranslateMat = glm::inverse(camLocalTranslateMat);
	camUpdate();
}

void Camera::camScaleLocal(float x, float y, float z)
{
	camLocalScaleMat = glm::scale(glm::vec3(x, y, z));
	camLocalScaleMat = glm::inverse(camLocalScaleMat);
	camUpdate();
}

void Camera::camRotateLocal(float x, float y, float z)
{
	CamlocalRotationMat = glm::rotate( (float)(x * (M_PI / 180)), glm::vec3(1, 0, 0));
	CamlocalRotationMat *= glm::rotate( (float)(y * (M_PI / 180)), glm::vec3(0, 1, 0));
	CamlocalRotationMat *= glm::rotate((float)(z * (M_PI / 180)), glm::vec3(0, 0, 1));
	CamlocalRotationMat = glm::inverse(CamlocalRotationMat);
	camUpdate();
}


void Camera::camTranslateWorld(float x, float y, float z)
{
	CamworldTranslateMat = glm::translate( glm::vec3(x, y, z));
	CamworldTranslateMat = glm::inverse(CamworldTranslateMat);
	camUpdate();
}

void Camera::camScaleWorld(float x, float y, float z)
{
	camWorldScaleMat = glm::scale(glm::vec3(x, y, z));
	camWorldScaleMat = glm::inverse(camWorldScaleMat);
	camUpdate();
}

void Camera::camRotateWorld(float x, float y, float z)
{
	CamworldRotationMat = glm::rotate( (float)(x * (M_PI / 180)), glm::vec3(1, 0, 0));
	CamworldRotationMat *= glm::rotate((float)(y * (M_PI / 180)), glm::vec3(0, 1, 0));
	CamworldRotationMat *= glm::rotate((float)(z * (M_PI / 180)), glm::vec3(0, 0, 1));

	CamworldRotationMat = glm::inverse(CamworldRotationMat);
	camUpdate();
}

void Camera::setViewTransformation()
{
	view_transformation = Camworld * Camlocal * lookAt;
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
		//projection_transformation = glm::perspective(left, right, bottom, top);


		left = left * 0.01745329251994329576923690768489f;

		float tanHalfFovy = tan(left / 2.0f);

		glm::mat4x4 result(0);

		result[0][0] = 1.0f / (right * tanHalfFovy);
		result[1][1] = 1.0f / (tanHalfFovy);
		result[2][2] = -(bottom + top) / (bottom - top);
		result[2][3] = -1.0f;
		result[3][2] = -(2.0f * top * bottom) / (top - bottom);

		//glm::mat4 compare = {
		//	(),
		//	(),
		//	(),
		//	()
		//}

		projection_transformation = result;



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
