#pragma once
#include <Windows.h>
#include <GL/glew.h>
#include <GL/GL.h>

class Texture{
public:
	Texture(int width, int height);
	Texture(const char* path);

private:
	GLuint handle;
	int width, height;
};