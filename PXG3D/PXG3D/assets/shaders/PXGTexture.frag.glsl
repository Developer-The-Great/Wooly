//DIFFUSE TEXTURE FRAGMENT SHADER
#version 330 // for glsl version (12 is for older versions , say opengl 2.1


in vec2 texCoord;
out vec4 fragment_color;


struct Material
{
	sampler2D texture_diffuse1;
	sampler2D texture_diffuse2;
};

uniform Material material;

void main( void ) {
	vec4 result = vec4(0,0,0,0);
	result += texture(material.texture_diffuse1,texCoord);
	result += texture(material.texture_diffuse2,texCoord);

	fragment_color = result;


}
