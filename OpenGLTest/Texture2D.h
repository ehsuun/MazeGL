#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include "FrameBuffer.h"

class Texture2D {
public:
	int width = 0;
	int height = 0;

	GLubyte *data;

	Texture2D() {
		width = 0;
		height = 0;
	}

	Texture2D(int w, int h) {
		width = w;
		height = h;
		data = (GLubyte *)malloc(sizeof(GLubyte) * width * height * 3);
	}

	Color GetPixel(int x, int y);

};


#endif		/* TEXTURE2D_H */