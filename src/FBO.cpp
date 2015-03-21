#include "FBO.h"

#include <iostream>

using namespace std;

FBO::FBO(int w, int h, bool useDepthBuffer) : Texture(w, h){
	glBindTexture(GL_TEXTURE_2D, getID());
	//-------------------------
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	//Attach 2D texture to this FBO
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, getID(), 0);
	//-------------------------

	if (useDepthBuffer){
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
		//-------------------------
		//Attach depth buffer to FBO
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);
	}

	//-------------------------
	//Does the GPU support current FBO configuration?
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
		cerr << "There is a problem with this framebuffer!" << endl;
	else
		cout << "Framebuffer complete!" << endl;
	
	unbind();
	glBindTexture(GL_TEXTURE_2D, 0);
}

void FBO::bind(){
	glGetIntegerv(GL_VIEWPORT, viewportParams);
	//printf("Viewport: %d %d %d %d\n", viewportParams[0], viewportParams[1], viewportParams[2], viewportParams[3]);
	//glViewport(0, 0, width, height);
	//cout << " bind to " << fbo << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void FBO::unbind(){
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//cout << " unbind " << fbo << endl;
	//glViewport(viewportParams[0], viewportParams[1], viewportParams[2], viewportParams[3]);
}