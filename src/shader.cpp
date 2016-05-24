#include "shader.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cstdio>

// load and compile a single shader from sh.file
// sh.id is an existing shader object
// shader type is defined by shader object type
static bool loadShader(const ShaderInfo& sh) {
    // open file
    FILE* fp = std::fopen(sh.file, "rb");
    if (!fp) {
        std::perror(sh.file);
        return false;
    }

    // get file size: seek to end of file is more cross-platform than fstat
    std::fseek(fp, 0, SEEK_END);
    int size = std::ftell(fp);
    std::fseek(fp, 0, SEEK_SET);

    // read entire file and compile as shader
    GLchar* shader = new GLchar[size];
    std::fread(shader, 1, size, fp);
    if (std::ferror(fp)) {
        std::perror(sh.file);
        delete[] shader;
        std::fclose(fp);
        return false;
    }

    std::fclose(fp);
    glShaderSource(sh.id, 1, (const GLchar**)(&shader), &size);
    glCompileShader(sh.id);
    delete[] shader;

    // report compile errors
    GLint success;
    glGetShaderiv(sh.id, GL_COMPILE_STATUS, &success);
    if (!success) {
        // how big is the message?
        GLsizei infoLen;
        glGetShaderiv(sh.id, GL_INFO_LOG_LENGTH, &infoLen);

        // print the message
        char* infoLog = new char[infoLen];
        glGetShaderInfoLog(sh.id, infoLen, 0, infoLog);
        std::fprintf(stderr, "%s:\n%s", sh.file, infoLog);

        // free the message buffer
        delete[] infoLog;
        return false;
    }

    return true;
}

// load a set of shaders
bool loadShaders(unsigned int progID, const ShaderList& components) {
    // load and link shader code
    ShaderList::const_iterator ci = components.begin(), end = components.end();
    for (; ci != end; ++ci) {
        if (!loadShader(*ci)) return false;
        glAttachShader(progID, ci->id);
    }
    glLinkProgram(progID);

    // report link errors
    GLint success;
    glGetProgramiv(progID, GL_LINK_STATUS, &success);
    if (!success) {
        // how big is the message?
        GLsizei infoLen;
        glGetProgramiv(progID, GL_INFO_LOG_LENGTH, &infoLen);

        // print the message
        char* infoLog = new char[infoLen];
        glGetProgramInfoLog(progID, infoLen, 0, infoLog);
        std::fprintf(stderr, "link:\n%s", infoLog);

        // free the message buffer
        delete[] infoLog;
        return false;
    }

    return true;
}

