#define _USE_MATH_DEFINES
#include <cmath>
#include <imgui/imgui.h>
#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <nfd.h>

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <iostream>
#include "Renderer.h"
#include "Scene.h"
#include "Utils.h"

/**
 * Fields
 */
bool show_demo_window = false;
bool show_another_window = false;
glm::vec4 clear_color = glm::vec4(0.8f, 0.8f, 0.8f, 1.00f);

float scale=1.0f;
glm::vec3 camTranslate = glm::vec3(0.0f);
bool ortho = false;
bool persp = false;
bool showAxis = false;
bool changeScreenSize = false;
bool drawRec = false;
static int windowWidth = 1900, windowHeight = 1200;


/**
 * Function declarations
 */
static void GlfwErrorCallback(int error, const char* description);
GLFWwindow* SetupGlfwWindow(int w, int h, const char* window_name);
ImGuiIO& SetupDearImgui(GLFWwindow* window);
void StartFrame();
void RenderFrame(GLFWwindow* window, Scene& scene, Renderer& renderer, ImGuiIO& io);
void Cleanup(GLFWwindow* window);
void DrawImguiMenus(ImGuiIO& io, Scene& scene);

/**
 * Function implementation
 */
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
	// TODO: Handle mouse scroll here
}

int main(int argc, char **argv)
{
	GLFWwindow* window = SetupGlfwWindow(windowWidth, windowHeight, "Mesh Viewer");
	if (!window)
		return 1;

	int frameBufferWidth, frameBufferHeight;
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);

	Renderer renderer = Renderer(frameBufferWidth, frameBufferHeight);
	Scene scene = Scene(windowWidth, windowHeight);
	
	ImGuiIO& io = SetupDearImgui(window);
	glfwSetScrollCallback(window, ScrollCallback);
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
		StartFrame();
		DrawImguiMenus(io, scene);
		RenderFrame(window, scene, renderer, io);
    }

	Cleanup(window);
    return 0;
}

static void GlfwErrorCallback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

GLFWwindow* SetupGlfwWindow(int w, int h, const char* window_name)
{
	glfwSetErrorCallback(GlfwErrorCallback);
	if (!glfwInit())
		return NULL;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	#if __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif
	
	GLFWwindow* window = glfwCreateWindow(w, h, window_name, NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync
						 // very importent!! initialization of glad
						 // https://stackoverflow.com/questions/48582444/imgui-with-the-glad-opengl-loader-throws-segmentation-fault-core-dumped

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	return window;
}


ImGuiIO& SetupDearImgui(GLFWwindow* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();
	ImGui::StyleColorsDark();
	return io;
}

void StartFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void RenderFrame(GLFWwindow* window, Scene& scene, Renderer& renderer, ImGuiIO& io)
{
	ImGui::Render();
	int frameBufferWidth, frameBufferHeight;
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
	bool update = false;
	
	if (frameBufferWidth != renderer.GetViewportWidth() || frameBufferHeight != renderer.GetViewportHeight())
	{
		scene.width = frameBufferWidth;
		scene.height = frameBufferHeight;
		scene.getActiveCamera().setViewPort(frameBufferWidth, frameBufferHeight);
	}

	float camSpeed = 0.01f;
	Camera& cam = (scene.getActiveCamera());

	if (!io.WantCaptureKeyboard)
	{ 
		if (ImGui::IsKeyPressed('W'))
		{
			
			cam.camTranslateWorld(camTranslate.x, 0.01f, camTranslate.z);		
		}
		if (ImGui::IsKeyPressed('S'))
		{
			cam.camTranslateWorld(camTranslate.x, -0.01f, camTranslate.z);
		}
		if (ImGui::IsKeyPressed('D'))
		{
			cam.camTranslateWorld(0.01f, camTranslate.y, camTranslate.z);
		}
		if (ImGui::IsKeyPressed('A'))
		{
			cam.camTranslateWorld(-0.01f, camTranslate.y, camTranslate.z);
		}
		if (ImGui::IsKeyPressed('O'))
		{
			cam.camScaleWorld(1.1f, 1.1f,1.1f);
		}
		if (ImGui::IsKeyPressed('I'))
		{
			cam.camScaleWorld(0.95f,0.95f, 0.95f);
		}
	}

	if (!io.WantCaptureMouse)
	{
		// TODO: Handle mouse events here
		if (io.MouseDown[0])
		{
			// Left mouse button is down
		}
	}

	renderer.ClearColorBuffer(clear_color);
	renderer.clearZBuffer();
	renderer.Render(scene);
	renderer.SwapBuffers();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glfwMakeContextCurrent(window);
	glfwSwapBuffers(window);

}

void Cleanup(GLFWwindow* window)
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwDestroyWindow(window);
	glfwTerminate();
}


