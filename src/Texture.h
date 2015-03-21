#pragma once
#include <Windows.h>
#include <GL/glew.h>
#include <GL/GL.h>

class Texture{
public:
	Texture(int width, int height);
	Texture(const char* path);

	GLuint getID();

protected:
	int width, height;
private:
	GLuint handle;
	void init(int width, int height);
};