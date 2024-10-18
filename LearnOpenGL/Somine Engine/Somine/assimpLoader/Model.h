#pragma once
#include "assimp/Importer.hpp"
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../Transform.h"
using namespace std;
#include "Mesh.h"
#include "../Shader.h"



class Model
{
public:
    Model(std::string const &path);
    Model();
    void draw(Shader& shader);
    void draw();
    void addMesh(const Mesh& mesh);
    Transform transform;
    void setShaderRef(Shader* shader);
    Shader* ref_shader;
protected:
    vector<Mesh> m_meshes;

private:
    vector<Texture> m_textures_loaded;
    string m_directory;

    void loadModel(string const& path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    unsigned int textureFromFile(const char* c_str, const string& string);
    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
};
