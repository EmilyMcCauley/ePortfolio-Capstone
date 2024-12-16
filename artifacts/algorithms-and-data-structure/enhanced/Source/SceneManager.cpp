///////////////////////////////////////////////////////////////////////////////
// scenemanager.cpp
///////////////////////////////////////////////////////////////////////////////

#include "SceneManager.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif

#include <glm/gtx/transform.hpp>

// declaration of global variables
namespace
{
	const char* g_ModelName = "model";
	const char* g_ColorValueName = "objectColor";
	const char* g_TextureValueName = "objectTexture";
	const char* g_UseTextureName = "bUseTexture";
	const char* g_UseLightingName = "bUseLighting";
}


/***********************************************************
 *  SceneManager()
 *
 *  The constructor for the class
 ***********************************************************/
SceneManager::SceneManager(ShaderManager* pShaderManager)
{
	m_pShaderManager = pShaderManager;
	m_basicMeshes = new ShapeMeshes();

	// initialize the texture collection
	for (int i = 0; i < 16; i++)
	{
		m_textureIDs[i].tag = "/0";
		m_textureIDs[i].ID = -1;
	}
	m_loadedTextures = 0;
}
/***********************************************************
 *  ~SceneManager()
 *
 *  The destructor for the class
 ***********************************************************/
SceneManager::~SceneManager()
{
	// clear the allocated memory
	m_pShaderManager = NULL;
	delete m_basicMeshes;
	m_basicMeshes = NULL;
	// destroy the created OpenGL textures
	DestroyGLTextures();
}


/***********************************************************
 *  CreateGLTexture()
 *
 *  This method is used for loading textures from image files,
 *  configuring the texture mapping parameters in OpenGL,
 *  generating the mipmaps, and loading the read texture into
 *  the next available texture slot in memory.
 ***********************************************************/
