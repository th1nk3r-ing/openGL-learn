#version 300 es 
precision mediump float;

in vec3 ourColor;
in  vec2 TexCoord;
out vec4 fragColor;
uniform sampler2D texture_0;
uniform sampler2D texture_1;

void main()
{
	vec4 textureColor = mix(texture(texture_0, TexCoord), texture(texture_1, TexCoord), 0.35);
	
	fragColor = textureColor * vec4(ourColor, 1.0);
}
