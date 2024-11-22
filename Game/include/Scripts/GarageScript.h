#pragma once

#include "Script.h"
#include "OrbitScript.h"
//#include "Input.h"

struct GarageScriptParams : ScriptParams
{
    //TransformComponent* m_OrbitTarget = nullptr;
    //TransformComponent* m_OrbitTarget2 = nullptr;
    OrbitScript* orbitScript = nullptr;
    bool carHasBeenSelected = false;
    Entity* selectedCar = nullptr;
    //float m_Radius = 2;
    //float m_Speed = 1.5f;
    //float currentAngle = 0;

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
};