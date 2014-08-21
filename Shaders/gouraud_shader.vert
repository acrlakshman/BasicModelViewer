#version 330

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_color;
layout (location = 2) in vec3 vertex_normal;
layout (location = 3) in vec2 texture_coordinates;

uniform mat4 projection_matrix;
uniform mat4 modelview_matrix;
uniform mat4 mvp_matrix;
uniform mat3 normal_matrix;
uniform mat4 modelview_cam;

flat out vec3 color;
out vec3 normal;
out vec3 position;
out vec2 UV;

struct Light {
	vec3 position;
	vec3 color_ambient;
	vec3 color_diffuse;
	vec3 color_specular;
};

#define max_lights 32

uniform int num_lights;
uniform Light light[max_lights];
uniform float shininess;
uniform bool include_ambience;
uniform bool include_diffuse;
uniform bool include_specular;

vec3 color_specular;
vec3 color_ambient;
vec3 color_diffuse;
vec3 light_pos;

void main()
{
	color = vertex_color;
	normal = normalize(normal_matrix * vertex_normal);
	position = vec3(modelview_matrix * vec4(vertex_position,1.0));
	gl_Position = mvp_matrix * vec4(vertex_position, 1.0);

	vec3 final_color = vec3(0.0, 0.0, 0.0);
	//final_color += color;

	for (int i = 0; i < num_lights; ++i) {
		light_pos = light[i].position;
		light_pos = vec3(modelview_cam * vec4(light_pos, 1.0));

		color_ambient = light[i].color_ambient;
		color_diffuse = light[i].color_diffuse;
		color_specular = light[i].color_specular;

		if (include_ambience)
			final_color += color_ambient;

		vec3 N = normalize(normal);
		vec3 L = normalize(light_pos-position);
		vec3 E = normalize(-position);

		float lambertTerm = dot(N,L);

		if(lambertTerm > 0.0)
		{
			if (include_diffuse)
				final_color += color_diffuse* lambertTerm;

			vec3 R = reflect(-L, N);
			float specular = pow( max(dot(R, E), 0.0), shininess );
			if (include_specular)
				final_color += color_specular* specular;	
		}
	}

	color = final_color;
	UV = texture_coordinates;
}
