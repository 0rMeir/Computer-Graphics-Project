#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>
#include <iostream>

#include "Renderer.h"
#include "InitShader.h"
#include <glm/gtx/transform.hpp>
#include <glm/gtx/normal.hpp>

#define INDEX(width,x,y,c) ((x)+(y)*(width))*3+(c)
#define Z_INDEX(width,x,y) ((x)+(y)*(width))
bool print = true;
bool draw = false;
bool flatShading = true;
bool phong = false;
bool reflectVector = false;

bool** boolArr;
	

Renderer::Renderer(int viewport_width, int viewport_height) :
	viewport_width(viewport_width),
	viewport_height(viewport_height)
{
	InitOpenglRendering();
	CreateBuffers(viewport_width, viewport_height);

	colors[0] = glm::vec3(255, 127, 80);
	colors[1] = glm::vec3(255, 0, 0);
	colors[2] = glm::vec3(0, 255, 0);
	colors[3] = glm::vec3(0, 0, 255);
	colors[4] = glm::vec3(255, 255, 0);
	colors[5] = glm::vec3(0, 255, 255);
	colors[6] = glm::vec3(255, 0, 255);
	colors[7] = glm::vec3(128, 0, 0);
	colors[8] = glm::vec3(128, 128, 0);
	colors[9] = glm::vec3(0, 128, 0);

}

Renderer::~Renderer()
{
	delete[] color_buffer;
	for (int i = 0; i < viewport_height; i++)
	{
		delete[] triangle_buffer[i];
	}
	delete[] triangle_buffer;
}

void Renderer::PutPixel(int i, int j, const glm::vec3& color)
{
	if (i < 0) return; if (i >= viewport_width) return;
	if (j < 0) return; if (j >= viewport_height) return;
	
	color_buffer[INDEX(viewport_width, i, j, 0)] = color.x;
	color_buffer[INDEX(viewport_width, i, j, 1)] = color.y;
	color_buffer[INDEX(viewport_width, i, j, 2)] = color.z;
}

void Renderer::DrawLine(const glm::ivec2& p1, const glm::ivec2& p2, const glm::vec3& color)
{
	
	glm::ivec2 start = glm::ivec2(p1);
	glm::ivec2 end = glm::ivec2(p2);
	if (p1.x > p2.x)
	{
		start = glm::ivec2(p2);
		end = glm::ivec2(p1);
	}
	int transform1 = 1, transform2 = 1;
	int* coordinate1 = &start.y;
	int* coordinate2 = &start.x;
	int firstDelta = (end.x - start.x);
	int secondDelta = (end.y - start.y);

	double m = ((double)secondDelta / (double)firstDelta);

	int theEnd = end.x;
	int* theStart = &start.x;
	int firstDeltaChange = 1;
	int secondDeltaChange = 1;
	int e = 0;

	if(m < -1 || m > 1)
	{
		coordinate1 = &start.x;
		coordinate2 = &start.y;
		theEnd = end.y;
		theStart = &start.y;
		swap(firstDelta, secondDelta);

		if (m < -1)
		{
			transform2 = -1;
			firstDeltaChange = -1;
		}
	}
	if (m<=0 && m>=-1)
	{
		transform1 = -1;
		secondDeltaChange = -1;
	}

	while (*theStart != theEnd)
	{
		if (e > 0)
		{
			*coordinate1 += transform1;
			e -= 2 * (firstDelta)*firstDeltaChange;
		}
		PutPixel(start.x, start.y, color);
		*coordinate2 += transform2;	
		e += 2*(secondDelta)*secondDeltaChange;

	}

}

void Renderer::CreateBuffers(int w, int h)
{
	CreateOpenglBuffer(); //Do not remove this line.
	color_buffer = new float[3 * w * h];
	ClearColorBuffer(glm::vec3(0.0f, 0.0f, 0.0f));
	z_buffer = new float[w * h];
}

void Renderer::clearZBuffer()
{
	for (int i = 0; i < viewport_width; i++)
	{
		for (int j = 0; j < viewport_height; j++)
		{
			z_buffer[Z_INDEX(viewport_width, i, j)] = FLT_MAX;
		}
	}
}

//##############################
//##OpenGL stuff. Don't touch.##
//##############################

