#version 410

layout (location = 0) out vec4 FragColor;

//uniform mat4 projection_matrix;
//uniform mat4 modelview_matrix;
//uniform mat4 mvp_matrix;
//uniform mat3 normal_matrix;
//uniform mat3 modelview_cam;

float shininess = 100.0;


flat in vec3 color;
//in vec3 normal;
in vec3 position;

void main()
{	
	FragColor = vec4(1.0, 1.0, 1.0 ,1.0);
}


