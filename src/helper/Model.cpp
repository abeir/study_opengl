#include "Model.h"
#include "Helper.h"
#include <assimp/Importer.hpp>
#include <utility>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


Model::Model(const char *filePath) : filePath_{filePath} {
}

Model::Model(std::string filePath) : filePath_{std::move(filePath)} {
}



bool Model::load() {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(filePath_, aiProcess_Triangulate | aiProcess_FlipUVs);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
        setError(importer.GetErrorString());
        return false;
    }
    dir_ = filePath_.substr(0, filePath_.find_last_of('/'));

    processNode(scene->mRootNode, scene);
    return true;
}


void Model::draw(Shader &shader) {
    for(auto &mesh : meshes_){
        mesh.draw(shader);
    }
}

void Model::processNode(aiNode *node, const aiScene *scene) {
    for(unsigned int i=0; i<node->mNumMeshes; i++){
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes_.emplace_back(processMesh(mesh, scene));
    }
    for(unsigned int i=0; i<node->mNumChildren; i++){
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for(unsigned int i=0; i<mesh->mNumVertices; i++){
        Vertex vertex{};
        vertex.position.x = mesh->mVertices[i].x;
        vertex.position.y = mesh->mVertices[i].y;
        vertex.position.z = mesh->mVertices[i].z;

        vertex.normal.x = mesh->mNormals[i].x;
        vertex.normal.y = mesh->mNormals[i].y;
        vertex.normal.z = mesh->mNormals[i].z;

        if(mesh->mTextureCoords[0]){
            vertex.texCoord.x = mesh->mTextureCoords[0][i].x;
            vertex.texCoord.y = mesh->mTextureCoords[0][i].y;
        }else{
            vertex.texCoord = glm::vec2(0.0f, 0.0f);
        }
        vertices.emplace_back(vertex);
    }

    for(unsigned int i=0; i<mesh->mNumFaces; i++){
        aiFace face = mesh->mFaces[i];
        for(unsigned int j=0; j<face.mNumIndices; j++){
            indices.emplace_back(face.mIndices[j]);
        }
    }

    if(mesh->mMaterialIndex >= 0){
        aiMaterial *mat = scene->mMaterials[mesh->mMaterialIndex];

        std::vector<Texture> diffuses = loadMaterialTextures(mat, Texture::Diffuse);
        textures.insert(textures.end(), diffuses.begin(), diffuses.end());

        std::vector<Texture> speculars = loadMaterialTextures(mat, Texture::Specular);
        textures.insert(textures.end(), speculars.begin(), speculars.end());
    }

    Mesh m(vertices, indices, textures);
    return m;
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, Texture::TextureType textureType) {
    std::vector<Texture> textures;

    aiTextureType type = aiTextureType_NONE;
    if(textureType == Texture::Diffuse){
        type = aiTextureType_DIFFUSE;
    }else if(textureType == Texture::Specular){
        type = aiTextureType_SPECULAR;
    }

    auto count = mat->GetTextureCount(type);
    for(unsigned int i=0; i<count; i++){
        aiString path;
        mat->GetTexture(type, i, &path);
        const char* pathStr = path.C_Str();
        if(isTextureLoaded(pathStr)){
            continue;
        }
        std::string fullPath = Helper::joinPath(dir_, pathStr);

        Texture texture;
        auto textureId = Helper::loadTextureFromFile(fullPath.c_str());
        if(textureId == 0){
            setError(std::string("Failed to load texture from") + fullPath);
            return {};
        }
        texture.type = textureType;
        texture.path = pathStr;
        textures.emplace_back(texture);
        texturesLoaded_.emplace_back(texture);
    }
    return textures;
}

bool Model::isTextureLoaded(const char *path) {
    return std::any_of(texturesLoaded_.begin(), texturesLoaded_.end(), [path](Texture &tex){
        return tex.path == path;
    });
}


