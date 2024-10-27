#pragma once

#include <glad/glad.h>
#include <string>


class Texture
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
	void Unbind() const;
};

