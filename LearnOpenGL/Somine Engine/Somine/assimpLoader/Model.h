#pragma once
#include "assimp/Importer.hpp"
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "../Shader.h"

class Model
{
public:
    Model(std::string const &path, bool gammaCorrection=false);
    void draw(Shader& shader);

private:
    std::vector<Mesh> m_meshes;
   std:: vector<Texture> m_textures_loaded;
    std::string m_directory;
    bool m_gamma_correction;
    
    void loadModel(std::string const& path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    unsigned int textureFromFile(const char* c_str, const std::string& string);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};
