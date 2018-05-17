#version 400

uniform mat4 projection_matrix;
uniform mat4 model_matrix;
uniform mat4 view_matrix;

uniform int reflectionRender;
uniform int refractionRender;
uniform float water_level;

layout(location = 0) in vec2 a_Vertex;

out vec3 worldVertex;
out vec3 worldNormal;
out vec2 texCoord;

void main()
{	
	mat4 reflection_matrix=
	mat4(1,0,0,0,
		 0,-1,0,0,
		 0,0,1,0,
		 0,water_level*2,0,1);

	vec4 vworldVertex;
	if(reflectionRender==1)
    {
		vworldVertex =  model_matrix * reflection_matrix * vec4(a_Vertex.x, 1, a_Vertex.y, 1.0);
	}
	else
	{
		vworldVertex =  model_matrix * vec4(a_Vertex.x, 1, a_Vertex.y, 1.0);
	}

    mat4 normalMatrix = transpose(inverse(model_matrix));

	worldNormal = normalize(vec3(normalMatrix * vec4(0,1,0,1)));
	worldVertex = vworldVertex.xyz;
	texCoord=vworldVertex.xz;
	gl_Position=projection_matrix*view_matrix*vworldVertex;
}