#pragma once
#include "Scene.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "../../build/ShaderProgram.h"
#include "../../build/Texture2D.h"

class Renderer
{
public:
	void LoadShaders();
	void LoadTextures();
	void clearZBuffer();
	float* z_buffer;
	void Renderer::fillTriangles(glm::vec4 v1, glm::vec4 v2, glm::vec4 v3,glm::vec3 camPos,MeshModel& model,Scene& scene, Face& face, int faceIndex);
	glm::vec3 Renderer::getFaceNormal(const Scene& scene, Face& face, int faceIndex);
	bool** triangle_buffer;
	bool help = true;
	glm::vec3 colors[10];
	Renderer(int viewportWidth, int viewportHeight);
	virtual ~Renderer();
	void Render(Scene& scene);
	void SwapBuffers();
	void ClearColorBuffer(const glm::vec3& color);
	int GetViewportWidth() const;
	int GetViewportHeight() const;
	int flag = 0;
	void Renderer::drawAxis(Scene& scene);
	void Renderer::drawBoundingBox(Scene& scene);
	glm::vec3 Renderer::drawFaceNormals(Scene& scene,Face& face, int faceIndex);
	void Renderer::drawVerticesNormals(Scene& scene, Face& face, int faceIndex);
	void Renderer::drawRec(glm::vec4 v1, glm::vec4 v2, glm::vec4 v3, float minZ, float maxZ,float triangleMinZ);
	void Renderer::drawLightVectors(Face face, Scene& scene);
	float Renderer::CalcZ(int i, int j, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);

	//ShaderProgram lightShader;
	//ShaderProgram colorShader;
	//Texture2D texture1;


private:
	void PutPixel(const int i, const int j, const glm::vec3& color);
	void DrawLine(const glm::ivec2& p1, const glm::ivec2& p2, const glm::vec3& color);

	void CreateBuffers(int w, int h);
	void CreateOpenglBuffer();
	void InitOpenglRendering();

	float* color_buffer;
	int viewport_width;
	int viewport_height;
	GLuint gl_screen_tex;
	GLuint gl_screen_vtc;
};