bool SceneManager::CreateGLTexture(const char* filename, std::string tag)
{
	int width = 0;
	int height = 0;
	int colorChannels = 0;
	GLuint textureID = 0;

	// indicate to always flip images vertically when loaded
	stbi_set_flip_vertically_on_load(true);

	// try to parse the image data from the specified image file
	unsigned char* image = stbi_load(
		filename,
		&width,
		&height,
		&colorChannels,
		0);

	// if the image was successfully read from the image file
	if (image)
	{
		std::cout << "Successfully loaded image:" << filename << ", width:" << width << ", height:" << height << ", channels:" << colorChannels << std::endl;

		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// if the loaded image is in RGB format
		if (colorChannels == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		// if the loaded image is in RGBA format - it supports transparency
		else if (colorChannels == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		else
		{
			std::cout << "Not implemented to handle image with " << colorChannels << " channels" << std::endl;
			return false;
		}

		// generate the texture mipmaps for mapping textures to lower resolutions
		glGenerateMipmap(GL_TEXTURE_2D);

		// free the image data from local memory
		stbi_image_free(image);
		glBindTexture(GL_TEXTURE_2D, 0); // Unbind the texture

		// register the loaded texture and associate it with the special tag string
		m_textureIDs[m_loadedTextures].ID = textureID;
		m_textureIDs[m_loadedTextures].tag = tag;
		m_loadedTextures++;

		return true;
	}

	std::cout << "Could not load image:" << filename << std::endl;

	// Error loading the image
	return false;
}

/***********************************************************
 *  BindGLTextures()
 *
 *  This method is used for binding the loaded textures to
 *  OpenGL texture memory slots.  There are up to 16 slots.
 ***********************************************************/
void SceneManager::BindGLTextures()
{
	for (int i = 0; i < m_loadedTextures; i++)
	{
		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, m_textureIDs[i].ID);
	}
}

/***********************************************************
 *  DestroyGLTextures()
 *
 *  This method is used for freeing the memory in all the
 *  used texture memory slots.
 ***********************************************************/
void SceneManager::DestroyGLTextures()
{
	for (int i = 0; i < m_loadedTextures; i++)
	{
		glDeleteTextures(1, &m_textureIDs[i].ID);
	}
}

/***********************************************************
 *  FindTextureID()
 *
 *  This method is used for getting an ID for the previously
 *  loaded texture bitmap associated with the passed in tag.
 ***********************************************************/
int SceneManager::FindTextureID(std::string tag)
{
	int textureID = -1;
	int index = 0;
	bool bFound = false;

	while ((index < m_loadedTextures) && (bFound == false))
	{
		if (m_textureIDs[index].tag.compare(tag) == 0)
		{
			textureID = m_textureIDs[index].ID;
			bFound = true;
		}
		else
			index++;
	}

	return(textureID);
}

/***********************************************************
 *  FindTextureSlot()
 *
 *  This method is used for getting a slot index for the previously
 *  loaded texture bitmap associated with the passed in tag.
 ***********************************************************/
int SceneManager::FindTextureSlot(std::string tag)
{
	int textureSlot = -1;
	int index = 0;
	bool bFound = false;

	while ((index < m_loadedTextures) && (bFound == false))
	{
		if (m_textureIDs[index].tag.compare(tag) == 0)
		{
			textureSlot = index;
			bFound = true;
		}
		else
			index++;
	}

	return(textureSlot);
}
/***********************************************************
 *  FindMaterial()
 *
 *  This method is used for getting a material from the previously
 *  defined materials list that is associated with the passed in tag.
 ***********************************************************/
bool SceneManager::FindMaterial(std::string tag, OBJECT_MATERIAL& material)
{
	if (m_objectMaterials.size() == 0)
	{
		return(false);
	}

	int index = 0;
	bool bFound = false;
	while ((index < m_objectMaterials.size()) && (bFound == false))
	{
		if (m_objectMaterials[index].tag.compare(tag) == 0)
		{
			bFound = true;
			material.diffuseColor = m_objectMaterials[index].diffuseColor;
			material.specularColor = m_objectMaterials[index].specularColor;
			material.shininess = m_objectMaterials[index].shininess;
		}
		else
		{
			index++;
		}
	}

	return(true);
}
/***********************************************************
 *  SetTransformations()
 *
 *  This method is used for setting the transform buffer
 *  using the passed in transformation values.
 ***********************************************************/
void SceneManager::SetTransformations(
	glm::vec3 scaleXYZ,
	float XrotationDegrees,
	float YrotationDegrees,
	float ZrotationDegrees,
	glm::vec3 positionXYZ)
{
	// variables for this method
	glm::mat4 modelView;
	glm::mat4 scale;
	glm::mat4 rotationX;
	glm::mat4 rotationY;
	glm::mat4 rotationZ;
	glm::mat4 translation;

	// set the scale value in the transform buffer
	scale = glm::scale(scaleXYZ);
	// set the rotation values in the transform buffer
	rotationX = glm::rotate(glm::radians(XrotationDegrees), glm::vec3(1.0f, 0.0f, 0.0f));
	rotationY = glm::rotate(glm::radians(YrotationDegrees), glm::vec3(0.0f, 1.0f, 0.0f));
	rotationZ = glm::rotate(glm::radians(ZrotationDegrees), glm::vec3(0.0f, 0.0f, 1.0f));
	// set the translation value in the transform buffer
	translation = glm::translate(positionXYZ);

	modelView = translation * rotationZ * rotationY * rotationX * scale;

	if (NULL != m_pShaderManager)
	{
		m_pShaderManager->setMat4Value(g_ModelName, modelView);
	}
}

/***********************************************************
 *  SetShaderColor()
 *
 *  This method is used for setting the passed in color
 *  into the shader for the next draw command
 ***********************************************************/
void SceneManager::SetShaderColor(
	float redColorValue,
	float greenColorValue,
	float blueColorValue,
	float alphaValue)
{
	// variables for this method
	glm::vec4 currentColor;

	currentColor.r = redColorValue;
	currentColor.g = greenColorValue;
	currentColor.b = blueColorValue;
	currentColor.a = alphaValue;

	if (NULL != m_pShaderManager)
	{
		m_pShaderManager->setVec4Value(g_ColorValueName, currentColor);
	}
}
/***********************************************************
 *  SetShaderMaterial()
 *
 *  This method is used for passing the material values
 *  into the shader.
 ***********************************************************/
void SceneManager::SetShaderMaterial(
	std::string materialTag)
{
	if (m_objectMaterials.size() > 0)
	{
		OBJECT_MATERIAL material;
		bool bReturn = false;

		// find the defined material that matches the tag
		bReturn = FindMaterial(materialTag, material);
		if (bReturn == true)
		{
			// pass the material properties into the shader
			m_pShaderManager->setVec3Value("material.diffuseColor", material.diffuseColor);
			m_pShaderManager->setVec3Value("material.specularColor", material.specularColor);
			m_pShaderManager->setFloatValue("material.shininess", material.shininess);
		}
	}
}
/***********************************************************
 *  SetShaderTexture()
 *
 *  This method is used for setting the texture data
 *  associated with the passed in ID into the shader.
 ***********************************************************/
void SceneManager::SetShaderTexture(
	std::string textureTag)
{
	if (NULL != m_pShaderManager)
	{
		m_pShaderManager->setIntValue(g_UseTextureName, true);

		int textureID = -1;
		textureID = FindTextureSlot(textureTag);
		m_pShaderManager->setSampler2DValue(g_TextureValueName, textureID);
	}
}

/***********************************************************
 *  SetTextureUVScale()
 *
 *  This method is used for setting the texture UV scale
 *  values into the shader.
 ***********************************************************/
void SceneManager::SetTextureUVScale(float u, float v)
{
	if (NULL != m_pShaderManager)
	{
		m_pShaderManager->setVec2Value("UVscale", glm::vec2(u, v));
	}
}
/***********************************************************
  *  LoadSceneTextures()
  *
  *  This method is used for preparing the 3D scene by loading
  *  the shapes, textures in memory to support the 3D scene
  *  rendering
  ***********************************************************/
void SceneManager::LoadSceneTextures()
{  
	CreateGLTexture("textures/tea.jpg", "teaTexture");
	CreateGLTexture("textures/wood.jpg", "woodTexture");
	CreateGLTexture("textures/tree.jpg", "treeTexture");
	CreateGLTexture("textures/floor.jpg", "floorTexture");
	CreateGLTexture("textures/bamboo.jpg", "bambooTexture");
	CreateGLTexture("textures/rug.jpg", "rugTexture");
	CreateGLTexture("textures/wood2.jpg", "wood2Texture");
	BindGLTextures();
}
void SceneManager::DefineObjectMaterials()
{
	OBJECT_MATERIAL Material1;
	Material1.diffuseColor = glm::vec3(0.8f, 0.4f, 0.8f);
	Material1.specularColor = glm::vec3(0.2f, 0.2f, 0.2f);
	Material1.shininess = 1.0;
	Material1.tag = "Material1";
	m_objectMaterials.push_back(Material1);
	OBJECT_MATERIAL Material2;
	Material2.diffuseColor = glm::vec3(0.6f, 0.5f, 0.2f);
	Material2.specularColor = glm::vec3(0.1f, 0.2f, 0.2f);
	Material2.shininess = 1.0;
	Material2.tag = "Material2";
	m_objectMaterials.push_back(Material2);
	OBJECT_MATERIAL Material3;
	Material3.diffuseColor = glm::vec3(0.3f, 0.3f, 0.2f);
	Material3.specularColor = glm::vec3(0.7f, 0.7f, 0.8f);
	Material3.shininess = 8.0;
	Material3.tag = "Material3";
	m_objectMaterials.push_back(Material3);
	OBJECT_MATERIAL Material4;
	Material4.diffuseColor = glm::vec3(0.3f, 0.3f, 0.2f);
	Material4.specularColor = glm::vec3(0.9f, 0.9f, 0.8f);
	Material4.shininess = 10.0;
	Material4.tag = "Material4";
	m_objectMaterials.push_back(Material4);
	OBJECT_MATERIAL Material5;
	Material5.diffuseColor = glm::vec3(0.5f, 0.5f, 0.5f);
	Material5.specularColor = glm::vec3(0.7f, 0.7f, 0.7f);
	Material5.shininess = 6.0;
	Material5.tag = "Material5";
	m_objectMaterials.push_back(Material5);
	OBJECT_MATERIAL Material6;
	Material6.diffuseColor = glm::vec3(0.5f, 0.5f, 0.5f);
	Material6.specularColor = glm::vec3(0.73f, 0.3f, 0.3f);
	Material6.shininess = 6.0;
	Material6.tag = "Material6";
	m_objectMaterials.push_back(Material6);

}
/***********************************************************
 *  SetupSceneLights()
 *
 *  This method is called to add and configure the light
 *  sources for the 3D scene.  There are up to 4 light sources.
 ***********************************************************/
void SceneManager::SetupSceneLights()
{
	m_pShaderManager->setBoolValue(g_UseLightingName, true);
  
	// point light 1
	m_pShaderManager->setVec3Value("pointLights[0].position", -4.0f, 4.0f, 4.0f);
	m_pShaderManager->setVec3Value("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
	m_pShaderManager->setVec3Value("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
	m_pShaderManager->setVec3Value("pointLights[0].specular", 0.2f, 0.2f, 0.2f);
	m_pShaderManager->setBoolValue("pointLights[0].bActive", true);
	// point light 2
	m_pShaderManager->setVec3Value("pointLights[1].position", 4.0f, 4.0f, 4.0f);
	m_pShaderManager->setVec3Value("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
	m_pShaderManager->setVec3Value("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
	m_pShaderManager->setVec3Value("pointLights[1].specular", 0.2f, 0.2f, 0.2f);
	m_pShaderManager->setBoolValue("pointLights[1].bActive", true);
	// point light 3
	m_pShaderManager->setVec3Value("pointLights[2].position", 0.0f, 6.0f, 2.0f);
	m_pShaderManager->setVec3Value("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
	m_pShaderManager->setVec3Value("pointLights[2].diffuse", 1.0f, 1.0f, 1.0f);
	m_pShaderManager->setVec3Value("pointLights[2].specular", 0.2f, 0.2f, 0.2f);
	m_pShaderManager->setBoolValue("pointLights[2].bActive", true);
	// point light 4
	m_pShaderManager->setVec3Value("pointLights[3].position", -3.0f, 6.0f, 6.0f);
	m_pShaderManager->setVec3Value("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
	m_pShaderManager->setVec3Value("pointLights[3].diffuse", 0.3f, 0.3f, 0.3f);
	m_pShaderManager->setVec3Value("pointLights[3].specular", 0.8f, 0.8f, 0.8f);
	m_pShaderManager->setBoolValue("pointLights[3].bActive", true);

}

/***********************************************************
 *  PrepareScene()
 *
 *  This method is used for preparing the 3D scene by loading
 *  the shapes, textures in memory to support the 3D scene
 *  rendering
 ***********************************************************/
void SceneManager::PrepareScene()
{
	// load the textures for the 3D scene
	LoadSceneTextures();
	// define the materials for objects in the scene
	DefineObjectMaterials();
	// add and define the light sources for the scene
	SetupSceneLights();

	m_basicMeshes->LoadCylinderMesh();
	m_basicMeshes->LoadPlaneMesh();     // Load the plane for the table/ground
	m_basicMeshes->LoadSphereMesh();
	m_basicMeshes->LoadTaperedCylinderMesh();
	m_basicMeshes->LoadTorusMesh();
}

/***********************************************************
 *  RenderScene()
 *
 *  This method is called to render the scene
 ***********************************************************/
void SceneManager::RenderScene()
{
	// plate
	glm::vec3 scalePlate = glm::vec3(4.0f, 0.05f, 4.0f); // Adjust the size to be wider and thinner for a plate
	SetTransformations(scalePlate, 0.0f, 0.0f, 0.0f, glm::vec3(0.0f, -0.8f, 0.0f)); // Position it just above the table
	SetShaderTexture("bambooTexture"); // Use a suitable texture for the plate
	SetShaderMaterial("Material2"); // Material for the plate
	m_basicMeshes->DrawCylinderMesh(); // Draw the plate (still using DrawCylinderMesh, but with adjusted scale)

	// table
	glm::vec3 scaleTable = glm::vec3(10.0f, 0.01f, 10.0f); // Adjust size for a circular appearance
	SetTransformations(scaleTable, 0.0f, 0.0f, 0.0f, glm::vec3(0.0f, -1.0f, 0.0f)); // Position the circular table
	SetShaderTexture("woodTexture"); // Use a wood texture for the table
	SetShaderMaterial("Material2");
	m_basicMeshes->DrawCylinderMesh();

	// Drawing table legs
	glm::vec3 legScale = glm::vec3(0.2f, 10.0f, 0.2f); // Larger scale for table legs
	SetShaderTexture("woodTexture"); // Use wood texture for legs
	SetShaderMaterial("Material2");

	// Position for Leg 1 (front-left)
	SetTransformations(legScale, 0.0f, 0.0f, 0.0f, glm::vec3(-4.0f, -11.0f, 4.0f)); // Position it under the table
	SetShaderTexture("woodTexture"); // Use wood texture for legs
	SetShaderMaterial("Material2");
	m_basicMeshes->DrawCylinderMesh(); // Draw Leg 1

	// Position for Leg 2 (front-right)
	SetTransformations(legScale, 0.0f, 0.0f, 0.0f, glm::vec3(4.0f, -11.0f, 4.0f)); // Position it under the table
	SetShaderTexture("woodTexture"); // Use wood texture for legs
	SetShaderMaterial("Material2");
	m_basicMeshes->DrawCylinderMesh(); // Draw Leg 2

	// Position for Leg 3 (back-left)
	SetTransformations(legScale, 0.0f, 0.0f, 0.0f, glm::vec3(-4.0f, -11.0f, -4.0f)); // Position it under the table
	SetShaderTexture("woodTexture"); // Use wood texture for legs
	SetShaderMaterial("Material2");
	m_basicMeshes->DrawCylinderMesh(); // Draw Leg 3

	// Position for Leg 4 (back-right)
	SetTransformations(legScale, 0.0f, 0.0f, 0.0f, glm::vec3(4.0f, -11.0f, -4.0f)); // Position it under the table
	SetShaderTexture("woodTexture"); // Use wood texture for legs
	SetShaderMaterial("Material2");
	m_basicMeshes->DrawCylinderMesh(); // Draw Leg 4

	// teapot base
	glm::vec3 scaleTeapotBase = glm::vec3(1.2f, 0.5f, 1.2f); // Scale for teapot base
	SetTransformations(scaleTeapotBase, 0.0f, 0.0f, 0.0f, glm::vec3(0.0f, -0.5f, 0.0f)); // Position it
	SetShaderTexture("teaTexture"); // Use a ceramic texture
	SetShaderMaterial("Material4"); // Material for the cup
	m_basicMeshes->DrawSphereMesh(); // Draw the teapot body

	// Teapot Lid
	glm::vec3 scaleTeapotLid = glm::vec3(0.6f, 0.3f, 0.6f); // Scale for teapot lid
	SetTransformations(scaleTeapotLid, 0.0f, 0.0f, 0.0f, glm::vec3(0.0f, 0.0f, 0.0f)); // Position it on top
	SetShaderTexture("teaTexture"); // Use a ceramic texture
	SetShaderMaterial("Material4"); // Material for the cup
	m_basicMeshes->DrawSphereMesh(); // Draw the teapot lid

	// Teapot Lid2
	glm::vec3 scaleTeapotLid2 = glm::vec3(0.1f, 0.2f, 0.1f); // Scale for teapot lid
	SetTransformations(scaleTeapotLid2, 0.0f, 0.0f, 0.0f, glm::vec3(0.0f, 0.3f, 0.0f)); // Position it on top
	SetShaderTexture("teaTexture"); // Use a ceramic texture
	SetShaderMaterial("Material4"); // Material for the cup
	m_basicMeshes->DrawSphereMesh(); // Draw the teapot lid

	//teapot spout
	glm::vec3 scaleTeapotSpout = glm::vec3(0.2f, 0.4f, 0.2f); // Scale for spout
	SetTransformations(scaleTeapotSpout, 0.0f, 0.0f, 0.0f, glm::vec3(1.0f, -0.3f, 0.0f)); // Position it on the side
	SetShaderTexture("teaTexture"); // Use a ceramic texture
	SetShaderMaterial("Material4"); // Material for the cup
	m_basicMeshes->DrawTaperedCylinderMesh(); // Draw the spout
	//teapot handle
	glm::vec3 scaleCupHandle = glm::vec3(0.15f, 0.5f, 0.15f); // Scale for handle
	SetTransformations(scaleCupHandle, 0.0f, 0.0f, 0.0f, glm::vec3(-1.0f, 0.0f, -0.0f)); // Position it
	SetShaderTexture("teaTexture"); // Use a ceramic texture
	SetShaderMaterial("Material4"); // Material for the cup
	m_basicMeshes->DrawTorusMesh(); // Draw the cup handle
	//cup body
	glm::vec3 scaleCupBody = glm::vec3(0.5f, 0.3f, 0.5f); // Scale for cup body
	SetTransformations(scaleCupBody, 0.0f, 0.0f, 0.0f, glm::vec3(2.5f, -0.75f, 1.5f)); // Position it next to the teapot
	SetShaderTexture("teaTexture"); // Use a ceramic texture
	SetShaderMaterial("Material4"); // Material for the cup
	m_basicMeshes->DrawCylinderMesh(); // Draw the cup body

	// Cup Base
	glm::vec3 scaleCupBase = glm::vec3(0.45f, 0.05f, 0.45f); // Scale for cup base
	SetTransformations(scaleCupBase, 0.0f, 0.0f, 0.0f, glm::vec3(2.5f, -0.75f - 0.05f, 1.5f)); // Position it just below the cup body
	SetShaderTexture("teaTexture"); // Use a ceramic texture
	SetShaderMaterial("Material4"); // Material for the cup
	m_basicMeshes->DrawCylinderMesh(); // Draw the cup base

	// Cup Handle
	glm::vec3 scaleCupHandle2 = glm::vec3(0.1f, 0.3f, 0.1f); // Scale for cup handle
	SetTransformations(scaleCupHandle2, 0.0f, 0.0f, 0.0f, glm::vec3(2.0f, -0.5f, 1.5f)); // Position the handle
	SetShaderTexture("teaTexture"); // Use a ceramic texture
	SetShaderMaterial("Material4"); // Material for the cup
	m_basicMeshes->DrawTorusMesh(); // Draw the cup handle

	// Second Cup Body
	glm::vec3 scaleCupBody2 = glm::vec3(0.5f, 0.3f, 0.5f); // Scale for second cup body
	SetTransformations(scaleCupBody2, 0.0f, 0.0f, 0.0f, glm::vec3(-1.5f, -0.75f, 1.5f)); // Position it on the left side of the plate
	SetShaderTexture("teaTexture"); // Use a ceramic texture
	SetShaderMaterial("Material4"); // Material for the cup
	m_basicMeshes->DrawCylinderMesh(); // Draw the second cup body

	// Second Cup Base
	glm::vec3 scaleCupBase2 = glm::vec3(0.45f, 0.05f, 0.45f); // Scale for second cup base
	SetTransformations(scaleCupBase2, 0.0f, 0.0f, 0.0f, glm::vec3(-1.5f, -0.75f - 0.05f, 1.5f)); // Position it just below the second cup body
	SetShaderTexture("teaTexture"); // Use a ceramic texture
	SetShaderMaterial("Material4"); // Material for the cup
	m_basicMeshes->DrawCylinderMesh(); // Draw the second cup base

	// Second Cup Handle
	glm::vec3 scaleCupHandle3 = glm::vec3(0.1f, 0.3f, 0.1f); // Scale for second cup handle
	SetTransformations(scaleCupHandle3, 0.0f, 0.0f, 0.0f, glm::vec3(-2.0f, -0.5f, 1.5f)); // Position the handle to the left of the second cup body
	SetShaderTexture("teaTexture"); // Use a ceramic texture
	SetShaderMaterial("Material4"); // Material for the cup
	m_basicMeshes->DrawTorusMesh(); // Draw the second cup handle

	// Drawing the floor as a plane
	glm::vec3 scaleFloor = glm::vec3(30.0f, 1.0f, 30.0f);
	SetTransformations(scaleFloor, 0.0f, 0.0f, 0.0f, glm::vec3(0.0f, -11.5f, 0.0f));
	SetShaderTexture("floorTexture");
	SetShaderMaterial("Material6");
	m_basicMeshes->DrawPlaneMesh();

	// Circular Rug under the table
	glm::vec3 scaleRug = glm::vec3(15.0f, 0.5f, 15.0f);// (slightly larger than table)
	SetTransformations(scaleRug, 0.0f, 0.0f, 0.0f, glm::vec3(0.0f, -11.01f, 0.0f)); // Position just below the table
	SetShaderTexture("rugTexture"); // Use a rug texture or a colored material
	m_basicMeshes->DrawCylinderMesh(); // Draw the rug (cylinder scaled to look like a flat disk)

	// Create the barrel
	glm::vec3 scaleBarrelBody = glm::vec3(3.0f, 6.5f, 3.0f);  // Larger dimensions for the barrel
	SetTransformations(scaleBarrelBody, 0.0f, 0.0f, 0.0f, glm::vec3(-12.0f, -11.5f, 0.0f));  // Positioned level with the floor
	SetShaderTexture("wood2Texture");  // Use a wood texture for the barrel
	SetShaderMaterial("Material2");  // Use appropriate material for wooden texture
	m_basicMeshes->DrawCylinderMesh();  // Draw the main barrel body

	// Create the barrel
	glm::vec3 scaleBarrel2Body = glm::vec3(3.0f, 6.5f, 3.0f);  // Larger dimensions for the barrel
	SetTransformations(scaleBarrel2Body, 0.0f, 0.0f, 0.0f, glm::vec3(12.0f, -11.5f, 0.0f));  // Positioned level with the floor
	SetShaderTexture("wood2Texture");  // Use a wood texture for the barrel
	SetShaderMaterial("Material2");  // Use appropriate material for wooden texture
	m_basicMeshes->DrawCylinderMesh();  // Draw the main barrel body

}