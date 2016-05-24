// fragment shader for simple terrain application
#version 450 core
#extension GL_ARB_shader_draw_parameters : enable


// must match value in Geometry.hpp
#define MAX_DRAWS 16

// must match value in Image.hpp
#define MAX_TEXTURE_ARRAYS 8


// per-frame data using standard layout, matching C++
layout(std140) uniform FrameData { // per-frame data
	mat4 viewMatrix, viewInverse;
	mat4 projectionMatrix, projectionInverse;
};

// per-model data, should be multiple of four floats
struct ModelDataStruct {
	mat4 matrix, inverse;		// model to view transform
    vec4 color;                 // model flat color
};
layout(std140) uniform ModelData {
	ModelDataStruct model[MAX_DRAWS];
};

// each textures[i] is a different size, containing all textures of that size
//uniform sampler2DArray tex2d[MAX_TEXTURE_ARRAYS];

// input from vertex shader
in vec4 vPosition;
in vec3 vNormal;
in vec2 vTexcoord;
in vec4 vColor;
in float vID;

// output to frame buffer
out vec4 fragColor;


void main() {
    int id = int(vID + 0.5);

	// final color
	fragColor = vColor;
    fragColor = vec4(vNormal, 1);
}