// Basic tutorial on how opengl works:
// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/
// don't linger here for now, we will have a few tutorials about opengl later.
void Renderer::InitOpenglRendering()
{
	// Creates a unique identifier for an opengl texture.
	glGenTextures(1, &gl_screen_tex);

	// Same for vertex array object (VAO). VAO is a set of buffers that describe a renderable object.
	glGenVertexArrays(1, &gl_screen_vtc);

	GLuint buffer;

	// Makes this VAO the current one.
	glBindVertexArray(gl_screen_vtc);

	// Creates a unique identifier for a buffer.
	glGenBuffers(1, &buffer);

	// (-1, 1)____(1, 1)
	//	     |\  |
	//	     | \ | <--- The exture is drawn over two triangles that stretch over the screen.
	//	     |__\|
	// (-1,-1)    (1,-1)
	const GLfloat vtc[]={
		-1, -1,
		 1, -1,
		-1,  1,
		-1,  1,
		 1, -1,
		 1,  1
	};

	const GLfloat tex[]={
		0,0,
		1,0,
		0,1,
		0,1,
		1,0,
		1,1};

	// Makes this buffer the current one.
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	// This is the opengl way for doing malloc on the gpu. 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vtc)+sizeof(tex), NULL, GL_STATIC_DRAW);

	// memcopy vtc to buffer[0,sizeof(vtc)-1]
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vtc), vtc);

	// memcopy tex to buffer[sizeof(vtc),sizeof(vtc)+sizeof(tex)]
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vtc), sizeof(tex), tex);

	// Loads and compiles a sheder.
	GLuint program = InitShader( "vshader.glsl", "fshader.glsl" );

	// Make this program the current one.
	glUseProgram(program);

	// Tells the shader where to look for the vertex position data, and the data dimensions.
	GLint  vPosition = glGetAttribLocation( program, "vPosition" );
	glEnableVertexAttribArray( vPosition );
	glVertexAttribPointer( vPosition,2,GL_FLOAT,GL_FALSE,0,0 );

	// Same for texture coordinates data.
	GLint  vTexCoord = glGetAttribLocation( program, "vTexCoord" );
	glEnableVertexAttribArray( vTexCoord );
	glVertexAttribPointer( vTexCoord,2,GL_FLOAT,GL_FALSE,0,(GLvoid *)sizeof(vtc) );

	//glProgramUniform1i( program, glGetUniformLocation(program, "texture"), 0 );

	// Tells the shader to use GL_TEXTURE0 as the texture id.
	glUniform1i(glGetUniformLocation(program, "texture"),0);
}

void Renderer::CreateOpenglBuffer()
{
	// Makes GL_TEXTURE0 the current active texture unit
	glActiveTexture(GL_TEXTURE0);

	// Makes glScreenTex (which was allocated earlier) the current texture.
	glBindTexture(GL_TEXTURE_2D, gl_screen_tex);

	// malloc for a texture on the gpu.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, viewport_width, viewport_height, 0, GL_RGB, GL_FLOAT, NULL);
	glViewport(0, 0, viewport_width, viewport_height);
}

void Renderer::SwapBuffers()
{
	// Makes GL_TEXTURE0 the current active texture unit
	glActiveTexture(GL_TEXTURE0);

	// Makes glScreenTex (which was allocated earlier) the current texture.
	glBindTexture(GL_TEXTURE_2D, gl_screen_tex);

	// memcopy's colorBuffer into the gpu.
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, viewport_width, viewport_height, GL_RGB, GL_FLOAT, color_buffer);

	// Tells opengl to use mipmapping
	glGenerateMipmap(GL_TEXTURE_2D);

	// Make glScreenVtc current VAO
	glBindVertexArray(gl_screen_vtc);

	// Finally renders the data.
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::ClearColorBuffer(const glm::vec3& color)
{
	for (int i = 0; i < viewport_width; i++)
	{
		for (int j = 0; j < viewport_height; j++)
		{
			PutPixel(i, j, color);
		}
	}
}


