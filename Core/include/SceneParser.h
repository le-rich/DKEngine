#pragma once

#include "GLTFLoader.h"
#include "Components/CameraComponent.h"
#include "Components/LightComponent.h"
#include "Components/ScriptComponent.h"
#include "Managers/EntityManager.h"
#include "Scripts/OrbitScript.h"
#include "Scripts/TimerScript.h"
#include "Scripts/LapCheckpointScript.h"
#include "Scripts/LapManagerScript.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <json.hpp>
#include <fstream>
#include <iostream>
#include <unordered_map>

#define REQUIRED_EXCEPTION_HANDLER(object) catch (std::exception& e) { std::cout << "ERROR: " << e.what() << '\n'; std::cout << "Parsing error at line " << __LINE__ << " in file " << __FILE__ << '\n'; std::cout << object << '\n'; return;}
#define DEFAULT_EXCEPTION_HANDLER catch (std::exception& e) { std::cout << "WARNING: " << e.what() << '\n'; std::cout << "Verify naming of keys is correct \n";}
#define OPTIONAL_EXCEPTION_HANDLER catch (std::exception& e) {}

#define IF_COMPONENT_TYPE_NOT_FOUND if (componentMapIt == ComponentMap.end()) { std::cout << "ERROR: " << typeString << " not valid known Component" << std::endl; std::cout << "Check if the component is a valid listed component or is missing from component types" << std::endl; continue; }
#define IF_SCRIPT_TYPE_NOT_FOUND if (scriptTypeIt == ScriptMap.end()) { std::cout << "ERROR: " << typeString << " not valid known Script" << std::endl; std::cout << "Check if the script is a valid listed script or is missing from script types" << std::endl; continue; }
#define IF_SCENE_FILE_NOT_FOUND if (!inputStream.is_open()) { std::cout << "ERROR: " << pSceneFilePath << " does not exist!" << '\n'; return; }

namespace SceneParser
{
    using json = nlohmann::json;
    enum SceneElement
    {
        ASSETS = 0,
        SKYBOX,
        ENTITIES,
        GAMEMANAGER
    };

    static const std::string ASSET_KEY = "assets";
    static const std::string SKYBOX_KEY = "skybox";
    static const std::string ENTITY_KEY = "entities";
    static const std::string GAMEMANAGER_KEY = "gamemanager";

    std::unordered_map<std::string, SceneElement> const ElementMap = {
        {ASSET_KEY, SceneElement::ASSETS},
        {SKYBOX_KEY, SceneElement::SKYBOX},
        {ENTITY_KEY, SceneElement::ENTITIES},
        {GAMEMANAGER_KEY, SceneElement::GAMEMANAGER}
    };

    void ParseTransform(Entity* pEntity, json& transformObject)
    {
        Transform transform;
        try
        {
            std::vector<float> localPosition;
            transformObject.at("localPosition").get_to(localPosition);
            transform.localPosition = (localPosition.size() > 0) ? glm::vec3(localPosition[0], localPosition[1], localPosition[2]) : glm::vec3();
        } OPTIONAL_EXCEPTION_HANDLER;

        try
        {
            std::vector<float> localOrientation;
            transformObject.at("localOrientation").get_to(localOrientation);
            transform.localOrientation = (localOrientation.size() > 0) ? glm::quat(localOrientation[0], localOrientation[1], localOrientation[2], localOrientation[3]) : glm::quat(1.f, 0, 0, 0);
        } OPTIONAL_EXCEPTION_HANDLER;

        try
        {
            std::vector<float> localScale;
            transformObject.at("localScale").get_to(localScale);
            transform.localScale = (localScale.size() > 0) ? glm::vec3(localScale[0], localScale[1], localScale[2]) : glm::vec3(1.f);
        } OPTIONAL_EXCEPTION_HANDLER;

        pEntity->transform->setTransform(transform);
    }

