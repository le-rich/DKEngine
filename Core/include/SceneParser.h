#pragma once

#include "GLTFLoader.h"

#include <json.hpp>
#include <iostream>
#include <fstream>
#include <map>

namespace SceneParser
{
    using json = nlohmann::json;
    enum SceneElement
    {
        assets,
        entities
    };

    std::unordered_map<std::string, SceneElement> const ElementMap = { {"assets", SceneElement::assets}, {"entities", SceneElement::entities} };

    static void LoadScene(std::string pSceneFilePath)
    {
        std::ifstream i;
        i.open(pSceneFilePath);
        json j;
        i >> j;
        // Parse Assets
        // Parse Entities
        //j.parse();
        //for (json::iterator element = j.begin(); )
        //{
        //    printf(element.type_name());
        //    //ElementMap.find(element.first())
        //}
        for (auto& element : j.items())
        {
            auto it = ElementMap.find(element.key());
            if (it == ElementMap.end())
            {
                printf("ERROR: %s not valid Scene Element\n", element.key().c_str());
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
                    std::cout << entityElement.key() << " : " << entityElement.value() << "\n";
                    auto name = entityElement.value().find("name");
                    auto transform = entityElement.value().find("transform");
                    auto components = entityElement.value().find("components");

                    Entity* entity = EntityManager::getInstance().findFirstEntityByDisplayName(name.value());
                    if (entity == nullptr) entity = new Entity(name.value());
                    // Parse Transform
                    // Parse Components
                    EntityManager::getInstance().Instantiate(entity);
                }
                break;
            }
        }
    }
}