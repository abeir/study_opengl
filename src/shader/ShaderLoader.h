#ifndef STUDY_OPENGL_SHADERLOADER_H
#define STUDY_OPENGL_SHADERLOADER_H

#include <string>

class ShaderLoader {
public:
    bool load(const char* filePath);
    bool loadInSrc(const char* filePath);

    inline std::string& get() { return content_; }
private:
    static bool exists(const char* filePath);

    std::string content_;
};


#endif //STUDY_OPENGL_SHADERLOADER_H
