#pragma once
#include <Windows.h>
#include <GL/glew.h>
#include <GL/GL.h>

class Texture{
public:
	Texture(int width, int height);
	Texture(const char* path);

	virtual GLuint getID();
private:
	GLuint handle;
	void init(int width, int height);
	int width, height;
};