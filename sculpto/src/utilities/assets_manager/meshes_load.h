/*****************************************************************//**
 * \file   models_load.h
 * \brief  Assets manager model load functions defintion modulule.
 * 
 * \author Sabitov Kirill
 * \date   09 July 2022
 *********************************************************************/

#pragma once

#include "base.h"

/* Classes definition. */
struct aiScene;
struct aiMesh;
struct aiNode;
namespace scl {
    template <typename Tvertex> struct submesh_props;
    class mesh;
    struct vertex;
    class material_phong;
}

namespace scl::assets_manager
{
    /* Mesh for phong lighting model loader class. */
    class mesh_loader_phong
    {
    private:
        const aiScene *Scene;
        std::mutex OutSubmeshesPushMutex;
        std::vector<submesh_props<vertex>> &OutSubmeshes;
        std::string DirectoryPath;

        void GenerateSubmesh(aiMesh *Mesh, submesh_props<vertex> &OutSubmeshProps);
        shared<material_phong> GenerateSubmeshMaterial(aiMesh *Mesh);

    public:
        mesh_loader_phong(const aiScene *Scene, const std::string &DirectoryPath, std::vector<submesh_props<vertex>> &OutSubmeshes);
        void ProcessNode(aiNode *Node);

    };

    /**
     * Load model (all meshes with materials) from file function.
     * 
     * \param ModelFilePath - model file path.
     * \return loaded mesh pointer.
     */
    shared<mesh> LoadMeshes(const std::filesystem::path &ModelFilePath);
}
