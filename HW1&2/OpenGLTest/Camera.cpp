#include "Camera.h"



void Camera::gluPerspective()
{
	float scale = tan(fov * 0.5 * MY_PI / 180) * _near;
	r = aspect * scale, l = -r;
	t = scale, b = -t;
}

void Camera::glFrustum()
{
	// set OpenGL perspective projection matrix
	Mproj.M[0][0] = 2 * _near / (r - l);
	Mproj.M[0][1] = 0;
	Mproj.M[0][2] = 0;
	Mproj.M[0][3] = 0;

	Mproj.M[1][0] = 0;
	Mproj.M[1][1] = 2 * _near / (t - b);
	Mproj.M[1][2] = 0;
	Mproj.M[1][3] = 0;

	Mproj.M[2][0] = (r + l) / (r - l);
	Mproj.M[2][1] = (t + b) / (t - b);
	Mproj.M[2][2] = -(_far + _near) / (_far - _near);
	Mproj.M[2][3] = -1;

	Mproj.M[3][0] = 0;
	Mproj.M[3][1] = 0;
	Mproj.M[3][2] = -2 * _far * _near / (_far - _near);
	Mproj.M[3][3] = 0;
}
