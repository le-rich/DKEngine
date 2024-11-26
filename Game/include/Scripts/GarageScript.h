#pragma once

#include "Script.h"
#include "Input.h"

#include <functional>

class OrbitScript;

struct GarageScriptParams : ScriptParams
{
    OrbitScript* mOrbitScript = nullptr;
    bool mIsCarSelected = false;
    TransformComponent* mChosenTarget = nullptr;
    std::vector<Entity*> mCars = std::vector<Entity*>();
    int currSelectIndex = 0;

    std::function<void(Input::ActionType)> mSelectCallback;
    std::function<void(Input::ActionType)> mChooseCallback;
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
    void BindSelectKey();
    void BindChooseKey();
    void UnbindAllControls();

    void SelectNextCar(); // selects next car in meatspin (only one-way through list)
    void ChooseCar(); // prompts currently selected car for race
    void leaveGarage(); // transport car to track
private:
    GarageScriptParams mParams{};
    float unSelectedPosX = -10.0f; // remove later
    
};