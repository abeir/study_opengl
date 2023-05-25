#include "ShaderLoader.h"
#include "helper/Helper.h"
#include <fstream>
#include <unistd.h>
#include <sstream>

bool ShaderLoader::load(const char *filePath) {
    if(!exists(filePath)){
        return false;
    }

    std::stringstream content;

    std::ifstream f(filePath);
    content << f.rdbuf();
    content_ = content.str();
    return true;
}

bool ShaderLoader::loadInSrc(const char *filePath) {
    std::string full = Helper::inSrc(filePath);
    return load(full.c_str());
}

bool ShaderLoader::exists(const char *filePath) {
    return filePath && access(filePath, F_OK) != -1;
}

