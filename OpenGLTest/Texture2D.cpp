
#include "Texture2D.h"

Color Texture2D::GetPixel(int x, int y)
{
	Color c;
	c.R = data[3 * (y*width + x)];
	c.G = data[3 * (y*width + x)+1];
	c.B = data[3 * (y*width + x)+2];
	c.A = 255;

	return c;
}
