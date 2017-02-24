#ifndef RENDERER_H
#define RENDERER_H

#include "FrameBuffer.h"
#include <algorithm>
#include "Texture2D.h"
#include <vector>
#include "Transform.h"

class Renderer{

	FrameBuffer *buffer;
	Renderer();

public:
	float maxd;
	float mind = 1;
	Matrix44 MVP;
	int activeTexture = 0;
	vector<Texture2D> textures;
	bool backface = true;
	Renderer(FrameBuffer &buff) {
		buffer = &buff;
	}
		void RenderTriangle(Vec3 v0, Vec3 v1, Vec3 v2, Color col);
		void RenderTriangle(Vertex p0, Vertex p1, Vertex p2, Texture2D & tex);
		void RenderMesh(vector<GLfloat> verts);
		void RenderPolygon(vector<Vertex> verts, Texture2D &tex);
	
};
#endif		/* RENDERER_H */


