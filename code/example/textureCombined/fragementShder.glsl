#version 300 es
precision mediump float;
in vec3 ourColor;
in  vec2 TexCoord;
uniform sampler2D ourTexture;
out vec4 fragColor;
void main()
{
	fragColor = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0);
}