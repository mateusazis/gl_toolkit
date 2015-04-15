#pragma once
#include <Windows.h>
#include <GL/glew.h>
#include <GL/GL.h>
#include <map>

class Material;
class FBO;

class Mesh{
public:
	Mesh(GLenum topology, const float* vertices, int vCount, const int* indices = NULL, const int iCount = 0, const float* uv = NULL);
	void draw(Material* mat, FBO* target = NULL);
	void setVertices(const float* vertices, int vCount);
	void setColors(const float* colors);
	void setAttribute1f(GLint attrLoc, const float* values);

	static const GLuint POSITION_LOCATION = 0, UV_LOCATION = 1, COLOR_LOCATION = 2;
private:
	GLuint posVBO, indexVBO, uvVBO, colorVBO, vao;
	GLenum topology;
	int vCount, indexCount;

	bool hasIndices;

	std::map<GLuint, GLuint> attributeVBOs;
};
