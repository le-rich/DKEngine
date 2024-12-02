#pragma oncne
#include "Component.h"

#include "Material.h"

#include <glm.hpp>
#include <string>


class MaterialComponent: public Component 
{
public:
    MaterialComponent(Entity* entity);
    ~MaterialComponent();

    MaterialComponent(const MaterialComponent& other);
    Component* clone() const override;

    Material* MakePrimitiveMaterialInstance();
    Material* GetMaterialInstance() { return materialInstance; }

    // Material Info
    // TODO: Metallic vs Specular
    // TODO: Render Face Front/ Back/Both
    // TODO: Alpha Clipping
    // TODO: Receive Shadows Toggle

    // Base Surface Properties
    glm::vec4 albedoColor = glm::vec4(1.0f); // RGBA
    float metallic = 0.0f;
    float roughness = 0.5f;
    glm::vec3 emission = glm::vec3(1.0f);    // RGB

    // Material Maps
    std::string albedoMapPath;          // TODO
    std::string metallicMapPath;        // TODO
    std::string normalMapPath;          // TODO
    std::string heightMapPath;          // TODO
    std::string aoMapPath;              // TODO
    std::string emissionMapPath;        // TODO
   
protected:
    Material* materialInstance = nullptr;
};
