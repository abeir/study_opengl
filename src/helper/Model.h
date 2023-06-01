#ifndef STUDY_OPENGL_MODEL_H
#define STUDY_OPENGL_MODEL_H

#include "shader/Shader.h"
#include "Mesh.h"
#include <vector>
#include <string>


struct aiScene;
struct aiNode;
struct aiMesh;
struct aiMaterial;


class Model {
public:
    explicit Model(const char* fullFilePath);
    explicit Model(std::string fullFilePath);

    bool load();

    void draw(Shader &shader);

    inline std::vector<Mesh>& meshes() { return meshes_; }
    inline std::vector<Texture>& textures() { return texturesLoaded_; }

    inline bool hasError() { return !error_.empty(); }
    inline std::string& error() { return error_; }
private:
    inline void setError(const char* err) { error_ = err; }
    inline void setError(const std::string &err) { error_ = err; }

    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, Texture::TextureType textureType);

    bool isTextureLoaded(const char* path);

    std::vector<Mesh> meshes_;
    std::string filePath_;
    std::string dir_;
    std::string error_;
    std::vector<Texture> texturesLoaded_;
};


#endif //STUDY_OPENGL_MODEL_H
