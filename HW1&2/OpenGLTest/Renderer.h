#ifndef RENDERER_H
#define RENDERER_H

#include "FrameBuffer.h"
#include <algorithm>
#include "Texture2D.h"

class Renderer{

	FrameBuffer *buffer;


	Renderer();

public:
	int activeTexture = 0;
	Renderer(FrameBuffer &buff) {
		buffer = &buff;
	}
		void RenderTriangle(Vec3 v0, Vec3 v1, Vec3 v2, Color col);
		void RenderTriangle(Vertex &p0, Vertex &p1, Vertex &p2, Texture2D &tex);
	
};
#endif		/* RENDERER_H */


