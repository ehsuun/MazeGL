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

void Renderer::RenderTriangle(Vertex p0, Vertex p1, Vertex p2, Texture2D &tex)
{
	
	Vec3 temp;
	Transform::multPointMatrix(p0.position, temp, MVP);
	p0.position = temp;

	Transform::multPointMatrix(p1.position, temp, MVP);
	p1.position = temp;

	Transform::multPointMatrix(p2.position, temp, MVP);
	p2.position = temp;

	

	float Z2 =0;

	if ((p0.position.x<1.0 && p0.position.x>-1 && p0.position.y<1 && p0.position.y>-1 && p0.position.z > 0 && p0.position.z < 1) &&
		(p1.position.x<1.0 && p1.position.x>-1 && p1.position.y<1 && p1.position.y>-1 && p1.position.z > 0 && p1.position.z < 1) &&
		(p2.position.x<1.0 && p2.position.x>-1 && p2.position.y<1 && p2.position.y>-1 && p2.position.z > 0 && p2.position.z < 1))
	{


		Point2D v0 = Point2D(p0.position, buffer->width, buffer->height);
		Point2D v1 = Point2D(p1.position, buffer->width, buffer->height);
		Point2D v2 = Point2D(p2.position, buffer->width, buffer->height);


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
					float area = w0 + w1 + w2;


					//float iZ = ((1/p0.position.z*w0) + (1 / p1.position.z*w1) + (1 / p2.position.z*w2))*(1 / area);
					//p0.uv = p0.uv*(1.0 / p0.position.z);
					//p1.uv = p1.uv*(1.0 / p1.position.z);
					//p2.uv = p2.uv*(1.0 / p2.position.z);


					Vec2 sumUV = ((p0.uv*w0) + (p1.uv*w1) + (p2.uv*w2))*(1/ area);
					float depth = ((p0.position.z*w0) + (p1.position.z*w1) + (p2.position.z*w2))*(1 / area);
					//depth = p0.position.z;
					//sumUV = sumUV* (1/iZ);

					int _X = int(sumUV.x*tex.width);
					int _Y = int(sumUV.y*tex.height);


					if (!buffer->IsDepthLessThanBuffer(p.x, p.y, depth)) {
						buffer->DrawPixel(p.x, p.y, tex.GetPixel(_X, _Y));
						//buffer->DrawPixel(p.x, p.y, Color(int(*255)));
						buffer->DrawDepthPixel(p.x, p.y, depth);
					}
					
					
					
					//buffer->DrawPixel(p.x, p.y, Color(test));
				}


			}
		}
	}
}

void Renderer::RenderMesh(vector<GLfloat> verts)
{
	//verts.size() / 15 is the number of triangles as each vertex has 3 pos and 2 uvs (3+2)*3 = 15
	for (int i = 0; i < (verts.size() / 15); i++) {
		Vertex point1(
			Vec3(verts[i * 15],
				verts[(i * 15) + 1],
				verts[(i * 15) + 2])
			,
			Vec2((verts[(i * 15) + 3]), (verts[(i * 15) + 4]))
			);

		Vertex point2(
			Vec3(verts[i * 15 + 5],
				verts[i * 15 + 6],
				verts[i * 15 + 7]),
			Vec2((verts[(i * 15) + 8]), (verts[(i * 15) + 9]))
			);

		Vertex point3(
			Vec3(verts[i * 15 + 10],
				verts[i * 15 + 11],
				verts[i * 15 + 12]),
			Vec2((verts[(i * 15) + 13]), (verts[(i * 15) + 14]))
			);

		RenderTriangle(point1, point2, point3,textures[activeTexture]);
		if (backface) {
			RenderTriangle(point3, point2, point1, textures[activeTexture]);
		}
	}
}

void Renderer::RenderPolygon(vector<Vertex> verts, Texture2D & tex)
{
	int triCount = verts.size() - 2;

	if (triCount > 0) {
		for (int i = 0; i < triCount; i++) {
			RenderTriangle(verts[0], verts[i + 1], verts[i + 2], tex);
		}
	}
}


