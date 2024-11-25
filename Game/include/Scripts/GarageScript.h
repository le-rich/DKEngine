#pragma once

#include <glad/glad.h>
#include "Script.h"

class OrbitScript;

struct GarageScriptParams : ScriptParams
{
    OrbitScript* orbitScript = nullptr;
    bool carHasBeenSelected = false;
    TransformComponent* selectedTarget = nullptr;
    std::vector<Entity*> cars = std::vector<Entity*>();
    int currSelectIndex = 0;
};

class GarageScript : public Script
{
public:
    GarageScript(Entity* mEntity);
    ~GarageScript();

    void Update(float deltaTime) override;
    void SetParameters(ScriptParams* pScriptParameters) override;

    std::unique_ptr<Script> clone() const override
    {
        return std::make_unique<GarageScript>(*this);
    }

    // transport car to track
    void leaveGarage();

    // key callback bindings
    void bindOrbitKey();
    void bindSelectionKey();


private:
    GarageScriptParams mParams{};

    void SelectNextCar();
};