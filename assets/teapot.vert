#version 460

layout(location = 0) in vec3 position;
layout(location = 1) uniform mat4 Model;
layout(location = 2) uniform mat4 View;
layout(location = 3) uniform mat4 Projection;

void main(void) {
    gl_Position = Projection * View * Model * vec4(position, 1.0); // Projection * View * 
}