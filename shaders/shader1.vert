#version 120

attribute vec3 vPositionModel; // in object space
attribute vec3 vNormalModel; // in object space

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 itMatrix;

uniform vec3 eye;

struct lightStruct
{
	vec3 position;
	vec3 color;
};

#define NUM_LIGHTS 2

uniform lightStruct lights[NUM_LIGHTS];

uniform vec3 ka;
uniform vec3 kd;
uniform vec3 ks;
uniform float s;

varying vec3 color;

void main()
{
	gl_Position = projection * view * model * vec4(vPositionModel, 1.0);


	vec4 P = model * vec4(vPositionModel, 1.0);
	vec4 N = itMatrix * vec4(vNormalModel, 0.0);

	vec4 L = vec4(1.0f);
	vec4 R = vec4(1.0f);
	vec4 E = normalize(vec4(eye, 1) - P);

	vec3 I = ka;

	for(int i = 0; i < NUM_LIGHTS; i++)
	{
		L = normalize(vec4(lights[i].position, 1) - P);
		R = 2 * dot(L, N) * N - L;
		I += (lights[i].color * (kd * max(0, dot(L, N)) + (ks * pow(max(0, dot(R, E)), s))));
	}
	color = I;
}
