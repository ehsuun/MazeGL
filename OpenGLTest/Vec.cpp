#include "Vec.h"

Vec2::Vec2()
{
	x = 0;
	y = 0;
}

Vec2 Vec2::operator+(Vec2 rhs)
{
	Vec2 sum = Vec2();
	sum.x = x + rhs.x;
	sum.y = y + rhs.y;
	return sum;
}

void Vec2::operator=(Vec2 rhs)
{
	x = rhs.x;
	y = rhs.y;
}

Vec2 & Vec2::operator*=(float rhs)
{
	Vec2 mult = Vec2();
	mult.x *= rhs;
	mult.y *= rhs;
	return mult;
}

Vec2 Vec2::operator*(float rhs)
{
	Vec2 mult = Vec2();
	mult.x = x*rhs;
	mult.y = y*rhs;
	return mult;
}



Vec2::Vec2(GLfloat a, GLfloat b)
{
	x = a;
	y = b;
}

Vec2 Vec2::Lerp(Vec2 & a, Vec2 & b, float t)
{
	return Vec2(
		a.x + t * (b.x - a.x),
		a.y + t * (b.y - a.y)
		);
}

Vec3::Vec3()
{
	x = 0;
	y = 0;
	z = 0;
}

Vec3::Vec3(GLfloat a, GLfloat b, GLfloat c)
{
	x = a;
	y = b;
	z = c;
}

Vec3::Vec3(glm::vec3 vec)
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
}

Vec3 Vec3::operator* (float rhs)
{
	Vec3 mult = Vec3();
	mult.x = x*rhs;
	mult.y = y*rhs;
	mult.z = z*rhs;
	return mult;
}

void Vec3::operator=(Vec3 &rhs)
{
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;
}



Vec3 Vec3::operator+(Vec3 rhs)
{
	Vec3 sum = Vec3();
	sum.x = x + rhs.x;
	sum.y = y + rhs.y;
	sum.z = z + rhs.z;
	return sum;
}

Vec3 Vec3::operator-(Vec3& rhs)
{
	Vec3 sum = Vec3();
	sum.x = x - rhs.x;
	sum.y = y - rhs.y;
	sum.z = z - rhs.z;
	return sum;
}

Vec3 Vec3::Lerp(Vec3 &a, Vec3 &b, float t)
{
	return Vec3(
		a.x + t * (b.x - a.x),
		a.y + t * (b.y - a.y),
		a.z + t * (b.z - a.z)
		);
}

Vec3 & Vec3::operator+=(Vec3 rhs)
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	return *this;
}



Vec3 & Vec3::operator*=(float rhs)
{
	x *= rhs;
	y *= rhs;
	z *= rhs;
	return *this;
}


Vec3 Vec3::reflect(Vec3 vector, Vec3 normal)
{
	// I is the original array
	// N is the normal of the incident plane
	// R = I - (2 * N * ( DotProduct[ I,N] ))
	Vec3 reflectedVector;
	// inline the dotProduct here instead of calling method
	float dotProduct = vector.dot(normal);
	reflectedVector.x = vector.x - (2.0f * normal.x) * dotProduct;
	reflectedVector.y = vector.y - (2.0f * normal.y) * dotProduct;
	reflectedVector.z = vector.z - (2.0f * normal.z) * dotProduct;

	return reflectedVector;
}

float Vec3::dot(const Vec3& v) {
	return x*v.x + y*v.y + z*v.z;
}

Vec3 Vec3::cross(Vec3 & v)
{
	return Vec3((y*v.z) - (z*v.y),
		-((x*v.z) - (z*v.x)),
		((x*v.y) - (y*v.x)));
}

float Vec3::Magnitude()
{
	float mag = sqrt(x*x + y*y + z*z);
	return mag;
}

Vec3 Vec3::Normalized()
{
	Vec3 n = Vec3(x, y, z);
	float m = Magnitude();
	n.x = n.x / m;
	n.y = n.y / m;
	n.z = n.z / m;
	return n;
}

string Vec3::ToString()
{
	string s("");
	std::setprecision(2);
	s = to_string(x) + ", " + to_string(y) + ", " + to_string(z);
	return s;
}

Vec4::Vec4()
{
	x = 0;
	y = 0;
	z = 0;
	w = 0;
}

Vec4::Vec4(GLfloat a, GLfloat b, GLfloat c, GLfloat d)
{
	x = a;
	y = b;
	z = c;
	w = d;
}

Vec4& Vec4::operator=(Vec4& rhs)
{
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;
	w = rhs.w;
	return *this;
}

void Vertex::operator=(Vertex & rhs)
{
	position = rhs.position;
	uv = rhs.uv;
	w = rhs.w;
}

Vertex Vertex::Lerp(Vertex & a, Vertex & b, float t)
{
	float lerpW = a.w + t * (b.w - a.w);
	return Vertex(Vec3::Lerp(a.position, b.position, t), Vec2::Lerp(a.uv, b.uv, t), lerpW);
}

float Vertex::GetPos(int index)
{
	switch (index) {
	case 0:
		return position.x;
	case 1:
		return position.y;
	case 2:
		return position.z;
	}
}
