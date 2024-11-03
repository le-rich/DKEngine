#include "Scene.h"

#include "Entities/CameraEntity.h"
#include "GLTFLoader.h"
#include "Managers/EntityManager.h"
#include "Components/MeshComponent.h"

Scene::Scene()
{
    sceneRoot = new Entity("root");
    EntityManager::getInstance().addEntityToMap(*sceneRoot);
}

Scene::~Scene()
{

}

void Scene::SpawnSceneDefinition()
{
    CameraEntity* cameraEntity = new CameraEntity();
    cameraEntity->SetDisplayName("Main Camera");
    EntityManager::getInstance().Instantiate(cameraEntity);

    sceneRoot->addChild(cameraEntity);

    // TODO: Refactor to a Single Car Entity with Mesh and Rigidbody components

    std::string SOURCE_FOLDER = "Assets/TestAE/"; // TODO: JSONparser for list of assets, each asset has a PATH and FILE string.
    Entity* testCar = new Entity();
    testCar->SetDisplayName("Test Car");

    tinygltf::Model gltfModel = GLTFLoader::LoadFromFile(SOURCE_FOLDER + "ae86.gltf"); // TODO: Figure out location of assets/non code files within solution
    std::vector<UUIDv4::UUID> textures = GLTFLoader::LoadTextures(gltfModel, SOURCE_FOLDER);
    std::vector<UUIDv4::UUID> materials = GLTFLoader::LoadMaterials(gltfModel, textures);
    Mesh* testMesh = GLTFLoader::LoadMesh(gltfModel, gltfModel.meshes[0], materials);

    MeshComponent* meshComponent = new MeshComponent(testCar);
    meshComponent->setMesh(testMesh);

    testCar->addComponent(*meshComponent);

    // end TODO

    EntityManager::getInstance().Instantiate(testCar);
}
