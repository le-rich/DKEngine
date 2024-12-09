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

	/* These maps get loaded by the asset manager, which get registered as UUIDs
	Then we generate a material instance that registers itself with a UUID 
	Then when inside a mesh component, we check for a MaterialComponent 
	otherwise we default to its OwnMaterial or a default material instance.

	If the mateiralComponent is not a null pointer, we Draw().

	*/
   
protected:
    Material* materialInstance = nullptr;
};
