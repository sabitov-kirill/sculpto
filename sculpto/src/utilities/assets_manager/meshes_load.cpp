/*****************************************************************//**
 * \file   models_load.h
 * \brief  Assets manager model load functions implementation modulule.
 *
 * \author Sabitov Kirill
 * \date   09 July 2022
 *********************************************************************/

#include "sclpch.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "meshes_load.h"
#include "shaders_load.h"
#include "textures_load.h"
#include "core/resources/mesh.h"
#include "core/resources/vertex.h"
#include "core/resources/materials/material_phong.h"
#include "core/resources/topology/trimesh.h"
#include "core/render/render_context.h"

void scl::assets_manager::mesh_loader_phong::ProcessNode(aiNode *Node)
{
    // Process all meshes of current node.
    for (u32 i = 0; i < Node->mNumMeshes; i++)
    {
        aiMesh *mesh = Scene->mMeshes[Node->mMeshes[i]];
        submesh_props<scl::vertex> generating_submesh_props;
        GenerateSubmesh(mesh, generating_submesh_props);
        OutSubmeshes.push_back(generating_submesh_props);
    }

    // Run processing of all child nodes
    for (u32 i = 0; i < Node->mNumChildren; i++)
        ProcessNode(Node->mChildren[i]);
}

void scl::assets_manager::mesh_loader_phong::GenerateSubmesh(aiMesh *Mesh, scl::submesh_props<scl::vertex> &OutSubmeshProps)
{
    OutSubmeshProps.Topology.Vertices.resize(Mesh->mNumVertices);
    for (u32 i = 0; i < Mesh->mNumVertices; i++)
    {
        vertex v;
        v.Position  = { Mesh->mVertices[i].x,   Mesh->mVertices[i].y,   Mesh->mVertices[i].z };
        v.TexCoords = Mesh->mTextureCoords[0] ? vec2 { Mesh->mTextureCoords[0][i].x, Mesh->mTextureCoords[0][i].y } : vec2 {};
        OutSubmeshProps.Topology.Vertices[i] = v;
    }
    
    for (u32 i = 0; i < Mesh->mNumFaces; i++)
    {
        const aiFace &face = Mesh->mFaces[i];
        for (u32 j = 0; j < face.mNumIndices; j++)
            OutSubmeshProps.Topology.Indices.push_back(face.mIndices[j]);
    }

    OutSubmeshProps.Topology.EvaluateNormals();
    OutSubmeshProps.Topology.EvaluateTangentSpace();
    OutSubmeshProps.Material = GenerateSubmeshMaterial(Mesh);
}

scl::shared<scl::material_phong> scl::assets_manager::mesh_loader_phong::GenerateSubmeshMaterial(aiMesh *Mesh)
{
    shared<material_phong> mat = material_phong::Create(vec3 { 0.4 }, vec3 { 0 }, 1);
    if (Mesh->mMaterialIndex < 0) return mat;

    aiMaterial *ai_mat = Scene->mMaterials[Mesh->mMaterialIndex];
    aiString path;
    if (ai_mat->GetTextureCount(aiTextureType_DIFFUSE) > 0)
    {
        ai_mat->GetTexture(aiTextureType_DIFFUSE, 0, &path);
        mat->SetDiffuseMapTexture(assets_manager::LoadTexture(DirectoryPath + '/' + std::string(path.C_Str())));
    }
    if (ai_mat->GetTextureCount(aiTextureType_SPECULAR) > 0)
    {
        ai_mat->GetTexture(aiTextureType_SPECULAR, 0, &path);
        mat->SetSpecularMapTexture(assets_manager::LoadTexture(DirectoryPath + '/' + std::string(path.C_Str())));
    }
    if (ai_mat->GetTextureCount(aiTextureType_EMISSIVE) > 0)
    {
        ai_mat->GetTexture(aiTextureType_EMISSIVE, 0, &path);
        mat->SetEmissionMapTexture(assets_manager::LoadTexture(DirectoryPath + '/' + std::string(path.C_Str())));
    }
    if (ai_mat->GetTextureCount(aiTextureType_NORMALS) > 0)
    {
        ai_mat->GetTexture(aiTextureType_NORMALS, 0, &path);
        mat->SetNormalMapTexture(assets_manager::LoadTexture(DirectoryPath + '/' + std::string(path.C_Str())));
    }

    return mat;
}

scl::assets_manager::mesh_loader_phong::mesh_loader_phong(const aiScene *Scene,
                                                          const std::string &DirectoryPath,
                                                          std::vector<submesh_props<vertex>> &OutSubmeshes) :
    Scene(Scene), DirectoryPath(DirectoryPath), OutSubmeshes(OutSubmeshes) {}

scl::shared<scl::mesh> scl::assets_manager::LoadMeshes(const std::filesystem::path &ModelFilePath)
{
    SCL_CORE_INFO("Mesh creation from file \"{}\" started (this may take time).", ModelFilePath.string());

    u32 flags = aiProcess_Triangulate
        | aiProcess_OptimizeMeshes
        | aiProcess_GenNormals
        | aiProcess_CalcTangentSpace;
    Assimp::Importer importer {};
    const aiScene *scene = importer.ReadFile(ModelFilePath.string(), flags);
    SCL_CORE_ASSERT(scene && (!scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) && scene->mRootNode,
                    "Error while loading model: {}", importer.GetErrorString());

    std::vector<submesh_props<vertex>> out_submeshes;
    mesh_loader_phong mesh_loader(scene, ModelFilePath.parent_path().string(), out_submeshes);
    mesh_loader.ProcessNode(scene->mRootNode);

    return mesh::Create(out_submeshes);
}
