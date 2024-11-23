#pragma once
#include "Asset.h"
#include "Primitives.h"

static const std::string DEFAULT_SKYBOX_FOLDER = "Assets/Skybox/";

class Skybox : public Asset
{
public:
	Skybox(std::string pAssetName);

	void Bind();
	void Unbind();

	void Draw();

	UUIDv4::UUID mShaderID;
	UUIDv4::UUID mTextureID;
	GLuint mTexture;
private:
	std::shared_ptr<Primitive> mPrimitive;
};

