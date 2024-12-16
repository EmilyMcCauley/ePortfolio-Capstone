///////////////////////////////////////////////////////////////////////////////
// scenemanager.cpp
///////////////////////////////////////////////////////////////////////////////

#include "SceneManager.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif

#include <glm/gtx/transform.hpp>

namespace
{
	const char* g_ModelName = "model";
	const char* g_ColorValueName = "objectColor";
	const char* g_TextureValueName = "objectTexture";
	const char* g_UseTextureName = "bUseTexture";
	const char* g_UseLightingName = "bUseLighting";
}



SceneManager::SceneManager(ShaderManager* pShaderManager)
{
	m_pShaderManager = pShaderManager;
	m_basicMeshes = new ShapeMeshes();

	for (int i = 0; i < 16; i++)
	{
		m_textureIDs[i].tag = "/0";
		m_textureIDs[i].ID = -1;
	}
	m_loadedTextures = 0;
}

SceneManager::~SceneManager()
{
	m_pShaderManager = NULL;
	delete m_basicMeshes;
	m_basicMeshes = NULL;
	DestroyGLTextures();
}
class TextureManager {
public: 
	GLuint m_textureIDs[16];
	int m_textureCount;

	TextureManager() : m_textureCount (0) {}
};

static GLuint CreateGLTexture(const std::string& filepath) {
	int width, height, colorChannels;
	unsigned char* image = stbi_load(filepath.c_str(), &width, &height, &colorChannels, 0);

	GLuint textureID;

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(image);

	return textureID;
}

