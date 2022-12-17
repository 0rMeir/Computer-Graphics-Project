#include "Camera.h"


Camera::Camera(){}

Camera::~Camera(){}

const glm::mat4x4& Camera::GetProjectionTransformation() const
{
	return projection_transformation;
}

const glm::mat4x4& Camera::GetViewTransformation() const
{
	return view_transformation;
}

void Camera::SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up)
{
	lookAt = glm::lookAt(eye, at, up);
	CamworldTranslateMat = glm::mat4(1.0f);
	CamworldRotationMat = glm::mat4(1.0f);
	SetViewTransformation();
}

void Camera::CamtranslateWorld(float x, float y, float z)
{
	CamworldTranslateMat = glm::translate(CamworldTranslateMat, glm::vec3(x, y, z));
	Camupdate();
}

void Camera::CamrotateWorldl(float x, float y, float z)
{
	CamworldRotationMat = glm::rotate(CamworldRotationMat,glm::radians(x), glm::vec3(1, 0, 0));
	CamworldRotationMat = glm::rotate(CamworldRotationMat, glm::radians(y), glm::vec3(0, 1, 0));
	CamworldRotationMat = glm::rotate(CamworldRotationMat,glm::radians(z), glm::vec3(0, 0, 1));
	Camupdate();
}

void Camera::SetViewTransformation()
{
	view_transformation = glm::inverse(Camworld * Camlocal) * lookAt;
}
void Camera::Camupdate()
{
	Camworld = CamworldTranslateMat * CamworldRotationMat * CamworldScaleMat;
	Camlocal = CamlocalTranslateMat * CamlocalRotationMat * CamlocalScaleMat;
}

void Camera::SetProjection(float left, float right, float bottom, float top, float near, float far)
{
	this->top = top;
	this->bottom = bottom;
	projection_transformation = glm::ortho(left, right, bottom, top, near, far);
}

void Camera::SetViewPort(float width, float height)
{
	float scale = height / (top - bottom);
	//float translatex = width / (right - left);

	view_port = glm::translate(glm::vec3(width/2,height/2,0)) * glm::scale(glm::vec3(scale,scale,scale));

}

glm::mat4x4 Camera::getViewPort()
{
	return view_port;
}

glm::mat4x4 Camera::GetProjectionTransformation()
{
	return projection_transformation;
}
