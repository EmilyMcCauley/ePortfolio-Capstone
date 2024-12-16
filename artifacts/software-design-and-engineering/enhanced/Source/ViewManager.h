///////////////////////////////////////////////////////////////////////////////
// viewmanager.h
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ShaderManager.h"
#include "camera.h"

// GLFW library
#include "GLFW/glfw3.h" 

class ViewManager
{
public:
    // constructor
    ViewManager(ShaderManager* pShaderManager);
    // destructor
    ~ViewManager();

    // mouse position callback for mouse interaction with the 3D scene
    static void Mouse_Position_Callback(GLFWwindow* window, double xMousePos, double yMousePos);

    // scroll callback for mouse wheel interaction
    static void Scroll_Callback(GLFWwindow* window, double xoffset, double yoffset);

private:
    // pointer to shader manager object
    ShaderManager* m_pShaderManager;
    // active OpenGL display window
    GLFWwindow* m_pWindow;

    // process keyboard events for interaction with the 3D scene
    void ProcessKeyboardEvents();

    // projection type
    bool bOrthographicProjection; // false for perspective, true for orthographic

public:
    // create the initial OpenGL display window
    GLFWwindow* CreateDisplayWindow(const char* windowTitle);

    // prepare the conversion from 3D object display to 2D scene display
    void PrepareSceneView();
};