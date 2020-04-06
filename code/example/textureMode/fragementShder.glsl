#version 300 es 
precision mediump float;

in  vec2 texCoord_0;
in  vec2 texCoord_1;
uniform sampler2D texture_0, texture_1; 
out vec4 fragColor; 

void main()
{
	fragColor = mix(texture(texture_0, texCoord_0), texture(texture_1, texCoord_1), 0.30);	
}
