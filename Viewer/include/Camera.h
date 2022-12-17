#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

class Camera
{
public:
	Camera();
	virtual ~Camera();
	const glm::mat4x4& GetProjectionTransformation() const;
	const glm::mat4x4& GetViewTransformation() const;
	void SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up);
	void SetProjection(float left, float right, float buttom, float top, float near, float far);
	glm::vec3 eye = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 at = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

	void CamtranslateWorld(float x, float y, float z);
	void CamrotateWorldl(float x, float y, float z);
	void CamscaleWorld(float x, float y, float z);
	void Camupdate();
	void CamtranslateLocal(float x, float y, float z);
	void CamscaleLocal(float x, float y, float z);
	void CamrotateLocal(float x, float y, float z);
	void SetViewTransformation();
	void SetViewPort(float width, float height);
	glm::mat4x4 getViewPort();
	glm::mat4x4 GetProjectionTransformation();
	float top=0, bottom = 0;



	glm::mat4 CamlocalScaleMat = glm::mat4(1.0f);
	glm::mat4 CamlocalTranslateMat = glm::mat4(1.0f);
	glm::mat4 CamlocalRotationMat = glm::mat4(1.0f);
	glm::mat4 lookAt = glm::mat4(1.0f);
	glm::mat4 CamworldScaleMat = glm::mat4(1.0f);
	glm::mat4 CamworldTranslateMat = glm::mat4(1.0f);
	glm::mat4 CamworldRotationMat = glm::mat4(1.0f);
	glm::mat4 Camworld = glm::mat4(1.0f);
	glm::mat4 Camlocal = glm::mat4(1.0f);
	glm::mat4x4 view_transformation = glm::mat4(1.0f);
	glm::mat4x4 view_port = glm::mat4(1.0f);

	glm::mat4x4 projection_transformation = glm::mat4(1.0f);


private:

};
