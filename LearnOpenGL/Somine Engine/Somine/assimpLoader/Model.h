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
    void draw(const Shader* shader);
    void draw();
    void addMesh(const Mesh& mesh);
    Transform* transform;
    virtual void setShaderRef(const Shader* shader);
    virtual ~Model();
    const Material& GetMaterial();
protected:
    const Shader* m_shader; 
    vector<Mesh> m_meshes;
    std::vector<Material> m_materials;
    unsigned int textureFromFile(const char* c_str, const string& string);

private:
    vector<Texture> m_textures_loaded;
    string m_directory;
    void loadModel(string const& path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
};