void Renderer::drawAxis(Scene& scene)
{
	MeshModel model = scene.GetModel(0);

	glm::mat4x4 view = scene.getActiveCamera().getViewTransformation();
	glm::mat4 viewport = scene.getActiveCamera().view_port;
	glm::mat4 proj = scene.getActiveCamera().projection_transformation;

	//World Axis

	glm::ivec4 start = glm::ivec4(0, 0, 0, 1);
	glm::ivec4 y = glm::ivec4(0, 1, 0, 1);
	glm::ivec4 x = glm::ivec4(1, 0, 0, 1);
	glm::ivec4 z = glm::ivec4(0, 0, 1, 1);

	start = viewport * proj * view *  start;
	y = viewport * proj * view *  y;
	x = viewport * proj * view *  x;
	z = viewport * proj * view *  z;

	start /= start.w;
	y /= y.w;
	x /= x.w;
	z /= z.w;

	DrawLine(start, y, glm::ivec3(1, 0, 0));
	DrawLine(start, x, glm::ivec3(1, 0, 0));
	DrawLine(start, z, glm::ivec3(1, 0, 0));

	glm::ivec4 startM = glm::ivec4(0 , 0 , 0, 1);
	glm::ivec4 yM = glm::ivec4(0 , 1, 0, 1);
	glm::ivec4 xM = glm::ivec4(1, 0 , 0, 1);
	glm::ivec4 zM = glm::ivec4(0, 0, 1, 1);

	startM = viewport * proj * view * model.getWorldTransform() * startM;
	yM = viewport * proj * view * model.getWorldTransform() * yM;
	xM = viewport * proj * view * model.getWorldTransform() * xM;
	zM = viewport * proj * view * model.getWorldTransform() * zM;

	startM /= startM.w;
	yM /= yM.w;
	xM /= xM.w;
	zM /= zM.w;

	DrawLine(startM, yM, glm::ivec3(0, 0, 1));
	DrawLine(startM, xM, glm::ivec3(0, 0, 1));
	DrawLine(startM, zM, glm::ivec3(0, 0, 1));
}


void Renderer::drawBoundingBox(Scene& scene)
{
	MeshModel model = scene.GetActiveModel();
	glm::mat4x4 view = scene.getActiveCamera().getViewTransformation();
	glm::mat4 viewport = scene.getActiveCamera().view_port;
	glm::mat4 proj = scene.getActiveCamera().projection_transformation;

	glm::vec4 rdf = glm::vec4(model.maxX, model.minY, model.maxZ, 1);
	glm::vec4 ruf = glm::vec4(model.maxX, model.maxY, model.maxZ, 1);
	glm::vec4 ldf = glm::vec4(model.minX, model.minY, model.maxZ, 1);
	glm::vec4 luf = glm::vec4(model.minX, model.maxY, model.maxZ, 1);
	glm::vec4 rdb = glm::vec4(model.maxX, model.minY, model.minZ, 1);
	glm::vec4 rub = glm::vec4(model.maxX, model.maxY, model.minZ, 1);
	glm::vec4 ldb = glm::vec4(model.minX, model.minY, model.minZ, 1);
	glm::vec4 lub = glm::vec4(model.minX, model.maxY, model.minZ, 1);

	rdf = viewport * proj * view * model.transform(rdf); rdf /= rdf.w;
	ruf = viewport * proj * view * model.transform(ruf); ruf /= ruf.w;
	ldf = viewport * proj * view * model.transform(ldf); ldf /= ldf.w;
	luf = viewport * proj * view * model.transform(luf); luf /= luf.w;
	rdb = viewport * proj * view * model.transform(rdb); rdb /= rdb.w;
	rub = viewport * proj * view * model.transform(rub); rub /= rub.w;
	ldb = viewport * proj * view * model.transform(ldb); ldb /= ldb.w;
	lub = viewport * proj * view * model.transform(lub); lub /= lub.w;

	DrawLine(rdf, ldf, glm::ivec3(0, 0, 1));
	DrawLine(rdf, rdb, glm::ivec3(0, 0, 1));
	DrawLine(ldf, ldb, glm::ivec3(0, 0, 1));
	DrawLine(rdb, ldb, glm::ivec3(0, 0, 1));

	DrawLine(rdf, ruf, glm::ivec3(0, 0, 1));
	DrawLine(ldf, luf, glm::ivec3(0, 0, 1));
	DrawLine(ldb, lub, glm::ivec3(0, 0, 1));
	DrawLine(rdb, rub, glm::ivec3(0, 0, 1));

	DrawLine(ruf, luf, glm::ivec3(0, 0, 1));
	DrawLine(ruf, rub, glm::ivec3(0, 0, 1));
	DrawLine(luf, lub, glm::ivec3(0, 0, 1));
	DrawLine(rub, lub, glm::ivec3(0, 0, 1));
}


