#include "Model.h"

#include <iostream>
#include <__msvc_filebuf.hpp>
#include "../Macros.h"
#include "../stb_image.h"

Model::Model(std::string const& path)
{
    transform = NEW(Transform);
    loadModel(path);
}

Model::Model()
{
    transform = NEW(Transform);
}

Model::~Model()
{
    DELETE(transform, Transform);
}

Material& Model::GetMaterial() //placeholder 
{
    for (auto& m_material : m_materials)
    {
        if (m_material.ambient_color != glm::vec3(0.0f, 0.0f, 0.0f) &&
            m_material.diffuse_color != glm::vec3(0.0f, 0.0f, 0.0f))
            return m_material;
    }
    Material new_mat = Material();
    new_mat.ambient_color = glm::vec3(1.0f, 1.0f, 1.0f);
    new_mat.diffuse_color = glm::vec3(1.0f, 1.0f, 1.0f);
    m_materials.push_back(new_mat);
    return new_mat;
}


void Model::draw(const Shader* shader)
{
    shader->setUniformMatrix4fv("model", transform->getModelMatrix());
    for (GLuint i = 0; i < m_meshes.size(); i++)
        m_meshes[i].draw(shader);
}

void Model::draw()
{
    m_shader->use();
    m_shader->setUniformMatrix4fv("model", transform->getModelMatrix());
    for (GLuint i = 0; i < m_meshes.size(); i++)
        m_meshes[i].draw(m_shader);
}

void Model::addMesh(const Mesh& mesh)
{
    m_meshes.push_back(mesh);
}

void Model::setShaderRef(const Shader* shader)
{
    m_shader = shader;
    Material mat = GetMaterial();
    m_shader->setMaterial("material", mat);
}

void Model::loadModel(std::string const& path)
{
    Assimp::Importer importer;
    stbi_set_flip_vertically_on_load(false);
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }
    m_directory = path.substr(0, path.find_last_of('/'));
    m_materials.resize(scene->mNumMaterials);
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    for (GLuint i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        m_meshes.push_back(processMesh(mesh, scene));
    }
    for (int i = 0; i < scene->mNumMaterials; ++i)
    {
        aiMaterial* material = scene->mMaterials[i];
        aiColor3D ambient_color(0.0f, 0.0f, 0.0f);
        if (material->Get(AI_MATKEY_COLOR_AMBIENT, ambient_color) == AI_SUCCESS)
        {
            m_materials[i].ambient_color.r = ambient_color.r;
            m_materials[i].ambient_color.g = ambient_color.g;
            m_materials[i].ambient_color.b = ambient_color.b;
        }
        aiColor3D diffuse_color(0.0f, 0.0f, 0.0f);
        if (material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse_color) == AI_SUCCESS)
        {
            m_materials[i].diffuse_color.r = diffuse_color.r;
            m_materials[i].diffuse_color.g = diffuse_color.g;
            m_materials[i].diffuse_color.b = diffuse_color.b;
        }
        aiColor3D specular_color(0.0f, 0.0f, 0.0f);
        if (material->Get(AI_MATKEY_COLOR_SPECULAR, specular_color) == AI_SUCCESS)
        {
            m_materials[i].specular_color.r = specular_color.r;
            m_materials[i].specular_color.g = specular_color.g;
            m_materials[i].specular_color.b = specular_color.b;
        }
    }
    for (GLuint i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;

    for (GLuint i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector;
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.position = vector;
        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.normal = vector;
        }
        if (mesh->mTextureCoords[0])
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.tex_coords = vec;
        }
        else
            vertex.tex_coords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }
    for (GLuint i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        /*
         * Texture naming convention in the shaders assumed:
         * diffuse: texture_diffuseN
         * specular: texture_specularN
         * normal: texture_normalN
         * assuming N == seq. number from 1 to max sampler number
         */

        std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        std::vector<Texture> shininessMaps = loadMaterialTextures(material, aiTextureType_SHININESS,
                                                                  "specular_exponent");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
    }
    return Mesh(vertices, indices, textures);
}


std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
    std::vector<Texture> textures;
    // std::cout << mat->GetTextureCount(type) << typeName << endl;

    for (GLuint i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);

        bool skipIfLoadedAlready = false;
        for (GLuint j = 0; j < m_textures_loaded.size(); j++)
        {
            if (std::strcmp(m_textures_loaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(m_textures_loaded[j]);
                skipIfLoadedAlready = true;
                break;
            }
        }
        if (!skipIfLoadedAlready)
        {
            // std::cout << str.C_Str() << endl;
            Texture texture;
            texture.id = textureFromFile(str.C_Str(), this->m_directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            m_textures_loaded.push_back(texture);
        }
    }

    return textures;
}

unsigned Model::textureFromFile(const char* path, const std::string& directory)
{
    std::string filename = std::string(path);
    //sometimes the path will be absolute based on the pc that was made the fbx. To fix it:
    bool isAbsolute = (filename.find(":") != std::string::npos) || (filename[0] == '/') || (filename[0] == '\\');
    if (isAbsolute)
    {
        // If it's an absolute path, strip it to just the filename (after the last '/' or '\\')
        size_t pos = filename.find_last_of("/\\");
        if (pos != std::string::npos)
        {
            filename = filename.substr(pos + 1); // Extract the file name only
        }
    }

    std::string fullPath = directory + '/' + filename;
    std::string textureSubdirPath = directory + "/textures/" + filename;

    GLuint textureID;
    glGenTextures(1, &textureID);

    int width, height, nr_components;
    unsigned char* data = stbi_load(fullPath.c_str(), &width, &height, &nr_components, 0);

    if (!data)
    {
        data = stbi_load(textureSubdirPath.c_str(), &width, &height, &nr_components, 0);
        if (!data)
        {
            std::cout << "Texture failed to load at path: " << path << std::endl;
            stbi_image_free(data);
            return textureID; // Return an invalid texture ID
        }
    }

    if (data)
    {
        GLenum format;
        if (nr_components == 1)
            format = GL_RED;
        else if (nr_components == 3)
            format = GL_RGB;
        else if (nr_components == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
