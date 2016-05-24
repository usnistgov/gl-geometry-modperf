#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include "xform.hpp"
#include "shader.hpp"

#include <glm/vec2.hpp>

#include <vector>

class Geometry {
public:
    // GL version less than 4.1 likes fixed sized arrays.
    // If you change this, update it in any shaders too.
    static const int MAX_DRAWS = 16;

    // shader "uniform" parameter blocks
    enum UniformBlocks {
        FRAME_UNIFORMS,     // data that changes once per frame (view, lights)
        MODEL_UNIFORMS,     // per-object data (transforms, parameters, etc.)
        NUM_UNIFORM_BLOCKS
    };

    // per frame shader data type
    struct FrameUniforms {
        Xform viewMats;     // world to view matrix & inverse
        Xform projMats;     // view to NDC matrix and inverse
    };

    // per draw shader data type
    struct ModelUniforms {
        Xform modelMats;    // object to view matrix & inverse
        glm::vec4 color;    // flat color of object
    };

    // triangle index type: change here and in Geometry::draw
    typedef unsigned short Index;


    Geometry();
    ~Geometry();

    ModelUniforms* modelUniforms(unsigned int id) { return &_modelUniforms[id]; }

    Index* tris(unsigned int id) { return &_tris[_baseIndex[id]]; }

    unsigned int baseVert(unsigned int id) { return _baseVertex[id]; }
    glm::vec3* pos(unsigned int id) { return &_pos[id]; }
    glm::vec3* norm(unsigned int id) { return &_norm[id]; }
    glm::vec2* uv(unsigned int id) { return &_uv[id]; }

    // load (or replace) shaders
    void updateShaders();

    // reserve space for a new draw, returning the drawID (gl_DrawID)
    unsigned int addDraw(unsigned int vertCount, unsigned int indexCount);

    // update vertex and index data after last addDraw
    void finalizeDrawData();

    // draw current geometry
    void draw() const;

private:
    // parameters for each draw
    unsigned int _numDraws;                     // number of models to draw
    ModelUniforms _modelUniforms[MAX_DRAWS];    // per draw shader data

    // triangle index data
    int _indexCount[MAX_DRAWS];         // number of indices for each draw
    Index _baseIndex[MAX_DRAWS];        // starting index
    Index* _baseIndexOffset[MAX_DRAWS]; // offset to starting index, as pointer
    std::vector<Index> _tris;           // triangle index data

    // per vertex data
    int _baseVertex[MAX_DRAWS];     // starting vertex for each draw
    std::vector<glm::vec3> _pos;    // vertex position
    std::vector<glm::vec3> _norm;   // vertex normal
    std::vector<glm::vec2> _uv;     // texture coordinates
    std::vector<float> _drawID;     // drawID

    // shaders
    unsigned int _shaderID;
    ShaderList _shaderParts;

    enum Buffers {
        POS_BUFFER, NORM_BUFFER, UV_BUFFER, DRAWID_BUFFER,
        INDEX_BUFFER, MODEL_UNIFORM_BUFFER,
        NUM_BUFFERS
    };

    unsigned int _bufferIDs[NUM_BUFFERS];
    unsigned int _varrayID;


};

#endif

