#pragma once
#include "Script.h"

class Component;
class Entity;

#include <vector>
#include <memory>

class ScriptComponent : public Component {
public:

    ScriptComponent(Entity* mEntity);
    ~ScriptComponent();

    Component* clone() override;

    template<typename T, typename...Args>
    void AddScript(Args&&... args) {
        scripts.emplace_back(std::make_shared<T>(std::forward<Args>(args)...));
    }

    void UpdateScripts(float deltaTime) {
        for (auto& script: scripts)
        {
            script->Update(deltaTime);
        }
    }

    ScriptComponent& operator=(ScriptComponent& const other);

private:
    std::vector<std::shared_ptr<Script>> scripts;
};
