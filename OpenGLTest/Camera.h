#ifndef CAMERA_H
#define CAMERA_H


#include "Transform.h"
#include "Vec.h"
#include "globals.h"

class Camera {

public: float fov, aspect, _near, _far ,b,t,l,r;
		Matrix44 Mproj;
		Matrix44 worldToCamera;

	void gluPerspective();
	void glFrustum();

	Camera() {
		fov = 90.0;
		aspect = 4.0 / 3.0;
		_near = 0.01;
		_far = 100;
	}

	Camera(float _fov,float _aspect,float __near, float __far) {

		fov = _fov;
		aspect = _aspect;
		_near = __near;
		_far = __far;

	}

};


#endif		/* CAMERA_H */