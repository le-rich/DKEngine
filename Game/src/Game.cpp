#include "Game.h"

#include "Managers/EntityManager.h"

#include "Components/ScriptComponent.h"

Game::Game() {}

Game::~Game() {}

void Game::Initialize() 
{
    System::Initialize();
}

void Game::Update(float deltaTime) {
    auto scriptComponentUUIDs = EntityManager::getInstance().findEntitiesByComponent(ComponentType::Script);
    
    for (auto uuid : scriptComponentUUIDs) 
    {
         Entity* entity = EntityManager::getInstance().getEntity(uuid);

         if (entity != nullptr) {
            ScriptComponent* scriptComponent = dynamic_cast<ScriptComponent*>(entity->getComponent(ComponentType::Script));

            if (scriptComponent) 
            {
                scriptComponent->UpdateScripts(deltaTime); 
            }
         }
    }
}

void Game::FixedUpdate() 
{

}

