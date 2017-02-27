#pragma once
#include "Vec.h"


struct Matrix44
{
	float M[4][4];

	Matrix44();
	Matrix44(float fill);
	void operator=(glm::mat4x4 &rhs);
	void operator=(Matrix44 &rhs);
	Matrix44 Multiply(Matrix44 &rhs);
	
};



class Transform {
public:
	
	 static float** Multiply(float** A, int m, int n, float** B, int p);
	 static void Multiply(float A[3][3], float B[3][3], float output[3][3]);
	 static void Multiply(float A[3][3], float B[3][1], float output[3][1]);
	 static void Multiply(float A[4][4], float B[4][4], float output[4][4]);
	 static void Multiply( float A[4][4],  float B[4][1],  float output[4][1]);
	 static void MultiplyByProjection(float B[4][1], float output[4][1], float fov, float aspect, float nearZ, float farZ);

	 static void multPointMatrix(const Vec3 &in, Vec3 &out, const Matrix44 &M);

	 static void multVertexMatrix(const Vertex & in, Vertex & out, const Matrix44 & M);

	 static void multPointMatrix2(const Vec3 & in, Vec3 & out, const Matrix44 & M);

	 static Vec3 Project(Vec3 origin, float fov, float aspect, float nearZ, float farZ, Vec3 camPosition, Vec3 targetPosition);
	 static Vec3 Translate(Vec3 origin, Vec3 translation);
	 static Matrix44 TranslationMatrix(Vec3 & translation);
	 static Matrix44 ScaleMatrix(Vec3 & scale);
	 static Matrix44 RotationMatrix(Vec3 & rotation);
	 static Vec3 Rotate(Vec3 origin, Vec3 rotation, bool isVector);
	 static Vec3 Rotate(Vec3 origin, Vec3 rotation);
	 static Vec3 Scale(Vec3 origin, Vec3 scale);
	 static Matrix44 LookatMatrix(Vec3 eye, Vec3 target, Vec3 up);
	 static Matrix44 LookatMatrixFW(Vec3 eye, Vec3 forward, Vec3 up);
	 static Matrix44 lookAt(Vec3 & eye, Vec3 & center, Vec3 & up);
};