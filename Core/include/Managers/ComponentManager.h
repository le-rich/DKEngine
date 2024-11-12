#pragma once

#include "Component.h"
#include "Entity.h"
#include "Components/TransformComponent.h"
#include "Components/RigidBodyComponent.h"
#include "Components/CameraComponent.h"
#include "Components/LightComponent.h"
#include "Components/MeshComponent.h"
#include "Components/ScriptComponent.h"
#include "Script.h"
#include "Scripts/OrbitScript.h"

#include <string>

// Get list of components from name
void ParseComponents(std::string pName, std::vector<std::pair< ComponentType, std::string >>& pComponentListInOut)
{
    // TODO: Parse name to get components to be added to entity
}

std::vector<std::pair<ScriptType, ScriptParams*>> ParseScriptData(std::string pScriptString)
{
    // TODO: Parse script section to get scripts to add
    // TODO: Parse params data
    return { { ScriptType::OrbitScript, new OrbitScriptParams{} } };
}

namespace ComponentManager
{

    static MeshComponent* AddMeshComponent(Entity* pEntity, Mesh* pMesh)
    {
        MeshComponent* meshComponent = new MeshComponent(pEntity);
        meshComponent->setMesh(pMesh);
        pEntity->addComponent(*meshComponent);
        return meshComponent;
    }

    static CameraComponent* AddCameraComponent(Entity* pEntity)
    {
        CameraComponent* cameraComponent = new CameraComponent(pEntity);
        pEntity->addComponent(*cameraComponent);
        return cameraComponent;
    }

    static LightComponent* AddLightComponent(Entity* pEntity)
    {
        LightComponent* lightComponent = new LightComponent(pEntity);
        pEntity->addComponent(*lightComponent);
        return lightComponent;
    }

    static void AddScriptToComponent(Entity* pEntity, ScriptComponent* pScriptComponent, ScriptType pScriptType, ScriptParams* pScriptParams)
    {
        switch (pScriptType)
        {
        case ScriptType::OrbitScript:
        {
            OrbitScript* script;
            script = new OrbitScript(pEntity);
            script->SetParameters(pScriptParams);
            pScriptComponent->AddScript<OrbitScript>(*script);
            break;
        }
        default:
            break;
        }
    }

    static ScriptComponent* AddScriptComponent(Entity* pEntity)
    {
        ScriptComponent* scriptComponent = new ScriptComponent(pEntity);
        pEntity->addComponent(*scriptComponent);
        return scriptComponent;
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
            {
                std::vector<std::pair<ScriptType, ScriptParams*>> scripts = ParseScriptData(component.second);
                ScriptComponent* component = AddScriptComponent(pEntity);
                for (auto& scriptData : scripts)
                {
                    AddScriptToComponent(pEntity, component, scriptData.first, scriptData.second);
                }
            }
            break;
            default:
                break;
            }
        }
    }
};