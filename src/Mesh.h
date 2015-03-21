#pragma once
#include <Windows.h>
#include <GL/glew.h>
#include <GL/GL.h>

class Material;

class Mesh{
public:
	Mesh(GLenum topology, const float* vertices, int vCount, const int* indices = NULL, const int iCount = 0, const float* uv = NULL);
	void draw(Material* mat);

private:
	GLuint posVBO, indexVBO, uvVBO, vao;
	GLenum topology;
	int vCount, indexCount;

	bool hasIndices;
};