void SceneManager::BindGLTextures()
{
	for (int i = 0; i < m_loadedTextures; i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, m_textureIDs[i].ID);
	}
}
void SceneManager::DestroyGLTextures()
{
	for (int i = 0; i < m_loadedTextures; i++)
	{
		glDeleteTextures(1, &m_textureIDs[i].ID);
	}
}
int SceneManager::FindTextureID(std::string tag)
{
	int textureID = -1;
	int index = 0;
	bool bFound = false;

	return(textureID);
}
int SceneManager::FindTextureSlot(std::string tag)
{
	int textureSlot = -1;
	int index = 0;
	bool bFound = false;


	return(textureSlot);
}
bool SceneManager::FindMaterial(std::string tag, OBJECT_MATERIAL& material)
{

	int index = 0;
	bool bFound = false;
	{
		if (m_objectMaterials[index].tag.compare(tag) == 0)
		{
			bFound = true;
			material.diffuseColor = m_objectMaterials[index].diffuseColor;
			material.specularColor = m_objectMaterials[index].specularColor;
			material.shininess = m_objectMaterials[index].shininess;
		}
	}

	return(true);
}
void SceneManager::SetTransformations(
	glm::vec3 scaleXYZ,
	float XrotationDegrees,
	float YrotationDegrees,
	float ZrotationDegrees,
	glm::vec3 positionXYZ)
{
	glm::mat4 modelView;
	glm::mat4 scale;
	glm::mat4 rotationX;
	glm::mat4 rotationY;
	glm::mat4 rotationZ;
	glm::mat4 translation;
	scale = glm::scale(scaleXYZ);
	rotationX = glm::rotate(glm::radians(XrotationDegrees), glm::vec3(1.0f, 0.0f, 0.0f));
	rotationY = glm::rotate(glm::radians(YrotationDegrees), glm::vec3(0.0f, 1.0f, 0.0f));
	rotationZ = glm::rotate(glm::radians(ZrotationDegrees), glm::vec3(0.0f, 0.0f, 1.0f));
	translation = glm::translate(positionXYZ);

	modelView = translation * rotationZ * rotationY * rotationX * scale;

}
void SceneManager::SetShaderColor(
	float redColorValue,
	float greenColorValue,
	float blueColorValue,
	float alphaValue)
{
	glm::vec4 currentColor;

	currentColor.r = redColorValue;
	currentColor.g = greenColorValue;
	currentColor.b = blueColorValue;
	currentColor.a = alphaValue;

}
void SceneManager::SetShaderMaterial(
	std::string materialTag)
{
	if (m_objectMaterials.size() > 0)
	{
		OBJECT_MATERIAL material;
		bool bReturn = false;
		bReturn = FindMaterial(materialTag, material);
		if (bReturn == true)
		{
			m_pShaderManager->setVec3Value("material.diffuseColor", material.diffuseColor);
			m_pShaderManager->setVec3Value("material.specularColor", material.specularColor);
			m_pShaderManager->setFloatValue("material.shininess", material.shininess);
		}
	}
}
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
void SceneManager::SetTextureUVScale(float u, float v)
{
	if (NULL != m_pShaderManager)
	{
		m_pShaderManager->setVec2Value("UVscale", glm::vec2(u, v));
	}
}
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
void SceneManager::SetupSceneLights()
{
	m_pShaderManager->setBoolValue(g_UseLightingName, true);
	m_pShaderManager->setVec3Value("pointLights[0].position", -4.0f, 4.0f, 4.0f);
	m_pShaderManager->setVec3Value("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
	m_pShaderManager->setVec3Value("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
	m_pShaderManager->setVec3Value("pointLights[0].specular", 0.2f, 0.2f, 0.2f);
	m_pShaderManager->setBoolValue("pointLights[0].bActive", true);
	m_pShaderManager->setVec3Value("pointLights[1].position", 4.0f, 4.0f, 4.0f);
	m_pShaderManager->setVec3Value("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
	m_pShaderManager->setVec3Value("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
	m_pShaderManager->setVec3Value("pointLights[1].specular", 0.2f, 0.2f, 0.2f);
	m_pShaderManager->setBoolValue("pointLights[1].bActive", true);
	m_pShaderManager->setVec3Value("pointLights[2].position", 0.0f, 6.0f, 2.0f);
	m_pShaderManager->setVec3Value("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
	m_pShaderManager->setVec3Value("pointLights[2].diffuse", 1.0f, 1.0f, 1.0f);
	m_pShaderManager->setVec3Value("pointLights[2].specular", 0.2f, 0.2f, 0.2f);
	m_pShaderManager->setBoolValue("pointLights[2].bActive", true);
	m_pShaderManager->setVec3Value("pointLights[3].position", -3.0f, 6.0f, 6.0f);
	m_pShaderManager->setVec3Value("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
	m_pShaderManager->setVec3Value("pointLights[3].diffuse", 0.3f, 0.3f, 0.3f);
	m_pShaderManager->setVec3Value("pointLights[3].specular", 0.8f, 0.8f, 0.8f);
	m_pShaderManager->setBoolValue("pointLights[3].bActive", true);

}
void SceneManager::PrepareScene()
{
	LoadSceneTextures();
	DefineObjectMaterials();
	SetupSceneLights();

	m_basicMeshes->LoadCylinderMesh();
	m_basicMeshes->LoadPlaneMesh();
	m_basicMeshes->LoadSphereMesh();
	m_basicMeshes->LoadTaperedCylinderMesh();
	m_basicMeshes->LoadTorusMesh();
}
void SceneManager::RenderScene()
{

	glm::vec3 scalePlate = glm::vec3(4.0f, 0.05f, 4.0f); 
	SetTransformations(scalePlate, 0.0f, 0.0f, 0.0f, glm::vec3(0.0f, -0.8f, 0.0f)); 
	SetShaderTexture("bambooTexture");
	SetShaderMaterial("Material2"); 
	m_basicMeshes->DrawCylinderMesh(); 

	glm::vec3 scaleTable = glm::vec3(10.0f, 0.01f, 10.0f); 
	SetTransformations(scaleTable, 0.0f, 0.0f, 0.0f, glm::vec3(0.0f, -1.0f, 0.0f)); 
	SetShaderTexture("woodTexture"); 
	SetShaderMaterial("Material2");
	m_basicMeshes->DrawCylinderMesh();

	glm::vec3 legScale = glm::vec3(0.2f, 10.0f, 0.2f); 
	SetShaderTexture("woodTexture");
	SetShaderMaterial("Material2");

	SetTransformations(legScale, 0.0f, 0.0f, 0.0f, glm::vec3(-4.0f, -11.0f, 4.0f)); 
	SetShaderTexture("woodTexture");
	SetShaderMaterial("Material2");
	m_basicMeshes->DrawCylinderMesh(); 

	SetTransformations(legScale, 0.0f, 0.0f, 0.0f, glm::vec3(4.0f, -11.0f, 4.0f)); 
	SetShaderTexture("woodTexture"); 
	SetShaderMaterial("Material2");
	m_basicMeshes->DrawCylinderMesh(); 


	SetTransformations(legScale, 0.0f, 0.0f, 0.0f, glm::vec3(-4.0f, -11.0f, -4.0f)); 
	SetShaderTexture("woodTexture"); 
	SetShaderMaterial("Material2");
	m_basicMeshes->DrawCylinderMesh(); 


	SetTransformations(legScale, 0.0f, 0.0f, 0.0f, glm::vec3(4.0f, -11.0f, -4.0f));
	SetShaderTexture("woodTexture");
	SetShaderMaterial("Material2");
	m_basicMeshes->DrawCylinderMesh();

	glm::vec3 scaleTeapotBase = glm::vec3(1.2f, 0.5f, 1.2f);
	SetTransformations(scaleTeapotBase, 0.0f, 0.0f, 0.0f, glm::vec3(0.0f, -0.5f, 0.0f));
	SetShaderTexture("teaTexture");
	SetShaderMaterial("Material4");
	m_basicMeshes->DrawSphereMesh();

	glm::vec3 scaleTeapotLid = glm::vec3(0.6f, 0.3f, 0.6f);
	SetTransformations(scaleTeapotLid, 0.0f, 0.0f, 0.0f, glm::vec3(0.0f, 0.0f, 0.0f));
	SetShaderTexture("teaTexture");
	SetShaderMaterial("Material4");
	m_basicMeshes->DrawSphereMesh();

	glm::vec3 scaleTeapotLid2 = glm::vec3(0.1f, 0.2f, 0.1f);
	SetTransformations(scaleTeapotLid2, 0.0f, 0.0f, 0.0f, glm::vec3(0.0f, 0.3f, 0.0f));
	SetShaderTexture("teaTexture");
	SetShaderMaterial("Material4");
	m_basicMeshes->DrawSphereMesh();

	glm::vec3 scaleTeapotSpout = glm::vec3(0.2f, 0.4f, 0.2f);
	SetTransformations(scaleTeapotSpout, 0.0f, 0.0f, 0.0f, glm::vec3(1.0f, -0.3f, 0.0f));
	SetShaderTexture("teaTexture");
	SetShaderMaterial("Material4");
	m_basicMeshes->DrawTaperedCylinderMesh();

	glm::vec3 scaleCupHandle = glm::vec3(0.15f, 0.5f, 0.15f); 
	SetTransformations(scaleCupHandle, 0.0f, 0.0f, 0.0f, glm::vec3(-1.0f, 0.0f, -0.0f)); 
	SetShaderTexture("teaTexture");
	SetShaderMaterial("Material4");
	m_basicMeshes->DrawTorusMesh();

	glm::vec3 scaleCupBody = glm::vec3(0.5f, 0.3f, 0.5f);
	SetTransformations(scaleCupBody, 0.0f, 0.0f, 0.0f, glm::vec3(2.5f, -0.75f, 1.5f));
	SetShaderTexture("teaTexture");
	SetShaderMaterial("Material4");
	m_basicMeshes->DrawCylinderMesh();

	glm::vec3 scaleCupBase = glm::vec3(0.45f, 0.05f, 0.45f);
	SetTransformations(scaleCupBase, 0.0f, 0.0f, 0.0f, glm::vec3(2.5f, -0.75f - 0.05f, 1.5f));
	SetShaderTexture("teaTexture"); 
	SetShaderMaterial("Material4"); 
	m_basicMeshes->DrawCylinderMesh(); 

	glm::vec3 scaleCupHandle2 = glm::vec3(0.1f, 0.3f, 0.1f);
	SetTransformations(scaleCupHandle2, 0.0f, 0.0f, 0.0f, glm::vec3(2.0f, -0.5f, 1.5f));
	SetShaderTexture("teaTexture");
	SetShaderMaterial("Material4");
	m_basicMeshes->DrawTorusMesh();

	glm::vec3 scaleCupBody2 = glm::vec3(0.5f, 0.3f, 0.5f);
	SetTransformations(scaleCupBody2, 0.0f, 0.0f, 0.0f, glm::vec3(-1.5f, -0.75f, 1.5f));
	SetShaderTexture("teaTexture");
	SetShaderMaterial("Material4");
	m_basicMeshes->DrawCylinderMesh();

	glm::vec3 scaleCupBase2 = glm::vec3(0.45f, 0.05f, 0.45f); 
	SetTransformations(scaleCupBase2, 0.0f, 0.0f, 0.0f, glm::vec3(-1.5f, -0.75f - 0.05f, 1.5f)); 
	SetShaderTexture("teaTexture"); 
	SetShaderMaterial("Material4");
	m_basicMeshes->DrawCylinderMesh(); 

	glm::vec3 scaleCupHandle3 = glm::vec3(0.1f, 0.3f, 0.1f);
	SetTransformations(scaleCupHandle3, 0.0f, 0.0f, 0.0f, glm::vec3(-2.0f, -0.5f, 1.5f)); 
	SetShaderTexture("teaTexture"); 
	SetShaderMaterial("Material4"); 
	m_basicMeshes->DrawTorusMesh(); 

	glm::vec3 scaleFloor = glm::vec3(30.0f, 1.0f, 30.0f);
	SetTransformations(scaleFloor, 0.0f, 0.0f, 0.0f, glm::vec3(0.0f, -11.5f, 0.0f));
	SetShaderTexture("floorTexture");
	SetShaderMaterial("Material6");
	m_basicMeshes->DrawPlaneMesh();

	glm::vec3 scaleRug = glm::vec3(15.0f, 0.5f, 15.0f);
	SetTransformations(scaleRug, 0.0f, 0.0f, 0.0f, glm::vec3(0.0f, -11.01f, 0.0f)); 
	SetShaderTexture("rugTexture");
	m_basicMeshes->DrawCylinderMesh(); 

	glm::vec3 scaleBarrelBody = glm::vec3(3.0f, 6.5f, 3.0f); 
	SetTransformations(scaleBarrelBody, 0.0f, 0.0f, 0.0f, glm::vec3(-12.0f, -11.5f, 0.0f));
	SetShaderTexture("wood2Texture");
	SetShaderMaterial("Material2");
	m_basicMeshes->DrawCylinderMesh(); 


	glm::vec3 scaleBarrel2Body = glm::vec3(3.0f, 6.5f, 3.0f); 
	SetTransformations(scaleBarrel2Body, 0.0f, 0.0f, 0.0f, glm::vec3(12.0f, -11.5f, 0.0f)); 
	SetShaderTexture("wood2Texture"); 
	SetShaderMaterial("Material2");
	m_basicMeshes->DrawCylinderMesh(); 

}
