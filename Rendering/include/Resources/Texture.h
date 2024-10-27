#pragma once

#include <glad/glad.h>
#include <string>

#include "Resources/Asset.h"


class Texture : Asset
{
private:
	unsigned int mTexturerID;
	int mType;
	std::string mFilePath;
	unsigned char* mBuffer;
	int mWidth, mHeight, mBPP;

public:
	Texture(const std::string& path);
	~Texture();

	void Bind() const;
	void BindToSlot(size_t pSlot) const;
	void Unbind() const;
};

