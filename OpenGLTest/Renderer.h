#ifndef RENDERER_H
#define RENDERER_H

#include "FrameBuffer.h"
#include <algorithm>

class Renderer{

	FrameBuffer *buffer;


	Renderer();

public:
	Renderer(FrameBuffer &buff) {
		buffer = &buff;
	}
		void RenderTriangle(Vec3 v0, Vec3 v1, Vec3 v2, Color col);

};
#endif		/* RENDERER_H */


