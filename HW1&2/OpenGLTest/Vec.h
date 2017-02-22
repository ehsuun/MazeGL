#include <glew.h>
#include <glfw3.h>
#include <iostream>
#include <string>
#include <iomanip>
#include <glm/glm.hpp>


#ifndef VEC_H
#define VEC_H

using namespace std;

struct Vec2
{
	GLfloat x;
	GLfloat y;

	Vec2();
	Vec2 operator+(Vec2 rhs);
	Vec2& operator*=(float rhs);
	Vec2 operator*(float rhs);

	Vec2(GLfloat a, GLfloat b);
};

struct Vec3
{
	GLfloat x;
	GLfloat y;
	GLfloat z;

	Vec3();
	Vec3(GLfloat a, GLfloat b, GLfloat c);
	Vec3(glm::vec3 vec);

	Vec3& operator*(float rhs);
	void operator=(Vec3 &rhs);

	//Vec3& operator=(glm::vec3 &rhs);

	Vec3& operator*=(float rhs);

	Vec3& operator+(Vec3 rhs);
	Vec3& operator+=(Vec3 rhs);
	Vec3& operator-(Vec3& rhs);

	static Vec3 reflect(Vec3 vector, Vec3 normal);

	float dot(const Vec3 & v);
	Vec3 cross(Vec3 & v);
	float Magnitude();
	Vec3 Normalized();
	string ToString();


};

struct Vec4
{
	GLfloat x;
	GLfloat y;
	GLfloat z;
	GLfloat w;

	Vec4& operator=(Vec4& rhs);

	Vec4();
	Vec4(GLfloat a, GLfloat b, GLfloat c, GLfloat d);
};



struct Vertex {
	Vec3 position;
	Vec2 uv;

	Vertex() {

	}
	Vertex(Vec3 &pos, Vec2 &UV) {
		position = pos;
		uv = UV;
	}
};

#endif VEC_H

