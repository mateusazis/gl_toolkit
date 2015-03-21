#include "Texture.h"
#include <IL/il.h>

static bool devilInit = false;

Texture::Texture(int width, int height){
	init(width, height);

	int mipmapLevel = 0;
	glTexImage2D(GL_TEXTURE_2D, mipmapLevel, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //GL_NEAREST is also allowed, instead of GL_LINEAR, as neither mipmap.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //GL_NEAREST is also allowed, instead of GL_LINEAR, as neither mipmap.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); //Prevents s-coordinate wrapping (repeating).
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); //Prevents t-coordinate wrapping (repeating).

	glBindTexture(GL_TEXTURE_2D, NULL);
}

Texture::Texture(const char* path){
	if (!devilInit){
		ilInit();
		ilEnable(IL_ORIGIN_SET);
		ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
		devilInit = true;
	}

	ILuint img = ilGenImage();
	ilBindImage(img);
	ilLoadImage(path);

	width = ilGetInteger(IL_IMAGE_WIDTH);
	height = ilGetInteger(IL_IMAGE_HEIGHT);

	ILubyte *data = new ILubyte[width * height * 4];
	init(width, height);

	ilCopyPixels(0, 0, 0, width, height, 1, IL_RGBA, IL_UNSIGNED_BYTE, data);
	ilDeleteImage(img);

	int mipmapLevel = 0;
	glTexImage2D(GL_TEXTURE_2D, mipmapLevel, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	

	delete [] data;
	glBindTexture(GL_TEXTURE_2D, NULL);
}

void Texture::init(int w, int h){
	glGenTextures(1, &handle);
	glBindTexture(GL_TEXTURE_2D, handle);

	// GL_texImage2D(GL_TEXTURE_2D, mipmapLevel, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, img);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //GL_NEAREST is also allowed, instead of GL_LINEAR, as neither mipmap.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //GL_NEAREST is also allowed, instead of GL_LINEAR, as neither mipmap.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); //Prevents s-coordinate wrapping (repeating).
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); //Prevents t-coordinate wrapping (repeating).

	width = w; height = h;
}

GLuint Texture::getID(){
	return handle;
}