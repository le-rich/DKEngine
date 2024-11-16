#pragma once
#include "Script.h"

#include <vector>
#include <memory>


class Component;
class Entity;

class ScriptComponent : public Component
{
public:

    ScriptComponent(Entity* mEntity);
    ~ScriptComponent();
    ScriptComponent(const ScriptComponent& other);

    Component* clone() const override;

    template<typename T, typename...Args>
    void AddScript(Args&&... args)
    {
        scripts.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
    }

    template<typename T>
    void CreateAndAddScript(ScriptParams* pScriptParams)
    {
        T* script;
        script = new T(entity);
        script->SetParameters(pScriptParams);
        this->AddScript<T>(*script);
    }

    void UpdateScripts(float deltaTime)
    {
        for (auto& script : scripts)
        {
            script->Update(deltaTime);
        }
    }

private:
    std::vector<std::unique_ptr<Script>> scripts;
};
