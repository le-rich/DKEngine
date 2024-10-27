#pragma once

#include "Component.h"
#include "../Rendering/include/Resources/Texture.h"
#include "../Rendering/include/Resources/Shader.h"


#include <memory>

// maybe this needs to store a model as well?

class RenderComponent : public Component {
public:
	RenderComponent(Entity* mEntity, std::shared_ptr<Texture> texture,
		std::shared_ptr<Shader> shader);

	// shared_ptr in case they get stored across multiple entities, mayb more memory efficient
	// getters
	std::shared_ptr<Texture> GetTexture() const;
	std::shared_ptr<Shader> GetShader() const;

	// setters
	void SetTexture(std::shared_ptr<Texture> newTexture);
	void SetShader(std::shared_ptr<Shader> newShader);

private:
	std::shared_ptr<Texture> texture;
	std::shared_ptr<Shader> shader;
};