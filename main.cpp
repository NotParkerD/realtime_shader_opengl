// Material and Light structs written by Parker Drake
// Display and Callback functions written by Parker Drake
// Shader files written by Parker Drake
// Used Tiny Object Loader from MIT

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

#include "Program.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"


#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define NUM_LIGHTS 2
#define NUM_MATERIALS 3

GLFWwindow *window;

Program program[3]; // Array of programs so that multiple shader files can be linked to different shading methods
int selProgram = 0;

std::vector<float> posBuff;
std::vector<float> norBuff;
std::vector<float> texBuff;

glm::vec3 eye(0.0f, 0.0f, 4.0f);

int matType = 0;
int selLight = 0;

struct materialStruct {
	glm::vec3 ka, kd, ks;
	float s;
} materials[NUM_MATERIALS];

struct lightStruct {
	glm::vec3 position;
	glm::vec3 color;
} lights[NUM_LIGHTS];


// Sets the material values based on which material is selected
void setMaterial()
{
	switch (matType)
	{
	case 0:
		materials[0].ka = glm::vec3(0.2, 0.2, 0.2);
		materials[0].kd = glm::vec3(0.8, 0.7, 0.7);
		materials[0].ks = glm::vec3(1.0, 1.0, 1.0);
		materials[0].s = 10.0;
		break;
	case 1:
		materials[0].ka = glm::vec3(0.0, 0.2, 0.2);
		materials[0].kd = glm::vec3(0.5, 0.7, 0.2);
		materials[0].ks = glm::vec3(0.1, 1.0, 0.1);
		materials[0].s = 100.0;
		break;
	case 2:
		materials[0].ka = glm::vec3(0.2, 0.2, 0.2);
		materials[0].kd = glm::vec3(0.1, 0.3, 0.9);
		materials[0].ks = glm::vec3(0.1, 0.1, 0.1);
		materials[0].s = 1.0;
		break;
	}
}


// Sets the initial color and position of the lights
void setLights()
{
	lights[0].position = glm::vec3(0.0, 0.0, 3.0);
	lights[0].color = glm::vec3(0.5, 0.5, 0.5);

	lights[1].position = glm::vec3(0.0, 3.0, 0.0);
	lights[1].color = glm::vec3(0.2, 0.2, 0.2);	
}

void Display()
{		
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), float(width) / float(height), 0.1f, 100.0f);
	glm::mat4 viewMatrix = glm::lookAt(eye, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 modelMatrix(1.0f);
	modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.2f, -1.0f, 0.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	
	glm::mat4 itMatrix = glm::inverse(glm::transpose(modelMatrix)); // Inverse transpose of transformation matrix

	program[selProgram].Bind();
	program[selProgram].SendUniformData(modelMatrix, "model");
	program[selProgram].SendUniformData(viewMatrix, "view");
	program[selProgram].SendUniformData(projectionMatrix, "projection");
	program[selProgram].SendUniformData(itMatrix, "itMatrix"); // Send inverted-transposed matrix to the GPU


	program[selProgram].SendUniformData(eye, "eye"); // Send the eye position to the GPU
	
	setMaterial(); // Set material variables and send to GPU
	program[selProgram].SendUniformData(materials[0].ka, "ka");
	program[selProgram].SendUniformData(materials[0].kd, "kd");
	program[selProgram].SendUniformData(materials[0].ks, "ks");
	program[selProgram].SendUniformData(materials[0].s, "s");

	// Send light data to the GPU
	program[selProgram].SendUniformData(lights[0].position, "lights[0].position");
	program[selProgram].SendUniformData(lights[0].color, "lights[0].color");
	program[selProgram].SendUniformData(lights[1].position, "lights[1].position");
	program[selProgram].SendUniformData(lights[1].color, "lights[1].color");


	glDrawArrays(GL_TRIANGLES, 0, posBuff.size() / 3);
	program[selProgram].Unbind();

}

