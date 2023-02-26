Computer Graphics Assignment #3
OpenGL renderer

1.

In this first lines of code, the function states that it needs one texture that goes by the name "gl_screen_tex" and one Vertex array in th name of "gl_screen_vtc",
and then right after we create the buffer.
the next step is creating two array, "vtc" and "tex".
they will define the coordinates of the screen model that we want to map, and the coordinates of the texture that we are going to map into.
then we dynamically allocate memory on the GPU for the new buffer and configure the data of the arrays we defined.
We initialize the shaders and bind them to the current program, we send the screen coordinates to vPosition and send the mapping coordinates to vTexCoord.
and finally the Vertex shader sends screen coordinates to fragment shader that uses vTexCoord to map the texture to the screen using textureLod().

3.

```
#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

// The model/view/projection matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 viewPort;

// These outputs will be available in the fragment shader as inputs
out vec3 orig_fragPos;
out vec3 fragPos;
out vec3 fragNormal;
out vec2 fragTexCoords;


void main()
{

	// Apply the model transformation to the 'position' and 'normal' properties of the vertex,
	// so the interpolated values of these properties will be available for usi n the fragment shader
	orig_fragPos = vec3(vec4(pos, 1.0f));
	vec4 helper = model * vec4(pos, 1.0f);
	fragPos = vec3(helper/helper.w);
	helper = model*vec4(normal,1.0f);
	fragNormal = vec3(helper/helper.w);

	// Pass the vertex texture coordinates property as it is. Its interpolated value
	// will be avilable for us in the fragment shader
	fragTexCoords = texCoords;

	// This is an internal OpenGL variable, we must set a value to this variable
	helper = viewPort * projection * view *  model * vec4(pos, 1.0f);
	gl_Position = helper/helper.w;

}
```

4. A fragment shader that only outputs a constant color (Black)

```
#version 330 core

struct Material
{
	sampler2D textureMap;
	// You can add more fields here...
	// Such as:
	//		1. diffuse/specular relections constants
	//		2. specular alpha constant
	//		3. Anything else you find relevant
};

// We set this field's properties from the C++ code
uniform Material material;

// Inputs from vertex shader (after interpolation was applied)
in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragTexCoords;
in vec3 orig_fragPos;
// The final color of the fragment (pixel)
out vec4 frag_color;

void main()
{
	frag_color = vec4(0,0,0,0);
}
```

5.

```
void Renderer::Render(Scene& scene)
{
	if (scene.GetModelCount() == 0)
	{
		return;
	}

	MeshModel& model = scene.GetModel(0);
	glm::mat4x4 modelTranformation = model.getModelTransformation();
	glm::mat4x4 view = scene.getActiveCamera().getViewTransformation();
	glm::mat4 viewport = scene.getActiveCamera().getViewPort();
	glm::mat4 proj = scene.getActiveCamera().getProjectionTransformation();


	/*if (true)
	{
		if (print)
		{
			int i = 0;
			for (Vertex vertex : model.GetVertices())
			{
				cout << "vertex" << i++ << ": " << "(" << vertex.position.x << "," << vertex.position.y << "," << vertex.position.z << ") " << endl;;
			}
		}

		print = false;
	}*/


	// Activate the 'colorShader' program (vertex and fragment shaders)
	colorShader.use();

	// Set the uniform variables
	colorShader.setUniform("model", modelTranformation);
	colorShader.setUniform("view", view);
	colorShader.setUniform("projection", proj);
	colorShader.setUniform("viewPort", viewport);
	//colorShader.setUniform("material.textureMap", 0);

	// Set 'texture1' as the active texture at slot #0
	texture1.bind(0);

	// Drag our model's faces (triangles) in fill mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindVertexArray(model.GetVAO());
	glDrawArrays(GL_TRIANGLES, 0, model.vertexim.size());
	glBindVertexArray(0);

	// Unset 'texture1' as the active texture at slot #0
	texture1.unbind(0);

	colorShader.setUniform("color", glm::vec3(0,0,0));

	// Drag our model's faces (triangles) in line mode (wireframe)
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glBindVertexArray(model.GetVAO());
	//glDrawArrays(GL_TRIANGLES, 0, model.GetVertices().size());
	//glBindVertexArray(0);
}
```






