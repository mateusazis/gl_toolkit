#pragma once
#include <Windows.h>
#include <GL/glew.h>
#include <GL/GL.h>

class Material{
public:
	Material(const char* vPath, const char* fPath);
	void commitQueue();

	GLuint progID;
};