// Keyboard character callback function
void CharacterCallback(GLFWwindow* lWindow, unsigned int key)
{
	switch (key) 
	{
	case 'q':
		glfwSetWindowShouldClose(window, GLFW_TRUE);
		break;
	case 'm':
		matType++;
		if (matType > NUM_MATERIALS - 1)
			matType = 0;
		break;
	case 'M':
		matType--;
		if (matType < 0)
			matType = NUM_MATERIALS - 1;
		break;
	case 'l':
		selLight++;
		if (selLight > NUM_LIGHTS - 1)
			selLight = 0;
		break;
	case 'L':
		selLight--;
		if (selLight < 0)
			selLight = NUM_LIGHTS - 1;
		break;
	case 'x':
		lights[selLight].position += glm::vec3(0.1, 0, 0);
		break;
	case 'X':
		lights[selLight].position += glm::vec3(-0.1, 0, 0);
		break;
	case 'y':
		lights[selLight].position += glm::vec3(0, 0.1, 0);

		break;
	case 'Y':
		lights[selLight].position += glm::vec3(0, -0.1, 0);

		break;
	case 'z':
		lights[selLight].position += glm::vec3(0, 0, 0.1);

		break;
	case 'Z':
		lights[selLight].position += glm::vec3(0, 0, -0.1);

		break;
	case '1':
		selProgram = 0;
		program[selProgram].Init();
		program[selProgram].SendAttributeData(posBuff, "vPositionModel");
		program[selProgram].SendAttributeData(norBuff, "vNormalModel");
		break;
	case '2':
		selProgram = 1;
		program[selProgram].Init();
		program[selProgram].SendAttributeData(posBuff, "vPositionModel");
		program[selProgram].SendAttributeData(norBuff, "vNormalModel");
		break;
	case '3':
		selProgram = 2;
		program[selProgram].Init();
		program[selProgram].SendAttributeData(posBuff, "vPositionModel");
		program[selProgram].SendAttributeData(norBuff, "vNormalModel");
		break;
	default:
		break;
	}
}


void FrameBufferSizeCallback(GLFWwindow* lWindow, int width, int height)
{
	glViewport(0, 0, width, height);
}

void LoadModel(char* name)
{
	// Taken from Shinjiro Sueda with slight modification
	std::string meshName(name);
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string errStr;
	bool rc = tinyobj::LoadObj(&attrib, &shapes, &materials, &errStr, meshName.c_str());
	if (!rc) {
		std::cerr << errStr << std::endl;
	}
	else {
		// Some OBJ files have different indices for vertex positions, normals,
		// and texture coordinates. For example, a cube corner vertex may have
		// three different normals. Here, we are going to duplicate all such
		// vertices.
		// Loop over shapes
		for (size_t s = 0; s < shapes.size(); s++) {
			// Loop over faces (polygons)
			size_t index_offset = 0;
			for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
				size_t fv = shapes[s].mesh.num_face_vertices[f];
				// Loop over vertices in the face.
				for (size_t v = 0; v < fv; v++) {
					// access to vertex
					tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
					posBuff.push_back(attrib.vertices[3 * idx.vertex_index + 0]);
					posBuff.push_back(attrib.vertices[3 * idx.vertex_index + 1]);
					posBuff.push_back(attrib.vertices[3 * idx.vertex_index + 2]);
					if (!attrib.normals.empty()) {
						norBuff.push_back(attrib.normals[3 * idx.normal_index + 0]);
						norBuff.push_back(attrib.normals[3 * idx.normal_index + 1]);
						norBuff.push_back(attrib.normals[3 * idx.normal_index + 2]);
					}
					if (!attrib.texcoords.empty()) {
						texBuff.push_back(attrib.texcoords[2 * idx.texcoord_index + 0]);
						texBuff.push_back(attrib.texcoords[2 * idx.texcoord_index + 1]);
					}
				}
				index_offset += fv;
				// per-face material (IGNORE)
				shapes[s].mesh.material_ids[f];
			}
		}
	}
}

void Init()
{
	glfwInit();
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Realtime Shader", NULL, NULL);
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	glewInit();
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glfwSetCharCallback(window, CharacterCallback);
	glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);

	LoadModel("../obj/bunny.obj");
	
	program[0].SetShadersFileName("../shaders/shader1.vert", "../shaders/shader1.frag");
	program[1].SetShadersFileName("../shaders/shader2.vert", "../shaders/shader2.frag");
	program[2].SetShadersFileName("../shaders/shader3.vert", "../shaders/shader3.frag");

	program[0].Init();
	program[0].SendAttributeData(posBuff, "vPositionModel");
	program[0].SendAttributeData(norBuff, "vNormalModel");
}


int main()
{	
	Init();
	setLights();
	while ( glfwWindowShouldClose(window) == 0) 
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		Display();
		glFlush();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}