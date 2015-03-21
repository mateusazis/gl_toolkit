#pragma once
#include <Windows.h>
#include <GL/glew.h>
#include <GL/GL.h>
#include <map>
#include <string>

class Texture;

struct TextureEntry {
	Texture* t;
	int unit;
};

class Material{
public:
	Material(const char* vPath, const char* fPath);

	void texture(const char* loc, Texture* t);
	void commitQueue();

	GLuint progID;

private:
	std::map<GLuint, int> textureUnits;
	std::map<std::string, TextureEntry> textureEntries;
};