glm::vec3 Renderer::drawFaceNormals(Scene& scene,Face& face,int faceIndex)
{
	MeshModel model = scene.GetActiveModel();
	glm::mat4x4 view = scene.getActiveCamera().getViewTransformation();
	glm::mat4 viewport = scene.getActiveCamera().view_port;
	glm::mat4 proj = scene.getActiveCamera().projection_transformation;

	glm::vec4 faceNorm = glm::vec4(0.0f);
	glm::vec4 vec = glm::vec4(0.0f);

	for (int i = 0; i < 3; i++)
	{
		glm::vec4 normal = glm::vec4(model.getVertexNormal(face.GetNormalIndex(i) - 1), 1);
		glm::vec4 vertex = glm::vec4(model.GetVertex(face.GetVertexIndex(i)-1),1);

		vec += vertex;
		faceNorm += normal;

		normal = vertex + (0.01f * normal);
		normal.w = 1;
		normal = viewport * proj * view * model.transform(normal); normal /= normal.w;
		vertex = viewport * proj * view * model.transform(vertex); vertex /= vertex.w;
		
		if (draw)
		{
			DrawLine(vertex, normal, glm::ivec3(0, 0, 1));
		}	
	}

	vec /= 3.0f;
	faceNorm /= 3.0f;

	faceNorm = vec + 0.01f * faceNorm; faceNorm.w = 1;
	faceNorm = viewport * proj * view * model.transform(faceNorm);  faceNorm/= faceNorm.w;
	vec = viewport * proj * view * model.transform(vec);  vec /= vec.w;

	if (draw)
	{
		DrawLine(vec, faceNorm, glm::ivec3(0, 0, 1));
	}

	return (glm::normalize(faceNorm));
}


glm::vec3 Renderer::getFaceNormal(const Scene& scene, Face& face, int faceIndex)
{
	MeshModel model = scene.GetActiveModel();
	glm::mat4x4 view = scene.getActiveCamera().getViewTransformation();
	glm::mat4 viewport = scene.getActiveCamera().view_port;
	glm::mat4 proj = scene.getActiveCamera().projection_transformation;
	glm::vec4 normal = glm::vec4(0.0f); 
	for (int i = 0; i < 3; i++)
	{
		normal+= glm::vec4(model.getVertexNormal(face.GetNormalIndex(i) - 1), 1);
	}
	normal = viewport * proj * view * model.transform(normal);
	normal /= normal.w;
	return (normal/3.0f);
}


void Renderer::drawRec(glm::vec4 v1, glm::vec4 v2, glm::vec4 v3, float minZ, float maxZ, float triangleAverageZ)
{
	float minX = min(v1.x,min(v2.x, v3.x));
	float minY = min(v1.y, min(v2.y, v3.y));
	float maxX = max(v1.x, max(v2.x, v3.x));
	float maxY = max(v1.y, max(v2.y, v3.y));

	int colorIndex = ((int)((v1.z + v2.z + v3.z) / 3)) % 10;

	glm::vec3 color = colors[colorIndex];

	DrawLine(glm::ivec2(minX, minY), glm::ivec2(minX, maxY), color);
	DrawLine(glm::ivec2(minX, minY), glm::ivec2(maxX, minY), color);
	DrawLine(glm::ivec2(maxX, minY), glm::ivec2(maxX, maxY), color);
	DrawLine(glm::ivec2(maxX, maxY), glm::ivec2(minX, maxY), color);
}

float sign(glm::vec4 v1, glm::vec4 v2, glm::vec4 v3)
{
	return (v1.x - v3.x) * (v2.y - v3.y) - (v2.x - v3.x) * (v1.y - v3.y);
}

bool PointInTriangle(glm::vec4 pt, glm::vec4 v1, glm::vec4 v2, glm::vec4 v3)
{
	float d1, d2, d3;
	bool has_neg, has_pos;

	d1 = sign(pt, v1, v2);
	d2 = sign(pt, v2, v3);
	d3 = sign(pt, v3, v1);

	has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
	has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

	return !(has_neg && has_pos);
}


float triangleSurfaceArea(const glm::vec3 p1, const glm::vec3 p2, const glm::vec3 p3)
{
	glm::vec3 p1_3(p1.x, p1.y, p1.z);
	glm::vec3 p2_3(p2.x, p2.y, p2.z);
	glm::vec3 p3_3(p3.x, p3.y, p3.z);
	glm::vec3 v1 = p2_3 - p1_3;
	glm::vec3 v2 = p3_3 - p1_3;
	glm::vec3 normal = glm::cross(v1, v2);
	return (0.5f * glm::length(normal) * glm::length(v1));
}