    void ParseCamera(Entity* pEntity, json pCameraParams)
    {
        CameraComponent* cameraComponent = new CameraComponent(pEntity);
        pEntity->addComponent(*cameraComponent);

        try
        {
            pCameraParams.at("fieldOfView").get_to(cameraComponent->fieldOfView);
        } OPTIONAL_EXCEPTION_HANDLER;

        try
        {
            pCameraParams.at("farClipPlane").get_to(cameraComponent->farClipPlane);
        } OPTIONAL_EXCEPTION_HANDLER;

        try
        {
            pCameraParams.at("nearClipPlane").get_to(cameraComponent->nearClipPlane);
        } OPTIONAL_EXCEPTION_HANDLER;
    }

    void ParseLight(Entity* pEntity, json pLightParams)
    {
        LightParams lightParams;
        try
        {
            std::vector<float> color;
            pLightParams.at("color").get_to(color);
            lightParams.color = (color.size() > 0) ? glm::vec4(color[0], color[1], color[2], color[3]) : glm::vec4(1.f);
        } OPTIONAL_EXCEPTION_HANDLER;

        std::string lightTypeString;
        try
        {
            pLightParams.at("type").get_to(lightTypeString);
            auto lightMapIt = LightTypeMap.find(lightTypeString);
            if (lightMapIt != LightTypeMap.end())
            {
                lightParams.type = lightMapIt->second;
            }
            else
            {
                std::cout << "ERROR: " << lightTypeString << " not valid LightType" << std::endl;
            }
        } OPTIONAL_EXCEPTION_HANDLER;

        try
        {
            pLightParams.at("intensity").get_to(lightParams.intensity);
        } OPTIONAL_EXCEPTION_HANDLER;

        try
        {
            pLightParams.at("constant").get_to(lightParams.constant);
        } OPTIONAL_EXCEPTION_HANDLER;

        try
        {
            pLightParams.at("linear").get_to(lightParams.linear);
        } OPTIONAL_EXCEPTION_HANDLER;

        try
        {
            pLightParams.at("quadratic").get_to(lightParams.quadratic);
        } OPTIONAL_EXCEPTION_HANDLER;

        try
        {
            pLightParams.at("cutoff").get_to(lightParams.cutoff);
        } OPTIONAL_EXCEPTION_HANDLER;

        try
        {
            pLightParams.at("outercutoff").get_to(lightParams.outercutoff);
        } OPTIONAL_EXCEPTION_HANDLER;

        try
        {
            pLightParams.at("createsShadows").get_to(lightParams.createsShadows);
        } OPTIONAL_EXCEPTION_HANDLER;

        LightComponent* lightComponent = new LightComponent(pEntity, lightParams);
        pEntity->addComponent(*lightComponent);
        //lightComponent->SetParams(lightParams);
    }

