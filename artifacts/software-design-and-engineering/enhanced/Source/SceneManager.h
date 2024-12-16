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
	// constructor
	SceneManager(ShaderManager* pShaderManager);
	// destructor
	~SceneManager();

	// properties for loaded texture access
	struct TEXTURE_INFO
	{
		std::string tag;
		uint32_t ID;
	};

	// properties for object materials
	struct OBJECT_MATERIAL
	{
		glm::vec3 diffuseColor;
		glm::vec3 specularColor;
		float shininess;
		std::string tag;
	};

private:
	// pointer to shader manager object
	ShaderManager* m_pShaderManager;
	// pointer to basic shapes object
	ShapeMeshes* m_basicMeshes;
	// the number of loaded textures
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

	// transformation values 
	// into the transform buffer
	void SetTransformations(
		glm::vec3 scaleXYZ,
		float XrotationDegrees,
		float YrotationDegrees,
		float ZrotationDegrees,
		glm::vec3 positionXYZ);

	// color values into the shader
	void SetShaderColor(
		float redColorValue,
		float greenColorValue,
		float blueColorValue,
		float alphaValue);

	// texture data into the shader
	void SetShaderTexture(
		std::string textureTag);

	// object material into the shader
	void SetShaderMaterial(
		std::string materialTag);

	//  texture UV scale into the shader
	void SetTextureUVScale(
		float u, float v);

public:
	void PrepareScene();
	void RenderScene();

	// loads textures from image files
	void LoadSceneTextures();
	// pre-set light sources for 3D scene
	void SetupSceneLights();
	// pre-define the object materials for lighting
	void DefineObjectMaterials();
};