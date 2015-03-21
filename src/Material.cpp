#include "Material.h"

#include <cstdio>
#include <iostream>

 // include GLEW and new version of GL on Windows

using namespace std;

static GLint getShader(const char* path, GLenum type){
	const int MAX_SIZE = 2048;
	char src[MAX_SIZE] = "";
	char line[200] = "";
	FILE* f = fopen(path, "r");
	while (fgets(line, MAX_SIZE, f)){
		strcat(src, line);
	}
	fclose(f);

	cout << "Src:" << endl << src << endl;

	GLint handle = glCreateShader(type);
	char* c = src;
	glShaderSource(handle, 1, &c, NULL);
	glCompileShader(handle);

	src[0] = '\0';
	int a;
	glGetShaderSource(handle, MAX_SIZE, &a, src);
	cout << "actual src: " << endl << src << endl;

	GLint isCompiled;
	glGetShaderiv(handle, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		char errorLog[2048];
		glGetShaderInfoLog(handle, maxLength, &maxLength, &errorLog[0]);
		cerr << "Error for " << (type == GL_FRAGMENT_SHADER ? "fragment" : "vertex") << " shader:" << endl;
		cerr << errorLog << endl;

		// Provide the infolog in whatever manor you deem best.
		// Exit with failure.
		glDeleteShader(handle); // Don't leak the shader.
	}
	return handle;
}


Material::Material(const char* vPath, const char* fPath){
	this->progID = glCreateProgram();
	//this.queue = { textures:[] };
	//this.textureUnits = [];
	GLint vHandle = getShader(vPath, GL_VERTEX_SHADER);
	GLint fHandle = getShader(fPath, GL_FRAGMENT_SHADER);

	glAttachShader(progID, vHandle);
	glAttachShader(progID, fHandle);
	glLinkProgram(progID);

	GLint status;
	glGetProgramiv(progID, GL_LINK_STATUS, &status);
	if (!status) {
		char errorLog[2048] = "";
		int read;
		glGetProgramInfoLog(progID, 2048, &read, errorLog);

		cerr << "Link error:" << endl;
		cerr << errorLog;
		return;
	}

	glValidateProgram(progID);
	glDetachShader(progID, vHandle);
	glDetachShader(progID, fHandle);
	glDeleteShader(vHandle);
	glDeleteShader(fHandle);
}

void Material::commitQueue(){
	glUseProgram(progID);
}