    void ParseScript(Entity* pEntity, json pScriptParams)
    {
        ScriptComponent* scriptComponent = new ScriptComponent(pEntity);
        pEntity->addComponent(*scriptComponent);
        json scripts;
        try
        {
            scripts = pScriptParams.at("scripts");
        } DEFAULT_EXCEPTION_HANDLER;

        for (auto& script : scripts.items())
        {
            try
            {
                const std::string typeString = script.value().at("script").template get<std::string>();
                auto scriptTypeIt = ScriptMap.find(typeString);
                IF_SCRIPT_TYPE_NOT_FOUND;

                json scriptParams = script.value().at("params");
                switch (scriptTypeIt->second)
                {
                    case ScriptType::OrbitScript:
                    {
                        OrbitScriptParams params;

                        try
                        {
                            std::string target = scriptParams.at("target").template get<std::string>();
                            Entity* targetEntity = EntityManager::getInstance().findFirstEntityByDisplayName(target);
                            params.m_OrbitTarget = targetEntity->transform;
                        } OPTIONAL_EXCEPTION_HANDLER;

                        try
                        {
                            scriptParams.at("radius").get_to(params.m_Radius);
                        } OPTIONAL_EXCEPTION_HANDLER;
                        try
                        {
                            scriptParams.at("speed").get_to(params.m_Speed);
                        } OPTIONAL_EXCEPTION_HANDLER;
                        try
                        {
                            scriptParams.at("currentAngle").get_to(params.currentAngle);
                        } OPTIONAL_EXCEPTION_HANDLER;

                        scriptComponent->CreateAndAddScript<OrbitScript>(&params);
                        break;
                    }
                    case ScriptType::TimerScript:
                    {
                        TimerScriptParams params;
                        try
                        {
                            std::string target = scriptParams.at("target").template get<std::string>();
                            Entity* targetEntity = EntityManager::getInstance().findFirstEntityByDisplayName(target);
                            params.m_TimerTarget = targetEntity->transform;
                        } OPTIONAL_EXCEPTION_HANDLER;

                        scriptComponent->CreateAndAddScript<TimerScript>(&params);
                        break;
                    }
                    case ScriptType::LapCheckpointScript:
                    {
                        LapCheckpointScriptParams params;
                        try
                        {
                            std::string target = scriptParams.at("other").template get<std::string>();
                            Entity* targetEntity = EntityManager::getInstance().findFirstEntityByDisplayName(target);
                            params.m_Other = targetEntity->transform;
                        } OPTIONAL_EXCEPTION_HANDLER;

                        try
                        {
                            int index = scriptParams.at("index").template get<int>();
                            params.m_Index = index;
                        } OPTIONAL_EXCEPTION_HANDLER;

                        scriptComponent->CreateAndAddScript<LapCheckpointScript>(&params);
                        break;
                    }
                    case ScriptType::LapManagerScript:
                    {
                        LapManagerScriptParams params;
                        LapManagerScript* lapManagerScript = new LapManagerScript(pEntity);
                        try
                        {
                            std::vector<std::string> checkpoints = scriptParams.at("checkpoints").template get<std::vector<std::string>>();
                            for (std::string checkpoint : checkpoints)
                            {
                                Entity* entity = EntityManager::getInstance().findFirstEntityByDisplayName(checkpoint);
                                ScriptComponent* checkpointScriptComponent = dynamic_cast<ScriptComponent*>(entity->getComponent(ComponentType::Script));
                                LapCheckpointScript* checkpointScript = checkpointScriptComponent->GetScript<LapCheckpointScript>();
                                checkpointScript->SetLapManager(lapManagerScript);
                                params.m_Checkpoints.push_back(checkpointScript);
                            }
                        }OPTIONAL_EXCEPTION_HANDLER;

                        try
                        {
                            int totallaps = scriptParams.at("totallaps").template get<int>();
                            params.m_TotalLaps = totallaps;
                        }OPTIONAL_EXCEPTION_HANDLER;

                        try
                        {
                            int currentlap = scriptParams.at("currentlap").template get<int>();
                            params.m_CurrentLap = currentlap;
                        }OPTIONAL_EXCEPTION_HANDLER;

                        try
                        {
                            int nextcheckpointindex = scriptParams.at("nextcheckpointindex").template get<int>();
                            params.m_NextCheckpointIndex = nextcheckpointindex;
                        }OPTIONAL_EXCEPTION_HANDLER;

                        lapManagerScript->SetParameters(&params);
                        scriptComponent->AddScript<LapManagerScript>(*lapManagerScript);
                        break;
                    }
                    default:
                        break;
                }
            }
            REQUIRED_EXCEPTION_HANDLER(script);
        }
    }

    void ParseComponents(Entity* pEntity, json& pComponentObjects)
    {
        for (auto& componentObject : pComponentObjects.items())
        {
            std::string typeString;
            json params;

            try
            {
                componentObject.value().at("type").get_to(typeString);
                params = componentObject.value().at("params");
            }
            REQUIRED_EXCEPTION_HANDLER(componentObject);

            auto componentMapIt = ComponentMap.find(typeString);
            IF_COMPONENT_TYPE_NOT_FOUND;


            switch (componentMapIt->second)
            {
                case ComponentType::Camera:
                    ParseCamera(pEntity, params);
                    break;

                case ComponentType::Light:
                    ParseLight(pEntity, params);
                    break;
                case ComponentType::Script:
                    ParseScript(pEntity, params);
                    break;
                default:
                    break;
            }
        }
    }

