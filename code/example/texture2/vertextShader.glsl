#version 300 es
layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;
out vec2 TexCoord;
out vec3 ourColor;
void main()
{
	ourColor = aColor;
	TexCoord = aTexCoord;
	gl_Position = vec4(vPosition.x, vPosition.y, vPosition.z, 1.0);
}
