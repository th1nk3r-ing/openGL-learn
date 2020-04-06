#version 300 es
layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec2 vTexCoord_0;
layout(location = 2) in vec2 vTexCoord_1;
out vec2 texCoord_0;
out vec2 texCoord_1;

void main()
{
	texCoord_0 = vTexCoord_0;
	texCoord_1 = vTexCoord_1;
	gl_Position = vec4(vPosition.x, vPosition.y, vPosition.z, 1.0);
}
