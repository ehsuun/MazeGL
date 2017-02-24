#include "Transform.h"
#include <math.h>

#define PI 3.14159265359

 float** Transform::Multiply(float** A, int m, int n, float** B, int p)
{
	float** C = new float*[m];
	for (int i = 0; i<m; i++)
	{
		C[i] = new float[p];
	}
	for (int i = 0; i<m; i++)
	{
		for (int j = 0; j<p; j++)
		{
			C[i][j] = 0;
			for (int k = 0; k<n; k++)
			{
				C[i][j] += A[i][k] * B[k][j];
			}
		}
	}
	return C;
}

 void Transform::Multiply(float A[3][3], float B[3][3], float output[3][3])
{
	for (int i = 0; i<3; i++)
	{
		for (int j = 0; j<3; j++)
		{
			output[i][j] = 0;
			for (int k = 0; k<3; k++)
			{
				output[i][j] += A[i][k] * B[k][j];
			}
		}
	}
}


 void Transform::Multiply(float A[3][3], float B[3][1], float output[3][1])
{
	for (int i = 0; i<3; i++)
	{
		output[i][0] = 0;
		for (int k = 0; k<3; k++)
		{
			output[i][0] += A[i][k] * B[k][0];
		}
	}
}

 void Transform::Multiply(float A[4][4], float B[4][4], float output[4][4])
{
	for (int i = 0; i<4; i++)
	{
		for (int j = 0; j<4; j++)
		{
			output[i][j] = 0;
			for (int k = 0; k<4; k++)
			{
				output[i][j] += A[i][k] * B[k][j];
			}
		}
	}
}

 void Transform::Multiply(float A[4][4], float B[4][1], float output[4][1])
{
	for (int i = 0; i<4; i++)
	{
		output[i][0] = 0;
		for (int k = 0; k<4; k++)
		{
			output[i][0] += A[i][k] * B[k][0];
		}
	}
}

 void Transform::MultiplyByProjection(float B[4][1], float output[4][1], float fov, float aspect, float nearZ, float farZ)
{

	


	float f = 1 / ((float)tanf(fov / 2.0));

	// generate the perspective projection matrix
	float A[4][4] = { 
	{ f / aspect,		0,								0,	0								},
	{ 0,				f,								0,	0								},
	{ 0,				0,	((nearZ + farZ) / (nearZ - farZ)),	2 * farZ*nearZ / (nearZ - farZ) },
	{ 0,				0,								-1,0								}};

	for (int i = 0; i<4; i++)
	{
		output[i][0] = 0;
		for (int k = 0; k<4; k++)
		{
			output[i][0] += A[i][k] * B[k][0];
		}
	}
	// Multiplying wc to output result for finilizing the projection
	for (int i = 0; i<4; i++)
	{
		output[i][0] = output[i][0] / output[3][0];
	}


}

 void Transform::multPointMatrix(const Vec3 & in, Vec3 & out, const Matrix44 & M)
 {
	 //out = in * Mproj;
	 out.x = in.x * M.M[0][0] + in.y * M.M[1][0] + in.z * M.M[2][0] + /* in.z = 1 */ M.M[3][0];
	 out.y = in.x * M.M[0][1] + in.y * M.M[1][1] + in.z * M.M[2][1] + /* in.z = 1 */ M.M[3][1];
	 out.z = in.x * M.M[0][2] + in.y * M.M[1][2] + in.z * M.M[2][2] + /* in.z = 1 */ M.M[3][2];
	 float w = in.x * M.M[0][3] + in.y * M.M[1][3] + in.z * M.M[2][3] + /* in.z = 1 */ M.M[3][3];



	 // normalize if w is different than 1 (convert from homogeneous to Cartesian coordinates)

	 if (w != 1) {
		 out.x /= w;
		 out.y /= w;
		 out.z /= w;
		 
	 }
	 
 }

 void Transform::multPointMatrix2(const Vec3 & in, Vec3 & out, const Matrix44 & M)
 {
	 float Point[4][1] = { {in.x},{in.y},{in.z},{1} };
	 float Mat[4][4];

	 for (int k = 0; k < 4; k++) {
		 for (int l = 0; l < 4; l++) {
			 Mat[l][k] = M.M[l][k];
		 }
	 }

	 float Output[4][1] = { { 1 },{ 1 },{ 1 },{ 1 } };
	 Transform::Multiply(Mat, Point, Output);

	 out.x = Output[0][0];
	 out.y = Output[0][1];
	 out.z = Output[0][2];

	 float w = Output[0][3];

	 // normalize if w is different than 1 (convert from homogeneous to Cartesian coordinates)
	 if (w != 1) {
		 out.x /= w;
		 out.y /= w;
		 out.z /= w;
	 }
 }


 Vec3 Transform::Project(Vec3 origin, float fov, float aspect, float nearZ, float farZ,Vec3 camPosition,Vec3 targetPosition)
 {
	 
	 float point[4][1] = { { origin.x },{ origin.y },{ origin.z },{ 1 } };
	 float output2[4][1];

	 Matrix44 worldview = Matrix44();

	 Matrix44 lookat = Transform::LookatMatrix(camPosition, targetPosition, Vec3(0, 1, 0));
	
	 Transform::Multiply(lookat.M, point, output2);
	 float output[4][1];
	 
	 MultiplyByProjection(output2, output, fov, aspect, nearZ, farZ);
	 return Vec3(output[0][0], output[1][0], output[2][0]);
 }

 Vec3 Transform::Translate(Vec3 origin, Vec3 translation)
 {
	 // generate the translation matrix
	float A[4][4] = {
	{ 1,	0,	0,	translation.x },
	{ 0,	1,	0,	translation.y },
	{ 0,	0,	1,	translation.z },
	{ 0,	0,	0,	1}
	};
	 
	 float B[4][1] = { {origin.x},{origin.y},{origin.z},{ 1 } };
	 float output[4][1];


	 Multiply(A, B, output);
	 
	 return Vec3(output[0][0],output[1][0],output[2][0]);
 }

 Matrix44 Transform::TranslationMatrix(Vec3& translation) {
	 float A[4][4] = {
		 { 1,	0,	0,	translation.x },
		 { 0,	1,	0,	translation.y },
		 { 0,	0,	1,	translation.z },
		 { 0,	0,	0,	1 }
	 };
	 Matrix44 Trans = Matrix44();

	 for (int i = 0; i < 3; i++)
	 {
		 for (int j = 0; j < 3; j++)
		 {
			 Trans.M[i][j] = A[i][j];
		 }
	 }
	 return Trans;
 }

 Matrix44 Transform::ScaleMatrix(Vec3& scale) {
	 
	 float A[4][4] = {
		 { scale.x,	0,	0,	0 },
		 { 0,	scale.y,	0,	0 },
		 { 0,	0,	scale.z,	0 },
		 { 0,	0,	0,	1 }
	 };

	 Matrix44 Trans = Matrix44();

	 for (int i = 0; i < 3; i++)
	 {
		 for (int j = 0; j < 3; j++)
		 {
			 Trans.M[i][j] = A[i][j];
		 }
	 }
	 return Trans;
 }

 Matrix44 Transform::RotationMatrix(Vec3& rotation) {
	 rotation *= (PI / 180.0f);
	 // generate the rotation around X matrix
	 float X[4][4] = {
		 { 1,	0,	0,	0 },
		 { 0,	cos(rotation.x),	-sin(rotation.x),	0 },
		 { 0,	sin(rotation.x),	cos(rotation.x),	0 },
		 { 0,	0,	0,	1 }
	 };


	 //generate the rotation around Y matrix

	 float Y[4][4] = {
		 { cos(rotation.y),	0,	sin(rotation.y),	0 },
		 { 0,	1,	0,	0 },
		 { -sin(rotation.y),	0,	cos(rotation.y),	0 },
		 { 0,	0,	0,	1 }
	 };

	 //generate the rotation around Z matrix

	 float Z[4][4] = {
		 { cos(rotation.z),	-sin(rotation.z),	0,	0 },
		 { sin(rotation.z),	cos(rotation.z),	0,	0 },
		 { 0,	0,	1,	0 },
		 { 0,	0,	0,	1 }
	 };

	 //float output[4][1];
	 float temp[4][4];
	 Matrix44 RotM = Matrix44();

	 Multiply(X, Y, temp);
	 Multiply(temp, Z, RotM.M);
	 return RotM;
 }

 Vec3 Transform::Rotate(Vec3 origin, Vec3 rotation, bool isVector)
 {
	 Matrix44 Rot = RotationMatrix(rotation);

	 float R[4][1] = { { origin.x },{ origin.y },{ origin.z },{ 1 } };
	 float R2[4][1] = { { origin.x },{ origin.y },{ origin.z },{ 0 } };

	 float output[4][1];

	 if (isVector) {
		 Multiply(Rot.M, R2, output);
	 }
	 else {
		 Multiply(Rot.M, R, output);
	 }

	 return Vec3(output[0][0], output[1][0], output[2][0]);
 }

 Vec3 Transform::Rotate(Vec3 origin, Vec3 rotation)
 {
	 rotation*=(PI / 180.0f);
	 // generate the rotation around X matrix
	 float X[4][4] = {
		 { 1,	0,	0,	0 },
		 { 0,	cos(rotation.x),	-sin(rotation.x),	0 },
		 { 0,	sin(rotation.x),	cos(rotation.x),	0 },
		 { 0,	0,	0,	1 }
	 };


	 //generate the rotation around Y matrix

	 float Y[4][4] = {
		 { cos(rotation.y),	0,	sin(rotation.y),	0 },
		 { 0,	1,	0,	0 },
		 { -sin(rotation.y),	0,	cos(rotation.y),	0 },
		 { 0,	0,	0,	1 }
	 };

	 //generate the rotation around Z matrix

	 float Z[4][4] = {
		 { cos(rotation.z),	-sin(rotation.z),	0,	0 },
		 { sin(rotation.z),	cos(rotation.z),	0,	0 },
		 { 0,	0,	1,	0 },
		 { 0,	0,	0,	1 }
	 };

	 float R[4][1] = { { origin.x },{ origin.y },{ origin.z },{ 1 } };
	 float output[4][1];

	 float temp[4][4];
	 /*
	 Multiply(X, R, output);
	 R[0][0] = output[0][0]; 
	 R[1][0] = output[1][0];
	 R[2][0] = output[2][0];
	 R[3][0] = output[3][0];
	 Multiply(Y, R, output);
	 R[0][0] = output[0][0];
	 R[1][0] = output[1][0];
	 R[2][0] = output[2][0];
	 R[3][0] = output[3][0];
	 Multiply(Z, R, output);
	 */
	 Multiply(X, Y, temp);
	 Multiply(temp, Z, X);
	 Multiply(X, R, output);

	 return Vec3(output[0][0], output[1][0], output[2][0]);
 }

 Vec3 Transform::Scale(Vec3 origin, Vec3 scale)
 {
	 // generate the translation matrix
	 float A[4][4] = {
		 { scale.x,	0,	0,	0 },
		 { 0,	scale.y,	0,	0 },
		 { 0,	0,	scale.z,	0 },
		 { 0,	0,	0,	1 }
	 };

	 float B[4][1] = { { origin.x },{ origin.y },{ origin.z },{ 1 } };
	 float output[4][1];

	 Multiply(A, B, output);

	 return Vec3(output[0][0], output[1][0], output[2][0]);
 }

 Matrix44::Matrix44()
 {
	 for (int i = 0; i < 4; i++)
	 {
		 for (int j = 0; j < 4; j++)
		 {
			 M[i][j] = 0;
		 }
	 }
 }

 Matrix44::Matrix44(float fill)
 {
	 for (int i = 0; i < 4; i++)
	 {
		 for (int j = 0; j < 4; j++)
		 {
			 M[i][j] = fill;
		 }
	 }
 }

 void Matrix44::operator=(glm::mat4x4 & rhs)
 {
	 for (int i = 0; i < 4; i++)
	 {
		 for (int j = 0; j < 4; j++)
		 {
			 M[i][j] = rhs[i][j];
		 }
	 }
 }


 void Matrix44::operator=(Matrix44 &rhs)
 {
	 for (int i = 0; i < 4; i++)
	 {
		 for (int j = 0; j < 4; j++)
		 {
			 M[i][j] = rhs.M[i][j];
		 }
	 }
 }

 Matrix44 Matrix44::Multiply(Matrix44 & rhs)
 {
	 Matrix44 mult = Matrix44();
	 Transform::Multiply(M, rhs.M, mult.M);
	 return mult;
 }

 Matrix44 Transform::LookatMatrix(Vec3 eye, Vec3 target, Vec3 up) {

		 Vec3 zaxis = (eye - target);	    // The "forward" vector.
		 zaxis = zaxis.Normalized();
		 Vec3 xaxis = up.cross(zaxis);// The "right" vector.
		 xaxis = xaxis.Normalized();
		 Vec3 yaxis = zaxis.cross(xaxis);			// The "up" vector.

											   // Create a 4x4 orientation matrix from the right, up, and forward vectors
											   // This is transposed which is equivalent to performing an inverse 
											   // if the matrix is orthonormalized (in this case, it is).
		 float orientation[4][4] = {
			 { xaxis.x, yaxis.x, zaxis.x, 0 },
			 { xaxis.y, yaxis.y, zaxis.y, 0 },
			 { xaxis.z, yaxis.z, zaxis.z, 0 },
			 { 0,       0,       0,     1 }
		 };


			 // Create a 4x4 translation matrix.
		 // The eye position is negated which is equivalent
		 // to the inverse of the translation matrix. 
		 // T(v)^-1 == T(-v)
		 float translation[4][4] = {
			 { 1,      0,      0,   0 },
			 { 0,      1,      0,   0 },
			 { 0,      0,      1,   0 },
			 { -eye.x, -eye.y, -eye.z, 1}
		 };
		 // Combine the orientation and translation to compute 
		 // the final view matrix

		 Matrix44 lookat = Matrix44();
		 Transform::Multiply(orientation, translation, lookat.M);
		 return lookat;
 }

 Matrix44 Transform::LookatMatrixFW(Vec3 eye, Vec3 forward, Vec3 up) {

	 Vec3 zaxis = forward;	    // The "forward" vector.
	 zaxis = zaxis.Normalized();
	 Vec3 xaxis = up.cross(zaxis);// The "right" vector.
	 xaxis = xaxis.Normalized();
	 Vec3 yaxis = zaxis.cross(xaxis);			// The "up" vector.

												// Create a 4x4 orientation matrix from the right, up, and forward vectors
												// This is transposed which is equivalent to performing an inverse 
												// if the matrix is orthonormalized (in this case, it is).
	 float orientation[4][4] = {
		 { xaxis.x, yaxis.x, zaxis.x, 0 },
		 { xaxis.y, yaxis.y, zaxis.y, 0 },
		 { xaxis.z, yaxis.z, zaxis.z, 0 },
		 { 0,       0,       0,     1 }
	 };


	 // Create a 4x4 translation matrix.
	 // The eye position is negated which is equivalent
	 // to the inverse of the translation matrix. 
	 // T(v)^-1 == T(-v)
	 float translation[4][4] = {
		 { 1,      0,      0,   0 },
		 { 0,      1,      0,   0 },
		 { 0,      0,      1,   0 },
		 { -eye.x, -eye.y, -eye.z, 1 }
	 };
	 // Combine the orientation and translation to compute 
	 // the final view matrix

	 Matrix44 lookat = Matrix44();
	 Transform::Multiply(orientation, translation, lookat.M);
	 return lookat;
 }


 Matrix44 Transform::lookAt(Vec3& eye,Vec3& center, Vec3& up)
 {
	 Vec3 f = center - eye;
	 f = f.Normalized();
	 Vec3 s = f.cross(up);
	 s = s.Normalized();
	 Vec3 u = s.cross(f);

	 Matrix44 Result(1);
	 Result.M[0][0] = s.x;
	 Result.M[1][0] = s.y;
	 Result.M[2][0] = s.z;
	 Result.M[0][1] = u.x;
	 Result.M[1][1] = u.y;
	 Result.M[2][1] = u.z;
	 Result.M[0][2] = -f.x;
	 Result.M[1][2] = -f.y;
	 Result.M[2][2] = -f.z;
	 Result.M[3][0] = -s.dot(eye);
	 Result.M[3][1] = -u.dot(eye);
	 Result.M[3][2] = f.dot(eye);
	 return Result;
 }