#version 300 es
precision mediump float;

in vec3 ourColor;
in  vec2 TexCoord;
uniform sampler2D texture_1;
out vec4 fragColor;

void main()
{
	fragColor = texture(texture_1, TexCoord) * vec4(ourColor, 1.0);
}