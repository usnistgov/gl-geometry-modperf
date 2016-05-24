#ifndef SHADER_HPP
#define SHADER_HPP

#include <list>

struct ShaderInfo {
    unsigned int id;
    const char* file;

    ShaderInfo(unsigned int i, char const* f) : id(i), file(f) {}
    ShaderInfo() : id(0), file(NULL) {}
};

typedef std::list<ShaderInfo> ShaderList;

// load a set of shaders
// progID is the program object
// components is a list of shader components to link
// return false on compile error
bool loadShaders(unsigned int progID, const ShaderList& components);

#endif

