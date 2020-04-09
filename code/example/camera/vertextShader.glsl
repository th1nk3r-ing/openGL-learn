#version 300 es
layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec2 vTexCoord;
out vec2 texCoord;
uniform mat4 model, view, projection;
void main()
{
	texCoord = vTexCoord;
	gl_Position = projection * view * model * vec4(vPosition, 1.0f);
}
