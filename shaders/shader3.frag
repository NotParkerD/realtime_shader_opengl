#version 120


uniform vec3 eye;

varying vec4 P;
varying vec4 N;

void main()
{
	vec4 L = vec4(1.0f);
	vec4 R = vec4(1.0f);
	vec4 E = normalize(vec4(eye, 1) - P);

	if (dot(N, E) < 0.30f && dot(N, E) >= 0.0f)
		gl_FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	else
		gl_FragColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);

}
