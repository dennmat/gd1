#ifndef _MATH_3D_H
#define _MATH_3D_H

#include "sgtypes.h"

#include <math.h>

#include <stdio.h>

class Vector3f {
public:
	float x, y, z, w;

	Vector3f();
	Vector3f(float _x, float _y, float _z, float _w = 0.0f);

	Vector3f cross(const Vector3f& v) const;
	Vector3f & normalize();

	Vector3f & operator+=(const Vector3f& v);
	Vector3f operator+(const Vector3f& v) const;
	Vector3f & operator-=(const Vector3f& v);
	Vector3f operator-(const Vector3f& v) const;
};

class Matrix4f {
public:
	float data[16];

	Matrix4f();
	Matrix4f(float _data[16]);
	
	Matrix4f operator*(const Matrix4f &rhs) const;
	Vector3f operator*(const Vector3f &rhs) const;

	void asIdentity();
	void asScale(float x, float y, float z);
	void asTranslate(float x, float y, float z);

	void asColumnMajor(float * _in) const;
};

class Quaternion {
public:
	float w, x, y, z;
	
	Quaternion();
	Quaternion(float _x, float _y, float _z, float _w);
	float magnitude();
	void normalize();

	Quaternion getConjugate() const;

	void rotate(float a, float _x, float _y, float _z);

	Quaternion & operator*=(const Quaternion &rhs);
	Quaternion operator*(const Quaternion &other) const;

	Vector3f operator*(const Vector3f &vec) const;

	Matrix4f toMatrix();
};

#endif