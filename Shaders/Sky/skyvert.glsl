#version 400
 
layout (location = 0) in vec3 vpos;
out vec3 textureCoord;

uniform mat4 view_matrix;
uniform mat4 projection_matrix;


void main()
{
    gl_Position =   projection_matrix * view_matrix * vec4(vpos, 1.0);
    textureCoord = vpos;
}  