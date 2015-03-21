#include "Material.h"
#include "Texture.h"

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

	GLint handle = glCreateShader(type);
	char* c = src;
	glShaderSource(handle, 1, &c, NULL);
	glCompileShader(handle);

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

	for (map<string, TextureEntry>::iterator it = textureEntries.begin(); it != textureEntries.end(); it++){
		string locName = it->first;
		TextureEntry entry = it->second;

		GLint loc = glGetUniformLocation(progID, locName.data());
		glActiveTexture(GL_TEXTURE0 + entry.unit);
		glBindTexture(GL_TEXTURE_2D, entry.t->getID());
		glUniform1i(loc, entry.unit);
	}
}

void Material::texture(const char* loc, Texture* t){
	glUseProgram(progID);

	int unit;
	int handle = t->getID();
	map<GLuint, int>::iterator it = textureUnits.find(handle);
	if (it == textureUnits.end()){
		unit = textureUnits.size();
		textureUnits[handle] = unit;
	}
	else 
		unit = it->second;

	TextureEntry entry;
	entry.t = t;
	entry.unit = unit;

	textureEntries[loc] = entry;
	//this.queue.textures[locName] = { texture: tex, unit : textureUnit };
}