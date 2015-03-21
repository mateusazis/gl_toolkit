#include "Texture.h"

class FBO : public Texture {
public:
	FBO(int width, int height, bool useDepthBuffer = false);

	void bind();
	void unbind();

private:
	GLuint fbo, rbo;	
	int viewportParams[4];
};