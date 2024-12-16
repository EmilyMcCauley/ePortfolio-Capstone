///////////////////////////////////////////////////////////////////////////////
// viewmanager.cpp
///////////////////////////////////////////////////////////////////////////////

#include "ViewManager.h"

// GLM Math Header inclusions
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>    

// declaration of the global variables and defines
namespace
{
    // Variables for window width and height
    const int WINDOW_WIDTH = 1000;
    const int WINDOW_HEIGHT = 800;
    const char* g_ViewName = "view";
    const char* g_ProjectionName = "projection";

    // camera object used for viewing and interacting with
    // the 3D scene
    Camera* g_pCamera = nullptr;

    // these variables are used for mouse movement processing
    float gLastX = WINDOW_WIDTH / 2.0f;
    float gLastY = WINDOW_HEIGHT / 2.0f;
    bool gFirstMouse = true;

    // time between current frame and last frame
    float gDeltaTime = 0.0f;
    float gLastFrame = 0.0f;

    // movement speed adjustment factor
    float cameraSpeedFactor = 1.0f;
}

/***********************************************************
 *  ViewManager()
 *
 *  The constructor for the class
 ***********************************************************/
ViewManager::ViewManager(ShaderManager* pShaderManager)
{
    // initialize the member variables
    m_pShaderManager = pShaderManager;
    m_pWindow = NULL;
    g_pCamera = new Camera();
    // default camera view parameters
    g_pCamera->Position = glm::vec3(0.0f, 5.0f, 12.0f);
    g_pCamera->Front = glm::vec3(0.0f, -0.5f, -2.0f);
    g_pCamera->Up = glm::vec3(0.0f, 1.0f, 0.0f);
    g_pCamera->Zoom = 80;
    g_pCamera->MovementSpeed = 20;

    // Set default projection to perspective
    bOrthographicProjection = false;
}

/***********************************************************
 *  ~ViewManager()
 *
 *  The destructor for the class
 ***********************************************************/
ViewManager::~ViewManager()
{
    // free up allocated memory
    m_pShaderManager = NULL;
    m_pWindow = NULL;
    if (NULL != g_pCamera)
    {
        delete g_pCamera;
        g_pCamera = NULL;
    }
}

/***********************************************************
 *  CreateDisplayWindow()
 *
 *  This method is used to create the main display window.
 ***********************************************************/
GLFWwindow* ViewManager::CreateDisplayWindow(const char* windowTitle)
{
    GLFWwindow* window = nullptr;

    // try to create the displayed OpenGL window
    window = glfwCreateWindow(
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        windowTitle,
        NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return NULL;
    }
    glfwMakeContextCurrent(window);

    // tell GLFW to capture all mouse events
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // this callback is used to receive mouse moving events
    glfwSetCursorPosCallback(window, &ViewManager::Mouse_Position_Callback);

    // Set the scroll callback to adjust movement speed
    glfwSetScrollCallback(window, &ViewManager::Scroll_Callback);

    // enable blending for supporting transparent rendering
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_pWindow = window;

    return(window);
}

/***********************************************************
 *  Mouse_Position_Callback()
 *
 *  This method is automatically called from GLFW whenever
 *  the mouse is moved within the active GLFW display window.
 ***********************************************************/
void ViewManager::Mouse_Position_Callback(GLFWwindow* window, double xMousePos, double yMousePos)
{
    if (gFirstMouse)
    {
        gLastX = xMousePos;
        gLastY = yMousePos;
        gFirstMouse = false;
    }

    // calculate the X offset and Y offset values for moving the 3D camera accordingly
    float xOffset = xMousePos - gLastX;
    float yOffset = gLastY - yMousePos; // reversed since y-coordinates go from bottom to top

    // set the current positions into the last position variables
    gLastX = xMousePos;
    gLastY = yMousePos;

    // move the 3D camera according to the calculated offsets
    g_pCamera->ProcessMouseMovement(xOffset, yOffset);
}

/***********************************************************
 *  Scroll_Callback()
 *
 *  This method is automatically called from GLFW whenever
 *  the mouse wheel is scrolled.
 ***********************************************************/
