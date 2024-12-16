///////////////////////////////////////////////////////////////////////////////
// scenemanager.h
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ShaderManager.h"
#include "ShapeMeshes.h"

#include <string>
#include <vector>

/***********************************************************
 *  SceneManager
 *
 *  This class contains the code for preparing and rendering
 *  3D scenes, including the shader settings.
 ***********************************************************/
class SceneManager
{
public:
	
	SceneManager(ShaderManager* pShaderManager);
	~SceneManager();
	struct TEXTURE_INFO
	{
		std::string tag;
		uint32_t ID;
	};

	struct OBJECT_MATERIAL
	{
		glm::vec3 diffuseColor;
		glm::vec3 specularColor;
		float shininess;
		std::string tag;
	};

private:
	ShaderManager* m_pShaderManager;
	ShapeMeshes* m_basicMeshes;
	int m_loadedTextures;
	// loaded textures info
	TEXTURE_INFO m_textureIDs[16];
	// defined object materials
	std::vector<OBJECT_MATERIAL> m_objectMaterials;

	// methods for managing OpenGL textures
	bool CreateGLTexture(const char* filename, std::string tag);
	bool FindMaterial(std::string tag, OBJECT_MATERIAL& material);
	void BindGLTextures();
	void DestroyGLTextures();
	int FindTextureID(std::string tag);
	int FindTextureSlot(std::string tag);

	void SetTransformations(
		glm::vec3 scaleXYZ,
		float XrotationDegrees,
		float YrotationDegrees,
		float ZrotationDegrees,
		glm::vec3 positionXYZ);

	void SetShaderColor(
		float redColorValue,
		float greenColorValue,
		float blueColorValue,
		float alphaValue);
	void SetShaderTexture(
		std::string textureTag);

	void SetShaderMaterial(
		std::string materialTag);
	void SetTextureUVScale(
		float u, float v);

public:
	void PrepareScene();
	void RenderScene();
	void LoadSceneTextures();
	void SetupSceneLights();
	void DefineObjectMaterials();
};