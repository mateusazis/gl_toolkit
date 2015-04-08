#pragma once

#include <opencv2/core/types_c.h>

struct CvVideoWriter;

class VideoWriter {
public:
	VideoWriter(const char* path, int width, int height, int fps = 60);
	~VideoWriter();
	void addFrame();
	void close();

private:
	CvVideoWriter *writer;
	IplImage* img;
	unsigned char* raw_image;
	int width, height;
};