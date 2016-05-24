#include "geometry.hpp"

#include "config.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// create necessary buffers
Geometry::Geometry()
: _numDraws(0) {
    glGenBuffers(NUM_BUFFERS, _bufferIDs);
    glGenVertexArrays(1, &_varrayID);

    // load shaders
    _shaderParts.push_back(ShaderInfo(glCreateShader(GL_VERTEX_SHADER),
                                      PROJECT_DATA_DIR"bump.vert"));
    _shaderParts.push_back(ShaderInfo(glCreateShader(GL_FRAGMENT_SHADER),
                                      PROJECT_DATA_DIR"bump.frag"));

    _shaderID = glCreateProgram();
    updateShaders();
}

// delete OpenGL objects
Geometry::~Geometry() {
    ShaderList::const_iterator ci = _shaderParts.begin(),
                               end = _shaderParts.end();
    for (ci = _shaderParts.begin(); ci != end; ++ci) {
        glDeleteShader(ci->id);
    }
    glDeleteProgram(_shaderID);

    glDeleteBuffers(NUM_BUFFERS, _bufferIDs);
    glDeleteVertexArrays(1, &_varrayID);
}

// helper to (re)attach OpenGL buffer to a shader variable by name
static void bindBuffer(unsigned int shaderID, const char* paramName,
                       unsigned int bufferID, unsigned int dimensions) {
    GLint attrib = glGetAttribLocation(shaderID, paramName);
    glBindBuffer(GL_ARRAY_BUFFER, bufferID);
    glVertexAttribPointer(attrib, dimensions, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(attrib);
}

// load (or replace) shaders
void Geometry::updateShaders() {
    loadShaders(_shaderID, _shaderParts);
    glUseProgram(_shaderID);

    // map shader name to texture to glActiveTexture number

    // re-connect attribute arrays
    glBindVertexArray(_varrayID);
    bindBuffer(_shaderID, "position",   _bufferIDs[POS_BUFFER],     3);
    bindBuffer(_shaderID, "normal",     _bufferIDs[NORM_BUFFER],    3);
    bindBuffer(_shaderID, "uv",         _bufferIDs[UV_BUFFER],      2);
    bindBuffer(_shaderID, "drawID",     _bufferIDs[DRAWID_BUFFER],  1);

    // (re)connect uniform blocks
    glUniformBlockBinding(_shaderID,
        glGetUniformBlockIndex(_shaderID, "FrameData"), FRAME_UNIFORMS);
    glUniformBlockBinding(_shaderID,
        glGetUniformBlockIndex(_shaderID, "ModelData"), MODEL_UNIFORMS);
}

// reserve space for a new draw, returning the drawID (gl_DrawID)
unsigned int Geometry::addDraw(unsigned int vtxCount, unsigned int idxCount) {
    // update counts
    const unsigned int vtxStart = _pos.size();
    const unsigned int vertTotal = vtxStart+vtxCount;
    const unsigned int idxStart = _tris.size();
    const unsigned int indexTotal = idxStart+idxCount;
    const unsigned int id = _numDraws++;
    assert(_numDraws < MAX_DRAWS);

    // resize arrays
    _pos.resize(vertTotal);
    _norm.resize(vertTotal);
    _uv.resize(vertTotal);
    _drawID.resize(vertTotal);
    _tris.resize(indexTotal);

    // fill in drawID for each new vertex
    for (unsigned int i=vtxStart; i!=vertTotal; ++i) {
        _drawID[i] = static_cast<float>(id);
    }

    // add to draw lists
    _indexCount[id] = idxCount;
    _baseIndex[id] = idxStart;
    _baseIndexOffset[id] = (Index*)0 + idxStart;
    _baseVertex[id] = vtxStart;

    return id;
}

// helper to bind buffer data
static void bindData(unsigned int bufferType, unsigned int bufferID,
                     unsigned int size, void* base, unsigned int bufferUsage) {
    glBindBuffer(bufferType, bufferID);
    glBufferData(bufferType, size, base, bufferUsage);
}

// get pointer to start of vertex array for specific draw
// update vertex and index data after last addDraw
void Geometry::finalizeDrawData() {
    // update draw data after it's all filled in
    const unsigned int vertTotal = _pos.size();
    const unsigned int indexTotal = _tris.size();
    glBindVertexArray(_varrayID);

    // load data into each buffer: GL_STATIC_DRAW since we won't change it
    bindData(GL_ARRAY_BUFFER, _bufferIDs[POS_BUFFER],
             vertTotal*sizeof(glm::vec3), _pos.data(), GL_STATIC_DRAW);
    bindData(GL_ARRAY_BUFFER, _bufferIDs[NORM_BUFFER],
             vertTotal*sizeof(glm::vec3), _norm.data(), GL_STATIC_DRAW);
    bindData(GL_ARRAY_BUFFER, _bufferIDs[UV_BUFFER],
             vertTotal*sizeof(glm::vec2), _uv.data(), GL_STATIC_DRAW);
    bindData(GL_ARRAY_BUFFER, _bufferIDs[DRAWID_BUFFER],
             vertTotal*sizeof(float), _drawID.data(), GL_STATIC_DRAW);

    bindData(GL_ELEMENT_ARRAY_BUFFER, _bufferIDs[INDEX_BUFFER],
             indexTotal*sizeof(Index), _tris.data(), GL_STATIC_DRAW);

    // per-model uniform data, this one is GL_DYNAMIC_DRAW since it changes
    bindData(GL_UNIFORM_BUFFER, _bufferIDs[MODEL_UNIFORM_BUFFER],
             sizeof(_modelUniforms), &_modelUniforms[0], GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, MODEL_UNIFORMS,
                     _bufferIDs[MODEL_UNIFORM_BUFFER]);
}

void Geometry::draw() const {
    // update per-model uniform data
    glBindBuffer(GL_UNIFORM_BUFFER, _bufferIDs[MODEL_UNIFORM_BUFFER]);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(_modelUniforms),
                    &_modelUniforms[0]);

    // draw everything
    glMultiDrawElementsBaseVertex(GL_TRIANGLES, &_indexCount[0],
        GL_UNSIGNED_SHORT, (GLvoid**)&_baseIndexOffset[0], _numDraws,
        &_baseVertex[0]);

}