    void ParseSkybox(json& pSkyboxObject)
    {
        // Create Skybox object if not exist
        std::string imageName = pSkyboxObject.template get<std::string>();
        std::shared_ptr<Skybox> skybox = AssetManager::GetInstance().GetSkyboxByName(imageName);
        if (!skybox)
        {
            std::shared_ptr<Texture> texture = std::make_shared<Texture>(DEFAULT_SKYBOX_FOLDER, imageName);
            AssetManager::GetInstance().AddTexture(texture);
            skybox = std::make_shared<Skybox>(imageName);
            skybox->mTextureID = texture->GetAssetID();
            skybox->GenerateTexture(texture);
            skybox->mShaderID = AssetManager::GetInstance().GetSkyboxShader()->GetAssetID();
            AssetManager::GetInstance().AddSkybox(skybox);
        }
        // Assign Skybox to Scene
        Core::getInstance().GetScene()->SetSkyboxID(skybox->GetAssetID());
    }

    void ParseAssets(json& pAssetObjects)
    {
        for (auto& asset : pAssetObjects)
        {
            try
            {
                const std::string path = asset.at("path").template get<std::string>();
                auto files = asset.at("file").template get<std::vector<std::string>>();

                for (const std::string file : files)
                {
                    Entity* entity = new Entity();

                    GLTFLoader::LoadModelAsEntity(entity, path, file);
                    EntityManager::getInstance().Instantiate(entity);
                }
            }
            DEFAULT_EXCEPTION_HANDLER;
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
            REQUIRED_EXCEPTION_HANDLER(entityElement);

            try
            {
                json transformObject = entityElement.at("transform");
                ParseTransform(entity, transformObject);

            }
            OPTIONAL_EXCEPTION_HANDLER;

            try
            {
                std::string parentName = entityElement.at("parent").template get<std::string>();
                Entity* parentEntity = EntityManager::getInstance().findFirstEntityByDisplayName(parentName);
                if (parentEntity == nullptr)
                {
                    parentEntity = new Entity(parentName);
                    EntityManager::getInstance().Instantiate(parentEntity);
                }
                entity->setParent(parentEntity);
                parentEntity->addChild(entity);

            }
            OPTIONAL_EXCEPTION_HANDLER;

            try
            {
                auto children = entityElement.at("children").template get<std::vector<std::string>>();
                for (std::string& childName : children)
                {
                    Entity* childEntity = EntityManager::getInstance().findFirstEntityByDisplayName(childName);
                    if (childEntity == nullptr)
                    {
                        childEntity = new Entity(childName);
                        EntityManager::getInstance().Instantiate(childEntity);
                    }
                    childEntity->setParent(entity);
                    entity->addChild(childEntity);
                }
            }
            OPTIONAL_EXCEPTION_HANDLER;

            try
            {
                json components = entityElement.at("components");
                ParseComponents(entity, components);
            }
            OPTIONAL_EXCEPTION_HANDLER;
        }
    }

    void ParseGameManager(json& pEntityObjects)
    {
        std::string name = "GameMangaer";
        Entity* entity = EntityManager::getInstance().findFirstEntityByDisplayName(name);
        if (entity == nullptr)
        {
            entity = new Entity(name);
            EntityManager::getInstance().Instantiate(entity);
        }

        try
        {
            json components = pEntityObjects.at("components");
            ParseComponents(entity, components);
        }
        OPTIONAL_EXCEPTION_HANDLER;
    }

    static void LoadScene(std::string pSceneFilePath)
    {
        std::ifstream inputStream;
        inputStream.open(pSceneFilePath);
        IF_SCENE_FILE_NOT_FOUND;

        json sceneJSON;
        inputStream >> sceneJSON;
        try
        {
            json assetObjects = sceneJSON.at(ASSET_KEY);
            ParseAssets(assetObjects);
        }
        DEFAULT_EXCEPTION_HANDLER;

        try
        {
            json skyboxObject = sceneJSON.at(SKYBOX_KEY);
            ParseSkybox(skyboxObject);
        }DEFAULT_EXCEPTION_HANDLER;

        try
        {
            json entitiesObject = sceneJSON.at(ENTITY_KEY);
            ParseEntities(entitiesObject);
        }
        DEFAULT_EXCEPTION_HANDLER;

        try
        {
            json gameManagerObject = sceneJSON.at(GAMEMANAGER_KEY);
            ParseGameManager(gameManagerObject);
        }
        DEFAULT_EXCEPTION_HANDLER;
    }
}