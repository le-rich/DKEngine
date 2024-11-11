#pragma once

#include "Component.h"
#include "Entity.h"
#include "ScriptList.h"
#include "Components/TransformComponent.h"
#include "Components/RigidBodyComponent.h"
#include "Components/CameraComponent.h"
#include "Components/LightComponent.h"
#include "Components/MeshComponent.h"
#include "Components/ScriptComponent.h"

#include <string>

// Get list of components from name
void ParseComponents(std::string pName, std::vector<std::pair< ComponentType, std::string >>& pComponentListInOut)
{
    // TODO: Parse name to get components to be added to entity
    // TODO: Parse script section to get scripts to add
}

std::vector<ScriptName> ParseScriptData(std::string pScriptString)
{
    return std::vector<ScriptName>();
}

namespace ComponentManager
{

    static void AddMeshComponent(Entity* pEntity, Mesh* pMesh)
    {
        MeshComponent* meshComponent = new MeshComponent(pEntity);
        meshComponent->setMesh(pMesh);
        pEntity->addComponent(*meshComponent);
    }

    static void AddCameraComponent(Entity* pEntity)
    {
        //TODO
    }

    static void AddLightComponent(Entity* pEntity)
    {
        //TODO
    }

    static void AddScriptComponent(Entity* pEntity, std::vector<ScriptName> const pScriptList)
    {
        //TODO: Setup each Script
        for (ScriptName script : pScriptList)
        {
            switch (script)
            {
            case ScriptName::OrbitalScript:
                break;
            default:
                break;
            }
        }

    }

    // Add components to entity from entity name
    // Does not add MeshComponents
    static void AddComponents(Entity* pEntity, std::string pName)
    {
        std::vector <std::pair< ComponentType, std::string>> componentList;

        ParseComponents(pName, componentList);
        for (auto& component : componentList)
        {
            switch (component.first)
            {
            case ComponentType::Camera:
                AddCameraComponent(pEntity);
                break;
            case ComponentType::Light:
                AddLightComponent(pEntity);
                break;
            case ComponentType::Script:
                // TODO: parse secondary field into scriptList
                //std::vector<ScriptName> scriptList;
                AddScriptComponent(pEntity, ParseScriptData(component.second));
                break;
            default:
                break;
            }
        }
    }
};