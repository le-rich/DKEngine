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
        assets,
        entities
    };

    std::unordered_map<std::string, SceneElement> const ElementMap = {
        {"assets", SceneElement::assets},
        {"entities", SceneElement::entities}
    };

    Transform getTransform(json& transformObject)
    {
        Transform transform;
        std::vector<float> localPosition;
        transformObject.at("localPosition").get_to(localPosition);
        transform.localPosition = (localPosition.size() > 0) ? glm::vec3(localPosition[0], localPosition[1], localPosition[2]) : glm::vec3();

        std::vector<float> localOrientation;
        transformObject.at("localOrientation").get_to(localOrientation);
        transform.localOrientation = (localOrientation.size() > 0) ? glm::quat(localOrientation[0], localOrientation[1], localOrientation[2], localOrientation[3]) : glm::quat(1.f, 0, 0, 0);

        std::vector<float> localScale;
        transformObject.at("localScale").get_to(localScale);
        transform.localScale = (localScale.size() > 0) ? glm::vec3(localScale[0], localScale[1], localScale[2]) : glm::vec3(1.f);

        return transform;
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
                
            }
            break;
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

    static void LoadScene(std::string pSceneFilePath)
    {
        std::ifstream i;
        i.open(pSceneFilePath);
        json j;
        i >> j;
        for (auto& element : j.items())
        {
            auto it = ElementMap.find(element.key());
            if (it == ElementMap.end())
            {
                std::cout << "ERROR: " << element.key() << " not valid Scene Element" << std::endl;
                continue;
            }
            switch (it->second)
            {
            case SceneElement::assets:
                for (auto& asset : element.value().items())
                {
                    auto folder = asset.value().find("folder");
                    auto file = asset.value().find("file");

                    Entity* entity = new Entity();
                    GLTFLoader::LoadModelAsEntity(entity, folder.value(), file.value());
                    EntityManager::getInstance().Instantiate(entity);
                }
                break;
            case SceneElement::entities:
                for (auto& entityElement : element.value().items())
                {
                    std::string name;
                    entityElement.value().at("name").get_to(name);
                    json transformObject = entityElement.value().at("transform");
                    json components = entityElement.value().at("components");

                    Entity* entity = EntityManager::getInstance().findFirstEntityByDisplayName(name);
                    if (entity == nullptr)
                    {
                        entity = new Entity(name);
                        EntityManager::getInstance().Instantiate(entity);
                    }

                    // Parse Transform
                    Transform transform = getTransform(transformObject);
                    entity->transform->setTransform(transform);

                    // Parse Components
                    getComponents(entity, components);

                }
                break;
            }
        }
    }
}