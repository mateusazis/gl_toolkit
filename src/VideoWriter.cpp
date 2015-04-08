#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <GL/glew.h>

#include "VideoWriter.hpp"

VideoWriter::VideoWriter(const char* path, int width, int height, int fps) : width(width), height(height){
	img = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);

	int isColor = 1;

	int format = CV_FOURCC('D', 'I', 'V', 'X');
	//format = CV_FOURCC('X', '2', '6', '4');

	writer = cvCreateVideoWriter(path, format, fps, cvSize(width, height), isColor);
	raw_image = new unsigned char[width*height * 3];
}

VideoWriter::~VideoWriter(){
	if (raw_image)
		close();
}

void VideoWriter::addFrame() {
	glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, raw_image);
	img->imageData = (char *)raw_image;
	cvFlip(img, img);
	cvWriteFrame(writer, img); // add the frame to the file
}

void VideoWriter::close(){
	delete[] raw_image;
	cvReleaseImage(&img);
	cvReleaseVideoWriter(&writer);

	raw_image = NULL;
}