#version 410

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_color;
layout (location = 2) in vec3 vertex_normal;
layout (location = 3) in vec2 texture_coordinates;

uniform mat4 projection_matrix;
uniform mat4 modelview_matrix;
uniform mat4 mvp_matrix;
uniform mat3 normal_matrix;
uniform mat3 modelview_cam;

flat out vec3 color;
out vec3 normal;
out vec3 position;
out vec2 UV;

void main()
{
	color = vertex_color;
	//normal = normalize(mat3(modelview_matrix)* vertex_normal);
	normal = normalize(normal_matrix * vertex_normal);
	position = vec3(modelview_matrix * vec4(vertex_position,1.0));
	gl_Position = mvp_matrix * vec4(vertex_position, 1.0);
	UV = texture_coordinates;
}
