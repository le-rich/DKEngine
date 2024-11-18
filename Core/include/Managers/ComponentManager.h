//#pragma once
//
//#include "Component.h"
//#include "Entity.h"
//#include "Components/TransformComponent.h"
//#include "Components/RigidBodyComponent.h"
//#include "Components/CameraComponent.h"
//#include "Components/LightComponent.h"
//#include "Components/MeshComponent.h"
//#include "Components/ScriptComponent.h"
//#include "Script.h"
//#include "Scripts/OrbitScript.h"
//
//
//namespace ComponentManager
//{
//
//    static MeshComponent* AddMeshComponent(Entity* pEntity, Mesh* pMesh)
//    {
//        MeshComponent* meshComponent = new MeshComponent(pEntity);
//        meshComponent->setMesh(pMesh);
//        pEntity->addComponent(*meshComponent);
//        return meshComponent;
//    }
//
//    static CameraComponent* AddCameraComponent(Entity* pEntity)
//    {
//        CameraComponent* cameraComponent = new CameraComponent(pEntity);
//        pEntity->addComponent(*cameraComponent);
//        return cameraComponent;
//    }
//
//    static LightComponent* AddLightComponent(Entity* pEntity)
//    {
//        LightComponent* lightComponent = new LightComponent(pEntity);
//        pEntity->addComponent(*lightComponent);
//        return lightComponent;
//    }    
//
//    static LightComponent* AddLightComponent(Entity* pEntity, LightParams pParams)
//    {
//        LightComponent* lightComponent = new LightComponent(pEntity, pParams);
//        pEntity->addComponent(*lightComponent);
//        return lightComponent;
//    }
//
//    //static ScriptComponent* AddScriptComponent(Entity* pEntity)
//    //{
//    //    ScriptComponent* scriptComponent = new ScriptComponent(pEntity);
//    //    pEntity->addComponent(*scriptComponent);
//    //    return scriptComponent;
//    //}
//};