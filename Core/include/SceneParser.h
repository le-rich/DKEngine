#pragma once

#include "GLTFLoader.h"
#include "Components/CameraComponent.h"
#include "Components/LightComponent.h"
#include "Components/ScriptComponent.h"
#include "Managers/EntityManager.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <json.hpp>
#include <fstream>
#include <iostream>
#include <unordered_map>

namespace SceneParser
{
    using json = nlohmann::json;
    enum SceneElement
    {
        ASSETS = 0,
        ENTITIES
    };

    static const std::string ASSET_KEY = "assets";
    static const std::string ENTITY_KEY = "entities";

    std::unordered_map<std::string, SceneElement> const ElementMap = {
        {ASSET_KEY, SceneElement::ASSETS},
        {ENTITY_KEY, SceneElement::ENTITIES}
    };

    void ParseTransform(Entity* pEntity, json& transformObject)
    {
        Transform transform;
        try
        {
            std::vector<float> localPosition;
            transformObject.at("localPosition").get_to(localPosition);
            transform.localPosition = (localPosition.size() > 0) ? glm::vec3(localPosition[0], localPosition[1], localPosition[2]) : glm::vec3();
        }
        catch (json::out_of_range& e)
        {
            std::cout << e.what() << '\n';
        }

        try
        {
            std::vector<float> localOrientation;
            transformObject.at("localOrientation").get_to(localOrientation);
            transform.localOrientation = (localOrientation.size() > 0) ? glm::quat(localOrientation[0], localOrientation[1], localOrientation[2], localOrientation[3]) : glm::quat(1.f, 0, 0, 0);
        }
        catch (json::out_of_range& e)
        {
            std::cout << e.what() << '\n';
        }

        try
        {
            std::vector<float> localScale;
            transformObject.at("localScale").get_to(localScale);
            transform.localScale = (localScale.size() > 0) ? glm::vec3(localScale[0], localScale[1], localScale[2]) : glm::vec3(1.f);
        }
        catch (json::out_of_range& e)
        {
            std::cout << e.what() << '\n';
        }

        pEntity->transform->setTransform(transform);
    }

    void getComponents(Entity* pEntity, json& pComponentObjects)
    {
        for (auto& componentObject : pComponentObjects.items())
        {
            std::cout << componentObject.key() << " : " << componentObject.value() << "\n";
            std::string typeString;
            componentObject.value().at("type").get_to(typeString);
            json params = componentObject.value().at("params");
            std::cout << typeString << " : " << params << std::endl;

            auto componentMapIt = ComponentMap.find(typeString);
            if (componentMapIt == ComponentMap.end())
            {
                std::cout << "ERROR: " << typeString << " not valid known Component" << std::endl;
                std::cout << "Check if the component is a valid listed component or is missing from component types" << std::endl;
                continue;
            }
            ComponentType type = componentMapIt->second;
            switch (type)
            {
                case ComponentType::Camera:
                {
                    // TODO: Check for missing fields
                    CameraComponent* cameraComponent = new CameraComponent(pEntity);
                    pEntity->addComponent(*cameraComponent);
                    params.at("fieldOfView").get_to(cameraComponent->fieldOfView);
                    params.at("farClipPlane").get_to(cameraComponent->farClipPlane);
                    params.at("nearClipPlane").get_to(cameraComponent->nearClipPlane);
                    break;
                }
                case ComponentType::Light:
                {
                    LightComponent* lightComponent = new LightComponent(pEntity);
                    pEntity->addComponent(*lightComponent);

                    LightParams lightParams;
                    std::vector<float> color;
                    params.at("color").get_to(color);
                    lightParams.color = (color.size() > 0) ? glm::vec4(color[0], color[1], color[2], color[3]) : glm::vec4(1.f);

                    std::string lightTypeString;
                    params.at("type").get_to(lightTypeString);
                    auto lightMapIt = LightTypeMap.find(lightTypeString);
                    if (lightMapIt == LightTypeMap.end())
                    {
                        std::cout << "ERROR: " << lightTypeString << " not valid LightType" << std::endl;
                    }
                    lightParams.type = lightMapIt->second;

                    params.at("intensity").get_to(lightParams.intensity);
                    params.at("constant").get_to(lightParams.constant);
                    params.at("linear").get_to(lightParams.linear);
                    params.at("quadratic").get_to(lightParams.quadratic);
                    params.at("cutoff").get_to(lightParams.cutoff);
                    params.at("outercutoff").get_to(lightParams.outercutoff);

                    lightComponent->SetParams(lightParams);
                }
                break;
                case ComponentType::Script:
                    break;
                default:
                    break;
            }
        }
    }

    void ParseAssets(json& pAssetObjects)
    {
        for (auto& asset : pAssetObjects)
        {
            try
            {
                auto folder = asset.at("folder");
                auto file = asset.at("file");

                Entity* entity = new Entity();
                GLTFLoader::LoadModelAsEntity(entity, folder, file);
                EntityManager::getInstance().Instantiate(entity);
            }
            catch (const json::out_of_range& e)
            {
                std::cout << e.what() << '\n';
                std::cout << asset << '\n';
            }
        }
    }

    void ParseEntities(json& pEntityObjects)
    {
        for (auto& entityElement : pEntityObjects)
        {
            Entity* entity = nullptr;
            try
            {
                std::string name = entityElement.at("name").template get<std::string>();
                entity = EntityManager::getInstance().findFirstEntityByDisplayName(name);
                if (entity == nullptr)
                {
                    entity = new Entity(name);
                    EntityManager::getInstance().Instantiate(entity);
                }

            }
            catch (const json::out_of_range& e)
            {
                std::cout << e.what() << '\n';
                entity = new Entity();
                EntityManager::getInstance().Instantiate(entity);
            }
            try
            {
                // Parse Transform
                json transformObject = entityElement.at("transform");
                ParseTransform(entity, transformObject);

            }
            catch (const json::out_of_range& e)
            {
                std::cout << e.what() << '\n';
            }
            try
            {
                // Parse Components
                json components = entityElement.at("components");
                getComponents(entity, components);
            }
            catch (const json::out_of_range& e)
            {
                std::cout << e.what() << '\n';
            }
        }
    }

    static void LoadScene(std::string pSceneFilePath)
    {
        std::ifstream inputStream;
        inputStream.open(pSceneFilePath);
        if (!inputStream.is_open())
        {
            std::cout << "ERROR: " << pSceneFilePath << " does not exist!" << '\n';
            return;
        }

        json sceneJSON;
        inputStream >> sceneJSON;
        try
        {
            json assetObjects = sceneJSON.at(ASSET_KEY);
            ParseAssets(assetObjects);
        }
        catch (const json::out_of_range& e)
        {
            std::cout << e.what() << '\n';
        }

        try
        {
            json entitiesObject = sceneJSON.at(ENTITY_KEY);
            ParseEntities(entitiesObject);
        }
        catch (const json::out_of_range& e)
        {
            std::cout << e.what() << '\n';
        }
    }
}