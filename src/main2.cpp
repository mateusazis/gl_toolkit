#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <stdio.h>

#include "Mesh.h"
#include "Material.h"
#include "Texture.h"
#include "FBO.h"

int main3() {
	// start GL context and O/S window using the GLFW helper library
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return 1;
	}

	// uncomment these lines if on Apple OS X
	/*glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

	GLFWwindow* window = glfwCreateWindow(400, 400, "Hello Triangle", NULL, NULL);
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
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"

	/* OTHER STUFF GOES HERE NEXT */

	float points[] = {
		-1.0f, -1.0f, 0.0f, 1.0f, 
		-1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, -1.0f, 0.0f, 1.0f
	};

	int indices[] = { 0, 1, 2, 0, 3, 2 };

	float uv[] = {
		0, 0,
		0, 1,
		1, 1,
		1, 0
	};

	Material *mat = new Material("shaders/basic.vert", "shaders/basic.frag");
	Material *m2 = new Material("shaders/basic.vert", "shaders/invert.frag");
	Mesh *m = new Mesh(GL_TRIANGLES, points, 4, indices, 6, uv);
	Texture *t = new Texture("a.jpg");
	FBO* fbo = new FBO(400, 400);
	mat->texture("tex", t);
	m2->texture("tex", fbo);

	
	while (!glfwWindowShouldClose(window)) {
		// wipe the drawing surface clear
		m->draw(mat, fbo);
		glFlush();

		glClear(GL_DEPTH_BUFFER_BIT);
		m->draw(m2);
		glFlush();

		// update other events like input handling 
		glfwPollEvents();
		// put the stuff we've been drawing onto the display
		glfwSwapBuffers(window);
	}
	// close GL context and any other GLFW resources

	glfwTerminate();
	return 0;
}