void ViewManager::Scroll_Callback(GLFWwindow* window, double xoffset, double yoffset)
{
    // Adjust the camera zoom based on scroll input
    if (yoffset > 0)
    {
        cameraSpeedFactor += 0.1f; // Zoom in or increase speed
    }
    else if (yoffset < 0)
    {
        cameraSpeedFactor = std::max(0.1f, cameraSpeedFactor - 0.1f); // Zoom out or decrease speed but not below 0.1
    }
    g_pCamera->MovementSpeed = 20.0f * cameraSpeedFactor; // Adjust the camera movement speed
}

/***********************************************************
 *  ProcessKeyboardEvents()
 *
 *  This method is called to process any keyboard events
 *  that may be waiting in the event queue.
 ***********************************************************/
void ViewManager::ProcessKeyboardEvents()
{
    // close the window if the escape key has been pressed
    if (glfwGetKey(m_pWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(m_pWindow, true);
    }

    // switch between perspective and orthographic views
    if (glfwGetKey(m_pWindow, GLFW_KEY_P) == GLFW_PRESS)
    {
        bOrthographicProjection = false; // Set to perspective
    }
    if (glfwGetKey(m_pWindow, GLFW_KEY_O) == GLFW_PRESS)
    {
        bOrthographicProjection = true; // Set to orthographic
    }

    // process camera zooming in and out
    if (glfwGetKey(m_pWindow, GLFW_KEY_W) == GLFW_PRESS)
    {
        g_pCamera->ProcessKeyboard(FORWARD, gDeltaTime);
    }
    if (glfwGetKey(m_pWindow, GLFW_KEY_S) == GLFW_PRESS)
    {
        g_pCamera->ProcessKeyboard(BACKWARD, gDeltaTime);
    }

    // process camera panning left and right
    if (glfwGetKey(m_pWindow, GLFW_KEY_A) == GLFW_PRESS)
    {
        g_pCamera->ProcessKeyboard(LEFT, gDeltaTime);
    }
    if (glfwGetKey(m_pWindow, GLFW_KEY_D) == GLFW_PRESS)
    {
        g_pCamera->ProcessKeyboard(RIGHT, gDeltaTime);
    }

    // process upward and downward movement
    if (glfwGetKey(m_pWindow, GLFW_KEY_Q) == GLFW_PRESS)
    {
        g_pCamera->Position += g_pCamera->Up * gDeltaTime * g_pCamera->MovementSpeed; // Move up
    }
    if (glfwGetKey(m_pWindow, GLFW_KEY_E) == GLFW_PRESS)
    {
        g_pCamera->Position -= g_pCamera->Up * gDeltaTime * g_pCamera->MovementSpeed; // Move down
    }
}

/***********************************************************
 *  PrepareSceneView()
 *
 *  This method is used for preparing the 3D scene by loading
 *  the shapes, textures in memory to support the 3D scene
 *  rendering
 ***********************************************************/
void ViewManager::PrepareSceneView()
{
    glm::mat4 view;
    glm::mat4 projection;

    // per-frame timing
    float currentFrame = glfwGetTime();
    gDeltaTime = currentFrame - gLastFrame;
    gLastFrame = currentFrame;

    // process any keyboard events that may be waiting in the 
    // event queue
    ProcessKeyboardEvents();

    // get the current view matrix from the camera
    view = g_pCamera->GetViewMatrix();

    // define the current projection matrix
    if (bOrthographicProjection)
    {
        // Orthographic projection
        projection = glm::ortho(
            -10.0f, 10.0f, // left, right
            -10.0f, 10.0f, // bottom, top
            0.1f, 100.0f   // near, far
        );
    }
    else
    {
        // Perspective projection
        projection = glm::perspective(glm::radians(g_pCamera->Zoom),
            (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT,
            0.1f,
            100.0f);
    }

    // if the shader manager object is valid
    if (NULL != m_pShaderManager)
    {
        // view matrix into the shader for proper rendering
        m_pShaderManager->setMat4Value(g_ViewName, view);
        // projection matrix into the shader for proper rendering
        m_pShaderManager->setMat4Value(g_ProjectionName, projection);
        // view position of the camera into the shader for proper rendering
        m_pShaderManager->setVec3Value("viewPosition", g_pCamera->Position);
    }
}