void DrawImguiMenus(ImGuiIO& io, Scene& scene)
{
	/**
	 * MeshViewer menu
	 */
	ImGui::Begin("MeshViewer Menu");
	
	// Menu Bar
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open", "CTRL+O"))
			{
				nfdchar_t* outPath = NULL;
				nfdresult_t result = NFD_OpenDialog("obj;", NULL, &outPath);
				if (result == NFD_OKAY)
				{
					scene.AddModel(Utils::LoadMeshModel(outPath));
					free(outPath);
					
						if (scene.getModelsNumber() > 0) {
							MeshModel model = scene.GetModel(0);
							for (int i = 0; i < model.GetVerticesCount(); i++) {
								glm::vec3 temp = model.GetVertex(i);
								cout << "v " << temp.x << " " << temp.y << " " << temp.z << endl;
							}
							for (int i = 0; i < model.GetFacesCount(); i++) {
								int vertex0 = model.GetFace(i).GetVertexIndex(0);
								int vertex1 = model.GetFace(i).GetVertexIndex(1);
								int vertex2 = model.GetFace(i).GetVertexIndex(2);
								int vertexn0 = model.GetFace(i).GetNormalIndex(0);
								int vertexn1 = model.GetFace(i).GetNormalIndex(1);
								int vertexn2 = model.GetFace(i).GetNormalIndex(2);
								

								cout << "f " << vertex0 << "//" << vertexn0<< " " << vertex1 << "//" << vertexn1 <<" "<<vertex2<<"//"<<vertexn2 << endl;
							}
						}

					
				}
				else if (result == NFD_CANCEL)
				{}
				else
				{}
				
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Change Screen size"))
			{	
				changeScreenSize = true;			
			}

			ImGui::EndMenu();
		}

		// TODO: Add more menubar items (if you want to)
		ImGui::EndMainMenuBar();
	}

	// Controls
	ImGui::ColorEdit3("Clear Color", (float*)&clear_color);
	// TODO: Add more controls as needed
	
	ImGui::End();

	
	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("My GUI");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &show_another_window);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	// 3. Show another simple window.
	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}

	{
	ImGui::SetWindowSize(ImVec2(200,200), ImGuiCond(0));
	ImGui::Begin("Main Menu");
	ImGui::Text("Local/World Model Transformation");

	if(ImGui::CollapsingHeader("Translate Local"))
	{ 
		ImGui::Text("Translate");
		static float X_AxisL=0;
		static float Y_AxisL=0;
		static float Z_AxisL=0;
		ImGui::SetNextItemWidth(100);
		if (ImGui::SliderFloat("Local X-Axis", &X_AxisL, -10, 10))
		{
			scene.GetActiveModel().translateLocal(X_AxisL, Y_AxisL, Z_AxisL);
		}
		ImGui::SetNextItemWidth(100);
		if (ImGui::SliderFloat("LocalY-Axis", &Y_AxisL, -10, 10))
		{
			scene.GetActiveModel().translateLocal(X_AxisL, Y_AxisL, Z_AxisL);
		}
		ImGui::SetNextItemWidth(100);
		if (ImGui::SliderFloat("LocalZ-Axis", &Z_AxisL, -10, 10))
		{
			scene.GetActiveModel().translateLocal(X_AxisL, Y_AxisL, Z_AxisL);
		}
	}

	if(ImGui::CollapsingHeader("Scale Local"))
	{ 
		static float scaleFactorL=0;

		ImGui::SetNextItemWidth(100);
		if (ImGui::SliderFloat("Local Scale", &scaleFactorL, 1, 2))
		{
			scene.GetActiveModel().scaleLocal(scaleFactorL, scaleFactorL);
		}
	}

	if (ImGui::CollapsingHeader("Rotate Local"))
	{
		static float XangleL;
		static float YangleL;
		static float ZangleL;

		ImGui::SetNextItemWidth(100);
		if (ImGui::SliderFloat("Local X-Axis Rotation Angle", &XangleL, 0, 360))
		{
			scene.GetActiveModel().rotateLocalX(XangleL);
		}
		ImGui::SetNextItemWidth(100);
		if (ImGui::SliderFloat("Local Y-Axis Rotation Angle", &YangleL, 0, 360))
		{
			scene.GetActiveModel().rotateLocalY(YangleL);
		}
		ImGui::SetNextItemWidth(100);
		if (ImGui::SliderFloat("Local Z-Axis Rotation Angle", &ZangleL, 0, 360))
		{
			scene.GetActiveModel().rotateLocalZ(ZangleL);
		}
	}
	ImGui::Text("");
	ImGui::Text("World Transformations");

	if (ImGui::CollapsingHeader("Translate World"))
	{
		ImGui::Text("Translate");
		static float X_Axis = 0;
		static float Y_Axis = 0;
		static float Z_Axis = 0;
		ImGui::SetNextItemWidth(100);
		if (ImGui::SliderFloat("X-Axis", &X_Axis, -10, 10))
		{
			scene.GetActiveModel().translateWorld(X_Axis, Y_Axis, Z_Axis);
		}
		ImGui::SetNextItemWidth(100);
		if (ImGui::SliderFloat("Y-Axis", &Y_Axis, -10, 10))
		{
			scene.GetActiveModel().translateWorld(X_Axis, Y_Axis, Z_Axis);
		}
		ImGui::SetNextItemWidth(100);
		if (ImGui::SliderFloat("Z-Axis", &Z_Axis, -10, 10))
		{
			scene.GetActiveModel().translateWorld(X_Axis, Y_Axis, Z_Axis);
		}
	}

	if (ImGui::CollapsingHeader("Scale World"))
	{
		static float scaleFactor = 0;

		ImGui::SetNextItemWidth(100);
		if (ImGui::SliderFloat("Scale", &scaleFactor, 1, 2))
		{
			scene.GetActiveModel().scaleWorld(scaleFactor, scaleFactor);
		}
	}


	if (ImGui::CollapsingHeader("Rotate World"))
	{
		static float Xangle;
		static float Yangle;
		static float Zangle;

		ImGui::SetNextItemWidth(100);
		if (ImGui::SliderFloat("X-Axis Rotation Angle", &Xangle, 1, 360))
		{
			scene.GetActiveModel().rotateWorldlX(Xangle);
		}
		ImGui::SetNextItemWidth(100);
		if (ImGui::SliderFloat("Y-Axis Rotation Angle", &Yangle, 1, 360))
		{
			scene.GetActiveModel().rotateWorldY(Yangle);
		}
		ImGui::SetNextItemWidth(100);
		if (ImGui::SliderFloat("Z-Axis Rotation Angle", &Zangle, 1, 360))
		{
			scene.GetActiveModel().rotateWorldZ(Zangle);
		}
	}

	ImGui::Text("");
	ImGui::Text("Projection & View-Volume:");

	if (ImGui::CollapsingHeader("Projection Type"))
	{
		if (ImGui::Checkbox("Orthographic", &ortho))
		{
			scene.getActiveCamera().setProjection(-1, 1, -1, 1, -1, 1,true);
			persp = false;
		}
		if (ImGui::Checkbox("Perspective", &persp))
		{
			scene.getActiveCamera().setProjection(-1, 1, -1, 1, -1, 1,false);
			ortho = false;
		}

		///////////////////////////////////////////////////
		/////////////////////////////////////////////////
		///////////////////////////////////////////////
	    /////////////////////////////////////////////

		if (ImGui::CollapsingHeader("View Volume"))
		{
			if (ortho || persp)
			{
				static float left = -1.0f;
				static float right = 1.0f;
				static float bottom = -1.0f;
				static float top = 1.0f;
				static float Near = 1.0f;
				static float Far = 1.0f;

				if (ortho)
				{
					if (ImGui::SliderFloat("Left", &left, -10, 10))
					{
						scene.getActiveCamera().setProjection(left, right, bottom, top, -1, 1, true);
					}
					if (ImGui::SliderFloat("Right", &right, -10, 10))
					{
						scene.getActiveCamera().setProjection(left, right, bottom, top, -1, 1, true);
					}
					if (ImGui::SliderFloat("Top", &top, -10, 10))
					{
						scene.getActiveCamera().setProjection(left, right, bottom, top, -1, 1, true);
					}
					if (ImGui::SliderFloat("Bottom", &bottom, -10, 10))
					{
						scene.getActiveCamera().setProjection(left, right, bottom, top, -1, 1, true);
					}
					if (ImGui::SliderFloat("Near", &Near, -10, 10)) {}
					if (ImGui::SliderFloat("Far", &Far, -10, 10)) {}
				}

				if(false)
				{
					float pFov = (float)(M_PI / 4);
					float pNear = 1.5f;
					float pFar = -10.0f;

					if (ImGui::SliderFloat("Fov Value", &pFov, 0.0f, (float)(2*M_PI)))
					{
						scene.getActiveCamera().setProjection(left, right, bottom, top, pFov, pNear, false);
					}
					if (ImGui::SliderFloat("Near Value", &pNear, -10, 10))
					{
						scene.getActiveCamera().setProjection(left, right, bottom, top, pFov, pNear, false);
					}
					if (ImGui::SliderFloat("Far Value", &pFar, -10, 10))
					{
						scene.getActiveCamera().setProjection(left, right, bottom, top, pFov, pNear, false);
					}
				}
			}
			
		}
	}

	ImGui::Text("");
	ImGui::Text("Camera:");

	if (ImGui::CollapsingHeader("Translate Camera - Local"))
	{
		ImGui::Text("Translate Local");
		static float Cam_X_AxisL = 0;
		static float Cam_Y_AxisL = 0;
		static float Cam_Z_AxisL = 0;
		ImGui::SetNextItemWidth(100);
		if (ImGui::SliderFloat("Cam Local X-Axis", &Cam_X_AxisL, -10, 10))
		{
			scene.getActiveCamera().camTranslateLocal(Cam_X_AxisL, Cam_Y_AxisL, Cam_Z_AxisL);
		}
		ImGui::SetNextItemWidth(100);
		if (ImGui::SliderFloat("Cam Local Y-Axis", &Cam_Y_AxisL, -10, 10))
		{
			scene.getActiveCamera().camTranslateLocal(Cam_X_AxisL, Cam_Y_AxisL, Cam_Z_AxisL);
		}
		ImGui::SetNextItemWidth(100);
		if (ImGui::SliderFloat("Cam Local Z-Axis", &Cam_Z_AxisL, -10, 10))
		{
			scene.getActiveCamera().camTranslateLocal(Cam_X_AxisL, Cam_Y_AxisL, Cam_Z_AxisL);
		}
	}


	if (ImGui::CollapsingHeader("Rotate Camera - Local"))
	{
		static float CamXangleL;
		static float CamYangleL;
		static float CamZangleL;

		ImGui::SetNextItemWidth(100);
		if (ImGui::SliderFloat("Cam Local X-Axis Rotation Angle", &CamXangleL, 1, 360))
		{
			scene.getActiveCamera().camRotateLocal(CamXangleL, CamYangleL, CamZangleL);
		}
		ImGui::SetNextItemWidth(100);
		if (ImGui::SliderFloat("Cam Local Y-Axis Rotation Angle", &CamYangleL, 1, 360))
		{
			scene.getActiveCamera().camRotateLocal(CamXangleL, CamYangleL, CamZangleL);
		}
		ImGui::SetNextItemWidth(100);
		if (ImGui::SliderFloat("Cam Local Z-Axis Rotation Angle", &CamZangleL, 1, 360))
		{
			scene.getActiveCamera().camRotateLocal(CamXangleL, CamYangleL, CamZangleL);
		}
	}

	if (ImGui::CollapsingHeader("Translate Camera - World"))
	{
		ImGui::Text("Translate Cam World");
		static float Cam_X_AxisW = 0;
		static float Cam_Y_AxisW = 0;
		static float Cam_Z_AxisW = 0;
		ImGui::SetNextItemWidth(100);
		if (ImGui::SliderFloat("Cam World X-Axis", &Cam_X_AxisW, -10, 10))
		{
			scene.getActiveCamera().camTranslateWorld(Cam_X_AxisW, Cam_Y_AxisW, Cam_Z_AxisW);
		}
		ImGui::SetNextItemWidth(100);
		if (ImGui::SliderFloat("Cam World Y-Axis", &Cam_Y_AxisW, -10, 10))
		{
			scene.getActiveCamera().camTranslateWorld(Cam_Y_AxisW, Cam_Y_AxisW, Cam_Z_AxisW);
		}
		ImGui::SetNextItemWidth(100);
		if (ImGui::SliderFloat("Cam World Z-Axis", &Cam_Z_AxisW, -10, 10))
		{
			scene.getActiveCamera().camTranslateWorld(Cam_X_AxisW, Cam_Y_AxisW, Cam_Z_AxisW);
		}
	}

	if (ImGui::CollapsingHeader("Rotate Camera - World"))
	{
		static float CamXangleW;
		static float CamYangleW;
		static float CamZangleW;

		ImGui::SetNextItemWidth(100);
		if (ImGui::SliderFloat("Cam World X-Axis Rotation Angle", &CamXangleW, 1, 360))
		{
			scene.getActiveCamera().camRotateWorld(CamXangleW, CamYangleW, CamZangleW);
		}
		ImGui::SetNextItemWidth(100);
		if (ImGui::SliderFloat("Cam World Y-Axis Rotation Angle", &CamYangleW, 1, 360))
		{
			scene.getActiveCamera().camRotateWorld(CamXangleW, CamYangleW, CamZangleW);
		}
		ImGui::SetNextItemWidth(100);
		if (ImGui::SliderFloat("Cam World Z-Axis Rotation Angle", &CamZangleW, 1, 360))
		{
			scene.getActiveCamera().camRotateWorld(CamXangleW, CamYangleW, CamZangleW);
		}
	}

	ImGui::Text("");

	if (ImGui::Checkbox("Show Axis", &scene.showAxis)) {}

	if (ImGui::Checkbox("Show Bounding Box", &scene.showBoundingBox)) {}

	if (ImGui::Checkbox("Show Normals", &scene.showNormals)) {}

	if (ImGui::Checkbox("Draw rectangles", &scene.showRectangles)) {}

	if (ImGui::Checkbox("Just WireFrame", &scene.wireFrame)) {}

	ImGui::Text("");

	//if (ImGui::Checkbox("Show Axis", &scene.drawRec)) {}




	ImGui::End(); 

}


	


		

}