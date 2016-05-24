// vertex shader for simple terrain demo
#version 450 core


// must match value in Geometry.hpp
#define MAX_DRAWS 16


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

// per-vertex input
in vec3 position;
in vec3 normal;
in vec2 uv;
in float drawID;

// output to fragment shader
out vec4 vPosition;
out vec3 vNormal;
out vec2 vTexcoord;
out vec4 vColor;
out float vID;


void main() {
    int id = int(drawID + 0.5);
    vID = id;

	// combined transform from model space to view space
	mat4 MV = viewMatrix * model[id].matrix;
    mat4 MVi = model[id].inverse * viewInverse;

	// transform position to view space and to projection space
	vPosition = MV * vec4(position, 1);
	gl_Position = projectionMatrix * vPosition;

    vNormal = normalize(normal * mat3(MVi));

	// just pass texture coordinates through
	vTexcoord = uv;
    vColor = model[id].color;
}
