#include "Renderer.h"


struct Point2D {
	int x, y;

	Point2D(Vec3 v,int width,int height) {
		 x = int(width *(v.x + 1) / 2);
		 y = int(height*(v.y + 1) / 2);
	}

	Point2D() {
		x = 0; y = 0;
	}
};

int orient2d(const Point2D& a, const Point2D& b, const Point2D& c)
{
	return (b.x - a.x)*(c.y - a.y) - (b.y - a.y)*(c.x - a.x);
}

float min3(float x, float y, float z) {
	return std::min(std::min(x, y), z);
}

float max3(float x, float y, float z) {
	return std::max(std::max(x, y), z);
}


void Renderer::RenderTriangle(Vec3 p0, Vec3 p1, Vec3 p2, Color col)
{
	if ((p0.x<1.0 && p0.x>-1 && p0.y<1 && p0.y>-1 && p0.z > 0 && p0.z < 1)&&
		(p1.x<1.0 && p1.x>-1 && p1.y<1 && p1.y>-1 && p1.z > 0 && p1.z < 1)&&
		(p2.x<1.0 && p2.x>-1 && p2.y<1 && p2.y>-1 && p2.z > 0 && p2.z < 1))
	{


		Point2D v0 = Point2D(p0, buffer->width, buffer->height);
	Point2D v1 = Point2D(p1, buffer->width, buffer->height);
	Point2D v2 = Point2D(p2, buffer->width, buffer->height);


	// Compute triangle bounding box
	int minX = min3(v0.x, v1.x, v2.x);
	int minY = min3(v0.y, v1.y, v2.y);
	int maxX = max3(v0.x, v1.x, v2.x);
	int maxY = max3(v0.y, v1.y, v2.y);

	// Clip against screen bounds
	minX = max(minX, 0);
	minY = max(minY, 0);
	maxX = std::min(maxX, buffer->width - 1);
	maxY = std::min(maxY, buffer->height - 1);

	// Rasterize
	Point2D p;
	for (p.y = minY; p.y <= maxY; p.y++) {
		for (p.x = minX; p.x <= maxX; p.x++) {
			// Determine barycentric coordinates
			int w0 = orient2d(v1, v2, p);
			int w1 = orient2d(v2, v0, p);
			int w2 = orient2d(v0, v1, p);

			// If p is on or inside all edges, render pixel.
			if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
				//renderPixel(p, w0, w1, w2);
				buffer->DrawPixel(p.x, p.y, col);
			}


		}
	}
}
}
