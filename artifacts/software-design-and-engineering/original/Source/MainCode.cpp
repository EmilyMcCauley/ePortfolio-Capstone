///////////////////////////////////////////////////////////////////////////////
// maincode.cpp
///////////////////////////////////////////////////////////////////////////////

#include <iostream> 
#include <cstdlib>         

#include <GL/glew.h> 
#include "GLFW/glfw3.h"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "SceneManager.h"
#include "ViewManager.h"
#include "ShapeMeshes.h"
#include "ShaderManager.h"

namespace
{
	const char* const WINDOW_TITLE = "7-1 FinalProject and Milestones";

	GLFWwindow* g_Window = nullptr;
	SceneManager* g_SceneManager = nullptr;
	ShaderManager* g_ShaderManager = nullptr;
	ViewManager* g_ViewManager = nullptr;
}
bool InitializeGLFW();
bool InitializeGLEW();


int main(int argc, char* argv[])
{
	g_ShaderManager = new ShaderManager();
	g_ViewManager = new ViewManager(
		g_ShaderManager);

	g_Window = g_ViewManager->CreateDisplayWindow(WINDOW_TITLE);


	g_ShaderManager->LoadShaders(
		"shaders/vertexShader.glsl",
		"shaders/fragmentShader.glsl");
	g_ShaderManager->use();

	g_SceneManager = new SceneManager(g_ShaderManager);
	g_SceneManager->PrepareScene();

	while (!glfwWindowShouldClose(g_Window))
	{
		glEnable(GL_DEPTH_TEST);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		g_ViewManager->PrepareSceneView();
		g_SceneManager->RenderScene();

		// Flips the the back buffer with the front buffer every frame.
		glfwSwapBuffers(g_Window);

		// query the latest GLFW events
		glfwPollEvents();
	}
}

/***********************************************************
 *	InitializeGLFW()
 *
 *  This function is used to initialize the GLFW library.
 ***********************************************************/
bool InitializeGLFW()
{
	glfwInit();

#ifdef __APPLE__
	// set the version of OpenGL and profile to use
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
	// set the version of OpenGL and profile to use
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

	return(true);
}

bool InitializeGLEW()
{
	GLenum GLEWInitResult = GLEW_OK;

	// try to initialize the GLEW library
	GLEWInitResult = glewInit();

	std::cout << "INFO: OpenGL Successfully Initialized\n";
	std::cout << "INFO: OpenGL Version: " << glGetString(GL_VERSION) << "\n" << std::endl;

	return(true);
}