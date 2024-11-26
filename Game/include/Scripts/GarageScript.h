#pragma once

#include <glad/glad.h>
#include "Script.h"
#include "Input.h"

#include <functional>

class OrbitScript;

struct GarageScriptParams : ScriptParams
{
    OrbitScript* orbitScript = nullptr;
    bool carHasBeenSelected = false;
    TransformComponent* selectedTarget = nullptr;
    std::vector<Entity*> cars = std::vector<Entity*>();
    int currSelectIndex = 0;

    std::function<void(Input::ActionType)> orbitKeyCallback;
    std::function<void(Input::ActionType)> selectionKeyCallback;
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

    // key callback bindings
    void bindOrbitKey();
    void bindSelectionKey();
    void unbindAllControls();

    void SelectNextCar(); // selects next car in meatspin (only one-way through list)
    void ChooseCar(); // prompts currently selected car for race
    void leaveGarage(); // transport car to track
private:
    GarageScriptParams mParams{};

    
};