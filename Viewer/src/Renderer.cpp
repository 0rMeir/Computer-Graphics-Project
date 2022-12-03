#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>
#include <iostream>

#include "Renderer.h"
#include "InitShader.h"
#include <glm/gtx/transform.hpp>

#define INDEX(width,x,y,c) ((x)+(y)*(width))*3+(c)
#define Z_INDEX(width,x,y) ((x)+(y)*(width))

Renderer::Renderer(int viewport_width, int viewport_height) :
	viewport_width(viewport_width),
	viewport_height(viewport_height)
{
	InitOpenglRendering();
	CreateBuffers(viewport_width, viewport_height);
}

Renderer::~Renderer()
{
	delete[] color_buffer;
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

void Renderer::Render(const Scene& scene)
{
	// TODO: Replace this code with real scene rendering code
	int half_width = viewport_width / 2;
	int half_height = viewport_height / 2;
	if (scene.GetModelCount() > 0)
	{
		MeshModel model = scene.GetModel(0);
		for (int i = 0; i < model.GetFacesCount(); i++)
		{
			Face face = model.GetFace(i);

			glm::vec4 v1 = glm::vec4(model.GetVertex(face.GetVertexIndex(0) - 1),1);
			glm::vec4 v2 = glm::vec4( model.GetVertex(face.GetVertexIndex(1) - 1),1);
			glm::vec4 v3 = glm::vec4(model.GetVertex(face.GetVertexIndex(2) - 1),1);

			v1 = model.localTransformMat * v1;
			v2 = model.localTransformMat * v2;
			v3 = model.localTransformMat * v3;

			v1 /= v1.w;
			v2 /= v2.w;
			v3 /= v3.w;

			DrawLine(glm::ivec2(v1.x + half_width, v1.y + half_height), glm::ivec2(v2.x + half_width, v2.y + half_height), glm::ivec3(0, 0, 0));
			DrawLine(glm::ivec2(v2.x + half_width, v2.y + half_height), glm::ivec2(v3.x + half_width, v3.y + half_height), glm::ivec3(0, 0, 0));
			DrawLine(glm::ivec2(v3.x + half_width, v3.y + half_height), glm::ivec2(v1.x + half_width, v1.y + half_height), glm::ivec3(0, 0, 0));

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