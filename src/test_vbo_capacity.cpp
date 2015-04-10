#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <stdio.h>
#include <Eigen/Dense>
#include <set>
#include <utility>
#include <iostream>
#include <cstdlib>
#include <ctime>

#include "Mesh.h"
#include "Material.h"
#include "Texture.h"
#include "FBO.h"
#include "VideoWriter.hpp"

using namespace Eigen;
using namespace std;

const int POINT_COUNT = 400;

#define for_range(i, range) for(int i = 0; i < range; i++)

static inline float tt(int x){
	return x / (POINT_COUNT/2.f) - 1;
}

int main_vbo(int argc, char** argv) {
	// start GL context and O/S window using the GLFW helper library
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return 1;
	}

	// uncomment these lines if on Apple OS X
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(POINT_COUNT, POINT_COUNT, "Hello Triangle", NULL, NULL);
	if (!window) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(window);


	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	glewInit();

	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	//glEnable(GL_DEPTH_TEST); // enable depth-testing
	//glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	/* OTHER STUFF GOES HERE NEXT */

	
	Vector4f *points = new Vector4f[POINT_COUNT*POINT_COUNT], *colors = new Vector4f[POINT_COUNT*POINT_COUNT];
	for (int i = 0; i < POINT_COUNT; i++){
		for (int j = 0; j < POINT_COUNT; j++){
			int a = i * POINT_COUNT + j;
			points[a] << tt(i), tt(j), 0.0f, 1.0f;
			float r = float(a) / (POINT_COUNT * POINT_COUNT);
			colors[a] << i % 256, j % 256, a / (256*256), 255;
			colors[a] /= 255.f;
		}
	}


	Material *mat = new Material("shaders/vbo.vert", "shaders/vbo.frag");
	Mesh *m = new Mesh(GL_POINTS, &points[0][0], POINT_COUNT*POINT_COUNT);
	m->setColors(&colors[0][0]);

	delete[] points;
	delete [] colors;
	glClearColor(0, 0, 0, 0);


	while (!glfwWindowShouldClose(window)) {
		// wipe the drawing surface clear
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m->draw(mat);


		// update other events like input handling 
		glfwPollEvents();
		// put the stuff we've been drawing onto the display
		glfwSwapBuffers(window);
	}

	// close GL context and any other GLFW resources
	glfwTerminate();
	return 0;
}