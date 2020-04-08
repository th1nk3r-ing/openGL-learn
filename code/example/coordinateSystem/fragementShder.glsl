#version 300 es 
precision highp float;
in vec2 texCoord;
uniform sampler2D texture_0;
uniform sampler2D texture_1;
out vec4 fragColor;
void main()
{
	fragColor = mix(texture(texture_0, texCoord), texture(texture_1, texCoord), 0.30);	
}
