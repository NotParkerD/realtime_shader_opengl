#version 120

attribute vec3 vPositionModel; // in object space
attribute vec3 vNormalModel; // in object space

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 itMatrix;

varying vec4 P;
varying vec4 N;

void main()
{
	gl_Position = projection * view * model * vec4(vPositionModel, 1.0);


	P = model * vec4(vPositionModel, 1.0);
	N = itMatrix * vec4(vNormalModel, 0.0);
}
