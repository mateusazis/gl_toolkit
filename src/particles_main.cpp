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

#define for_range(i, range) for(int i = 0; i < range; i++)

const int POINT_COUNT = 400;
const float RADIUS = 11;

class Particle{
public:
	Particle(){}
	Particle(Vector2f pos) : position(pos), mass(1), velocity(Vector2f(0,0)){

	}

	bool collidesWith(const Particle& other) const {
		const float diameter = 2 * RADIUS;
		return (position - other.position).squaredNorm() < diameter * diameter;
	}

	float distance(const Particle & other) const {
		return (position - other.position).norm();
	}
	Vector2f position;
	Vector2f velocity;
	Vector2f force;
	float mass;
};

class ParticleSystem {
public:
	ParticleSystem(){}
	void initializeSim(){
		for_range(i, POINT_COUNT){
			float x = i % 20;
			float y = i / 20;
			x *= 20;
			y *= 20;
			y += 30;
			particles[i] = Particle(Vector2f(x, y));
		}
	}

	void performCD(){
		collisions.clear();
		for (int i = 0; i < POINT_COUNT - 1; i++){
			for (int j = i+1; j < POINT_COUNT; j++){
				if (particles[i].collidesWith(particles[j]))
					collisions.insert(make_pair(i, j));
			}
		}
	}

	void computeForces(){
		const float SPRINT_STIFFNESS = .1f,
			DAMPING_COEFFICIENT = 0.01f;

		for (auto & p : particles)
			p.force = Vector2f::Zero();


		for (auto & collision : collisions){
			Particle & p1 = particles[collision.first];
			Particle & p2 = particles[collision.second];
			float delta = 2 * RADIUS - (p1.position - p2.position).norm();
			Vector2f deltaV = p1.velocity - p2.velocity;
			Vector2f n = (p1.position - p2.position).normalized();
			Vector2f vN = deltaV.dot(n) * n;

			Vector2f deltaVector = delta * n;
			//cout << "VN:" << endl << vN << endl;
			
			Vector2f normalForce = deltaVector * SPRINT_STIFFNESS + DAMPING_COEFFICIENT * -vN;
			p1.force += normalForce;
			p2.force -= normalForce;
		}
	}

	void integrate(){
		const float GRAVITY = -.01f;

		for (Particle & p : particles){
			if (p.position.y() - RADIUS > 0){
				p.velocity += p.force + Vector2f(0, GRAVITY);
				p.position += p.velocity;
			}
			else{
				p.velocity = Vector2f::Zero();
			}
//			cout << "Particle velocity: " << endl << p.velocity << endl;
				
			
		}
	}

	void updatePoints(){
		for_range(i, POINT_COUNT){
			Vector2f & pos = particles[i].position;
			points[i] = Vector4f(pos.x(), pos.y(), 0, 1);
		}
	}

	Particle particles[POINT_COUNT];
	Vector4f points[POINT_COUNT];
	set<pair<int, int>> collisions;
};

int main() {
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
	//glEnable(GL_DEPTH_TEST); // enable depth-testing
	//glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	/* OTHER STUFF GOES HERE NEXT */

	Material *mat = new Material("shaders/particle.vert", "shaders/particle.frag");
	mat->setFloat("radius", RADIUS);
	

	ParticleSystem pd;
	pd.initializeSim();
	pd.updatePoints();

	Mesh *m = new Mesh(GL_POINTS, &pd.points[0][0], POINT_COUNT);
	{
		Vector4f colors[POINT_COUNT];
		for_range(i, POINT_COUNT){
			float r = i % 20;
			float g = i / 20;
			r /= 20; g /= 20;
			colors[i] = Vector4f(r, g, sqrt(1-r*r+-g*g), 1);
		}
		m->setColors(&colors[0][0]);
	}
	glClearColor(1, 1, 1, 0);

	VideoWriter v = VideoWriter("out.avi", 400, 400);

	while (!glfwWindowShouldClose(window)) {
		// wipe the drawing surface clear
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		pd.performCD();
		pd.computeForces();
		pd.integrate();
		pd.updatePoints();
		m->setVertices(&pd.points[0][0], POINT_COUNT);

		m->draw(mat);
		v.addFrame();
		

		// update other events like input handling 
		glfwPollEvents();
		// put the stuff we've been drawing onto the display
		glfwSwapBuffers(window);
	}
	v.close();

	// close GL context and any other GLFW resources
	glfwTerminate();
	return 0;
}