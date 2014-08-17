#version 410

layout (location = 0) out vec4 FragColor;

uniform mat4 projection_matrix;
uniform mat4 modelview_matrix;
uniform mat4 mvp_matrix;
uniform mat3 normal_matrix;
uniform mat4 modelview_cam;

flat in vec3 color;
in vec3 position;
in vec3 normal;
in vec2 UV;
uniform sampler2D texture_t;

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
	vec3 final_color = vec3(0.0, 0.0, 0.0);
	//final_color += color;

	for (int i = 0; i < num_lights; ++i) {
		light_pos = light[i].position;
		light_pos = vec3(modelview_cam * vec4(light_pos, 1.0));

		color_ambient = light[i].color_ambient;
		color_diffuse = light[i].color_diffuse;
		color_specular = light[i].color_specular;

		vec3 eye_vec = position;

		if (include_ambience)
			final_color += color_ambient;

		vec3 N = normalize(normal);
		vec3 L = normalize(light_pos-eye_vec);
		vec3 E = normalize(-eye_vec);

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

	FragColor = vec4(final_color,1);
}

// ------------

/*
layout (location = 0) out vec4 FragColor;

uniform mat4 projection_matrix;
uniform mat4 modelview_matrix;
uniform mat4 mvp_matrix;
uniform mat3 normal_matrix;
uniform mat4 modelview_cam;

//uniform sampler2D texture_color;
//uniform float use_texture_s;
//uniform float use_modelview_s;
//uniform float no_of_lights_to_use;
//uniform float is_blur;
//uniform mat4 modelview_cam;
//uniform float transparency;

flat in vec3 color;
in vec3 position;
in vec3 normal;
in vec2 UV;
uniform sampler2D texture_t;

struct Light {
	vec3 position;
	vec3 color_ambient;
	vec3 color_diffuse;
	vec3 color_specular;
};

uniform int num_lights;
uniform float shininess;
uniform Light light[32];

vec3 ads( )
{
	vec3 n = normal;
  
	if (!gl_FrontFacing)
		n = -n;

	vec3 ads_vec = vec3(0., 0., 0.);
	vec3 final_color = ads_vec;

	for (int i = 0; i < num_lights; ++i) {
		vec3 light_pos = light[i].position;
		// This multiplication is necessary to decouple light position from camera movement
		//light_positions[0] = vec3(modelview_cam * vec4(light_positions[i], 1.0));
		//light_positions[i] = vec3(modelview_cam * vec4(light_positions[i], 1.0));
		light_pos = vec3(modelview_cam * vec4(light_pos, 1.0));

		vec3 s = normalize(light_pos - position);
		vec3 v = normalize(-position);
		vec3 r = reflect(-s, n);
		float s_dot_n = max(dot(s, n), 0.0);
		float amb = 0.5;

		ads_vec = color;
		ads_vec += light[i].color_ambient * amb;
		ads_vec += light[i].color_diffuse * s_dot_n;
		ads_vec += (s_dot_n > 0 ? light[i].color_specular * pow(max(dot(r, v), 0.0), shininess) : vec3(0.0));

		final_color += ads_vec;

		//ads_vec = ads_vec + amb * color + color * s_dot_n + (s_dot_n > 0 ? color * pow(max(dot(r, v), 0.0), shininess) : vec3(0.0));
	}
  
	return final_color;
}

void main()
{
	FragColor = vec4(ads(), 1.);
	//FragColor = vec4(color, 1.);
	//FragColor += texture(texture_t, UV);
}
*/


