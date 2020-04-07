#version 300 es
layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec2 vTexCoord;
out vec2 texCoord;
uniform mat4 transform;

void main()
{
	texCoord = vTexCoord;
	gl_Position = transform * vec4(vPosition, 1.0);
}
