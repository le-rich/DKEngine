#pragma once
#include "Resources/Asset.h"

class Texture : public Asset
{
private:
	unsigned int mTextureID;
	int mType;
	std::string mFilePath;
	unsigned char* mBuffer;
	int mWidth, mHeight, mBPP;

public:
	Texture(const std::string& pPath);
	Texture(const std::string& pPath, const std::string& pFileName);
	~Texture();

	void Bind() const;
	void BindToSlot(size_t pSlot) const;
	void Unbind() const;
};

