#version 460
// It was expressed that some drivers required this next line to function properly
precision highp float;

layout(location = 4) uniform vec3 color;

out vec4 gl_FragColor;

void main(void) {
    gl_FragColor = vec4(color, 1.0);
}