#version 120


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

varying vec4 P;
varying vec4 N;

void main()
{
	vec4 L;
	vec4 R;
	vec4 E = normalize(vec4(eye, 1) - P);

	vec3 I = ka;

	for(int i = 0; i < NUM_LIGHTS; i++)
	{
		L = normalize(vec4(lights[i].position, 1) - P);
		R = 2 * dot(L, N) * N - L;
		I += (lights[i].color * (kd * max(0, dot(L, N)) + (ks * pow(max(0, dot(R, E)), s))));
	}

	gl_FragColor = vec4(I, 1.0f);
}
