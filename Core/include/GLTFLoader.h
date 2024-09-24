#pragma once

#include <tiny_gltf.h>

namespace GLTFLoader
{
    /* Loads GLTF model from file. Returns tinygltf::Model object
    * .gltf files are effectively stored as json
    * 
    * https://github.com/KhronosGroup/glTF-Tutorials/blob/main/gltfTutorial/gltfTutorial_004_ScenesNodes.md
    * GLTF format contains the following key items:
    *   - Scenes
    *   - Nodes
    *       - Transforms
    *   - Meshes
    * 
    * Scenes are structured in a tree with a single root node and any amount of children per node.
    * Each node has its own transform that needs to be taken into consideration.
    */
    static tinygltf::Model LoadStaticModelFromFile(std::string const& pFilePath, bool pIsBinary)
    {
        tinygltf::Model gltfModel;
        tinygltf::TinyGLTF parser;
        std::string loaderErrors;
        std::string loaderWarnings;
        bool result = false;


        result = pIsBinary ? parser.LoadBinaryFromFile(&gltfModel, &loaderErrors, &loaderWarnings, pFilePath) : parser.LoadASCIIFromFile(&gltfModel, &loaderErrors, &loaderWarnings, pFilePath);

        if (!loaderWarnings.empty())
        {
            printf("%s: warnings while loading glTF model:\n % s\n",
                __FUNCTION__, loaderWarnings.c_str());
        }
        if (!loaderErrors.empty())
        {
            printf("%s: errors while loading glTF model:\n % s\n",
                __FUNCTION__, loaderErrors.c_str());
        }
        if (!result)
        {
            printf("%s error: could not load file '%s'\n",
                __FUNCTION__, pFilePath.c_str());
        }
        if (gltfModel.scenes.size() != 1)
        {
            printf("%s error: GLTF model file must contain only one scene '%s'\n",
                __FUNCTION__, pFilePath.c_str());
            return {};
        }
        // return the processed model
        return gltfModel;
    }
}