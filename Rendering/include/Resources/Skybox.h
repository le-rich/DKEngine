#pragma once
#include "Resources/Asset.h"
#include "Resources/Texture.h"
#include "Resources/Primitives.h"

static const std::string DEFAULT_SKYBOX_FOLDER = "Assets/Skybox/";

class Skybox : public Asset
{
public:
	Skybox(std::string pAssetName);

	void Bind();
	void Unbind();

	void Draw();
	
	void GenerateTexture(std::shared_ptr<Texture> texture);

	UUIDv4::UUID mShaderID;
	UUIDv4::UUID mTextureID;
	GLuint mTexture;
private:
	std::shared_ptr<Primitive> mPrimitive;
};