float triangleSurface(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3) {
	return glm::abs(0.5f * ((p2.x - p1.x) * (p3.y - p1.y) - (p3.x - p1.x) * (p2.y - p1.y)));
}

glm::vec3 interpolateNormals(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4, glm::vec3 normal1,glm::vec3 normal2,glm::vec3 normal3)
{
	float surface1 = triangleSurface(glm::vec2(p1), glm::vec2 (p2), glm::vec2(p4));
	float surface2 = triangleSurface(glm::vec2(p2), glm::vec2(p3), glm::vec2(p4));
	float surface3 = triangleSurface(glm::vec2(p1), glm::vec2(p3), glm::vec2(p4));
	float bigTriangle = surface1+surface2+surface3;
	return (((surface1/bigTriangle) * normal1) + ((surface2 / bigTriangle) * normal2) + ((surface3 / bigTriangle) * normal3));
}

glm::vec3 computeFaceNormal(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c)
{
	return glm::normalize(glm::cross(b - a, c - a));
}



float Renderer::CalcZ(int i, int j, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
{
	float t1 = triangleSurface(glm::vec2(i, j), p2, p3);
	float t2 = triangleSurface(glm::vec2(i, j), p1, p3);
	float t3 = triangleSurface(glm::vec2(i, j), p1, p2);
	float TrianglArea = t1 + t2 + t3;
	return (((t1 / TrianglArea) * p1.z) + ((t2 / TrianglArea) * p2.z) + ((t3 / TrianglArea) * p3.z));
}




void Renderer::fillTriangles(glm::vec4 v1, glm::vec4 v2, glm::vec4 v3,glm::vec3 camPos,MeshModel& model,Scene& scene, Face& face, int faceIndex)
{
	/*if (print)
	{
		cout << endl << endl << "(" << camPos.x << "," << camPos.y << "," << camPos.z << ")" << endl;
		print = false;
	}*/

	glm::vec3 normal1, normal2, normal3;
	glm::vec3 theColor;
	Light& light = scene.sceneLight;
	glm::vec3 faceNorm = computeFaceNormal(v1, v2, v3);
	glm::vec3 middleFace = (glm::vec3((v1 + v2 + v3) / 3.0f));

	if (phong)
	{
		MeshModel model = scene.GetActiveModel();
		glm::mat4x4 view = scene.getActiveCamera().getViewTransformation();
		glm::mat4 viewport = scene.getActiveCamera().view_port;
		glm::mat4 proj = scene.getActiveCamera().projection_transformation;
		glm::vec3 normal1a = viewport * proj * view * model.transform(glm::vec4(model.getVertexNormal(face.GetNormalIndex(0) - 1), 1.0f));
		glm::vec3 normal2a = viewport * proj * view * model.transform(glm::vec4(model.getVertexNormal(face.GetNormalIndex(1) - 1), 1.0f));
		glm::vec3 normal3a = viewport * proj * view * model.transform(glm::vec4(model.getVertexNormal(face.GetNormalIndex(2) - 1), 1.0f));	

	}
	else
	{
		theColor = light.computeColor(model, ((v1 + v2 + v3) / 3.0f), faceNorm, camPos);
	}

	float minX = min(v1.x, min(v2.x, v3.x));
	float minY = min(v1.y, min(v2.y, v3.y));
	float maxX = max(v1.x, max(v2.x, v3.x));
	float maxY = max(v1.y, max(v2.y, v3.y));

	for (int j = minY; j <= maxY; j++)
	{
		int i = minX;
		bool end = false;
		for (; i < maxX && !PointInTriangle(glm::vec4(i, j, 1, 1), v1, v2, v3); i++){}
		

		for (; i < maxX && PointInTriangle(glm::vec4(i, j, 1, 1), v1, v2, v3); i++)
		{
			if ((viewport_width * j + i) < (viewport_width * viewport_height) && (viewport_width * j + i) > 0)
			{
				float z = CalcZ(i,j,v1,v2,v3);

				if (z < (z_buffer[viewport_width * j + i]))
				{
					if (flatShading)
					{
						z_buffer[viewport_width * j + i] = z;
						PutPixel(i, j, theColor);
					}
					
					else
					{
					    z_buffer[viewport_width * j + i] = z;
						glm::vec3 phongNormal = interpolateNormals(v1, v2, v3, glm::vec3(i, j, z), glm::normalize(normal1), glm::normalize(normal2), glm::normalize(normal3));
						glm::vec3 theColor = light.computeColor(model, glm::vec3(i,j,z), phongNormal, camPos);
						PutPixel(i, j, theColor);
					}
				}
			}
		}
	}

	if (true)
	{
		glm::vec3 camDir = glm::normalize(light.pos - middleFace);
		glm::vec3 reflectionDirection = glm::reflect(camDir, faceNorm);

		DrawLine(middleFace + 50.f * reflectionDirection, middleFace, glm::vec3(1, 1, 0));
		DrawLine(middleFace + 50.f * reflectionDirection, middleFace + 90.f * reflectionDirection, glm::vec3(1, 0, 0));
	}
}



void Renderer::Render(Scene& scene)
{
	// TODO: Replace this code with real scene rendering code

	int half_width = viewport_width / 2;
	int half_height = viewport_height / 2;
		;
	if (scene.GetModelCount() > 0)
	{
		if (scene.showAxis)
		{
			drawAxis(scene);
		}

		if (scene.showBoundingBox)
		{
			drawBoundingBox(scene);
		}
		

		MeshModel model = scene.GetModel(0);
		float minZ = model.minZ;
		float maxZ = model.maxZ;
	

		for (int i = 0; i < model.GetFacesCount(); i++)
		{
			Face face = model.GetFace(i);
			glm::vec4 v1 = glm::vec4(model.GetVertex(face.GetVertexIndex(0) - 1),1);
			glm::vec4 v2 = glm::vec4( model.GetVertex(face.GetVertexIndex(1) - 1),1);
			glm::vec4 v3 = glm::vec4(model.GetVertex(face.GetVertexIndex(2) - 1),1);
			glm::mat4x4 view = scene.getActiveCamera().getViewTransformation();
			glm::mat4 viewport = scene.getActiveCamera().view_port;
			glm::mat4 proj = scene.getActiveCamera().projection_transformation;

			v1 = viewport * proj * view * (model.transform(v1));
			v2 = viewport * proj * view * (model.transform(v2));
			v3 = viewport * proj * view * (model.transform(v3));

			v1 /= v1.w;
			v2 /= v2.w;
			v3 /= v3.w;


			/*if (draw)
			{
				glm::vec3 lightDir = glm::normalize(scene.sceneLight.pos - glm::vec3(((v1 + v2 + v3) / 3.0f)));
				glm::vec3 reflect = glm::normalize(glm::reflect(-lightDir, thisNormal));
				glm::vec3 newReflect = glm::vec3((v1 + v2 + v3) / 3.0f) + 20.0f * reflect;
				DrawLine(glm::ivec2(((v1 + v2 + v3) / 3.0f)), glm::ivec2(newReflect), glm::vec3(0, 0, 255));
				glm::vec3 newLightDir = glm::vec3((v1 + v2 + v3) / 3.0f) + 20.0f * lightDir;
				DrawLine(glm::ivec2(((v1 + v2 + v3) / 3.0f)), glm::ivec2(newLightDir), glm::vec3(255, 0,0));

			}*/

		
			if (scene.wireFrame)
			{
				DrawLine(glm::ivec2(v1.x, v1.y), glm::ivec2(v2.x, v2.y), glm::ivec3(0, 0, 0));
				DrawLine(glm::ivec2(v2.x, v2.y), glm::ivec2(v3.x, v3.y), glm::ivec3(0, 0, 0));
				DrawLine(glm::ivec2(v3.x, v3.y), glm::ivec2(v1.x, v1.y), glm::ivec3(0, 0, 0));
			}
			else
			{
				//glm::mat4 cameraInverse = glm::inverse(view);
				//glm::vec3 cameraPosition = glm::vec3(glm::inverse(view)[3]);

				fillTriangles(v1, v2, v3, (glm::vec3(glm::inverse(view)[3])),model,scene,face, i);
			}


			/*if (scene.showNormals)
			{
				drawFaceNormals(scene, face, i);
			}

			if (scene.showRectangles)
			{
				if (scene.wireFrame)
				{
					drawRec(v1, v2, v3, minZ, maxZ, triangleAverageZ);
				}
			}*/
		}
	}
}

int Renderer::GetViewportWidth() const
{
	return viewport_width;
}

int Renderer::GetViewportHeight() const
{
	return viewport_height;
}