///////////////////////////////////////////////////////////////////////////////
// viewmanager.h
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ShaderManager.h"
#include "camera.h"
#include "GLFW/glfw3.h" 

class ViewManager
{
public:
    ViewManager(ShaderManager* pShaderManager);
    ~ViewManager();
    static void Mouse_Position_Callback(GLFWwindow* window, double xMousePos, double yMousePos);

    static void Scroll_Callback(GLFWwindow* window, double xoffset, double yoffset);

private:
    ShaderManager* m_pShaderManager;
    GLFWwindow* m_pWindow;

    // process keyboard events for interaction with the 3D scene
    void ProcessKeyboardEvents();

    // projection type
    bool bOrthographicProjection;

public:
    // create the initial OpenGL display window
    GLFWwindow* CreateDisplayWindow(const char* windowTitle);

    void PrepareSceneView();
};