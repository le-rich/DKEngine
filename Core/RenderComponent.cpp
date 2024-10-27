#pragma once

#include "Entity.h"
#include "RenderComponent.h"

#include "../Rendering/include/Resources/Texture.h"
#include "../Rendering/include/Resources/Shader.h"


// default constructor
RenderComponent::RenderComponent(Entity* mEntity, std::shared_ptr<Texture> texture, std::shared_ptr<Shader> shader)
	: Component(mEntity), texture(texture), shader(shader) {}

// getters
std::shared_ptr<Texture> RenderComponent::GetTexture() const {
    return texture;
}

std::shared_ptr<Shader> RenderComponent::GetShader() const {
    return shader;
}

// setters

void RenderComponent::SetTexture(std::shared_ptr<Texture> newTexture) {
    texture = newTexture;
}

void RenderComponent::SetShader(std::shared_ptr<Shader> newShader) {
    shader = newShader;
}