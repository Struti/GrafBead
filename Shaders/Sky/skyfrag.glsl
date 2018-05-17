#version 400

#define PI 3.1415f

in vec3 textureCoord;
layout (location = 0) out vec4 color;

uniform samplerCube skybox_texture;

void main()
{   
	vec3 tx = normalize(textureCoord);
    color = texture(skybox_texture, textureCoord);
}