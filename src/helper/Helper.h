#ifndef STUDY_OPENGL_HELPER_H
#define STUDY_OPENGL_HELPER_H

#include <string>
#include <glm/glm.hpp>

class Helper {
public:
    static std::string inSrc(const char* path);
    static std::string inRes(const char* name);

    static std::string joinPath(const std::string &path1, const std::string &path2);

    static unsigned int loadTextureFromFile(const char* fullPath, bool flipV = false);

    static float* boxVertices();
    static long long boxVerticesSize();
    static int boxVerticesStride();
    static long long boxVerticesTexCoordOffset();
    static long long boxVerticesNormalOffset();

    static glm::vec3* boxPositions();
    static int boxPositionLength();
private:
    static float boxVertices_[];
    static glm::vec3 boxPositions_[];
};


#endif //STUDY_OPENGL_HELPER_H
