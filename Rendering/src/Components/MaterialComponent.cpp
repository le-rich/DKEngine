#include "MaterialComponent.h"

MaterialComponent::MaterialComponent(Entity *entity) : Component(entity)
{
    this->componentType = ComponentType::Material;
}

MaterialComponent::~MaterialComponent() 
{

}

MaterialComponent::MaterialComponent(const MaterialComponent &other) : Component(other) 
{
    this->componentType = ComponentType::Material;

   this->albedoColor = other.albedoColor;
   this->metallic = other.metallic;
   this->roughness = other.roughness;
   this->emission = other.emission;

   this->albedoMapPath = other.albedoMapPath;
   this->metallicMapPath = other.metallicMapPath;
   this->normalMapPath = other.normalMapPath;
   this->heightMapPath = other.heightMapPath;
   this->aoMapPath = other.aoMapPath;
   this->emissionMapPath = other.emissionMapPath;
}

Component *MaterialComponent::clone() const 
{
    return new MaterialComponent(*this); 
}

Material *MaterialComponent::MakePrimitiveMaterialInstance() 
{ 
    return nullptr;
}
