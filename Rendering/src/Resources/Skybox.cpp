#include <glad/glad.h>

#include "Data/Shape.h"
#include "Managers/AssetManager.h"
#include "Resources/Skybox.h"

Skybox::Skybox(std::string pAssetName) : Asset(pAssetName)
{
    mPrimitive = std::make_shared<Primitive>(Cube.vertices, Cube.indices);

    //unsigned int envCubemap;
    glGenTextures(1, &mTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, mTexture);
    for (unsigned int i = 0; i < 6; ++i)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Skybox::Bind()
{
    AssetManager::GetInstance().GetShaderByID(mShaderID)->Use();
    AssetManager::GetInstance().GetShaderByID(mTexture)->Use();
}

void Skybox::Unbind()
{

}

void Skybox::Draw()
{
    mPrimitive->Draw();
}
