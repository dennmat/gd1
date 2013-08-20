#include "Math3d.h"

Vector3f::Vector3f() {
	x = 0.0; y = 0.0; z = 0.0; w = 1.0;
}

Vector3f::Vector3f(float _x, float _y, float _z, float _w) {
	x = _x; y = _y; z = _z; w = _w;
}

Vector3f Vector3f::cross(const Vector3f& v) const {
	const float _x = y * v.z - z * v.y;
	const float _y = z * v.x - x * v.z;
	const float _z = x * v.y - y * v.x;

	return Vector3f(_x, _y, _z);
}

Vector3f & Vector3f::operator+=(const Vector3f& v) {
	Vector3f result;

	result = *this + v;
	x = result.x;
	y = result.y;
	z = result.z;
	w = result.w;
	return *this;
}

Vector3f Vector3f::operator+(const Vector3f& v) const {
	Vector3f result;

	result.x = x + v.x;
	result.y = y + v.y;
	result.z = z + v.z;
	result.w = w;

	return result;
}

Vector3f & Vector3f::operator-=(const Vector3f& v) {
	Vector3f result;

	result = *this - v;
	x = result.x;
	y = result.y;
	z = result.z;
	w = result.w;
	return *this;
}

Vector3f Vector3f::operator-(const Vector3f& v) const {
	Vector3f result;

	result.x = x - v.x;
	result.y = y - v.y;
	result.z = z - v.z;
	result.w = w;

	return result;
}

Vector3f& Vector3f::normalize() {
	const float length = sqrtf(x * x + y * y + z * z);

	x /= length;
	y /= length;
	z /= length;

	return *this;
}

Matrix4f::Matrix4f() {
	for (int i = 0; i < 16; i++) {
		data[i] = 0.0f;
	}
}

Matrix4f::Matrix4f(float _data[16]) {
	for (int i = 0; i < 16; i++) {
		data[i] = _data[i];
	}
}

void Matrix4f::asIdentity() {
	for (int i = 0; i < 16; i++) {
		data[i] = 0.0f;
	}
	data[0] = 1.0f;
	data[5] = 1.0f;
	data[10] = 1.0f;
	data[15] = 1.0f;
}

void Matrix4f::asScale(float x, float y, float z) {
	asIdentity();
	data[0] = x;
	data[5] = y;
	data[10] = z;
}

void Matrix4f::asTranslate(float x, float y, float z) {
	asIdentity();
	data[3] = x;
	data[7] = y;
	data[11] = z;
}

Vector3f Matrix4f::operator*(const Vector3f &rhs) const {
	Vector3f result;

	result.x = (data[0]*rhs.x + data[1]*rhs.y + data[2]*rhs.z + data[3]*rhs.w);
	result.y = (data[4]*rhs.x + data[5]*rhs.y + data[6]*rhs.z + data[7]*rhs.w);
	result.z = (data[8]*rhs.x + data[9]*rhs.y + data[10]*rhs.z + data[11]*rhs.w);
	result.w = (data[12]*rhs.x + data[13]*rhs.y + data[14]*rhs.z + data[15]*rhs.w);

	return result;
}

void Matrix4f::asColumnMajor(float * _in) const {
	_in[0] = data[0];
	_in[1] = data[4];
	_in[2] = data[8];
	_in[3] = data[12];
	_in[4] = data[1];
	_in[5] = data[5];
	_in[6] = data[9];
	_in[7] = data[13];
	_in[8] = data[2];
	_in[9] = data[6];
	_in[10] = data[10];
	_in[11] = data[14];
	_in[12] = data[3];
	_in[13] = data[7];
	_in[14] = data[11];
	_in[15] = data[15];
}

Matrix4f Matrix4f::operator*(const Matrix4f &rhs) const {
	const float * a = data;
	const float * b = rhs.data;

	Matrix4f result;

	result.data[0] = (a[0]*b[0] + a[1]*b[4] + a[2]*b[8] + a[3]*b[12]);
	result.data[1] = (a[0]*b[1] + a[1]*b[5] + a[2]*b[9] + a[3]*b[13]);
	result.data[2] = (a[0]*b[2] + a[1]*b[6] + a[2]*b[10] + a[3]*b[14]);
	result.data[3] = (a[0]*b[3] + a[1]*b[7] + a[2]*b[11] + a[3]*b[15]);

	result.data[4] = (a[4]*b[0] + a[5]*b[4] + a[6]*b[8] + a[7]*b[12]);
	result.data[5] = (a[4]*b[1] + a[5]*b[5] + a[6]*b[9] + a[7]*b[13]);
	result.data[6] = (a[4]*b[2] + a[5]*b[6] + a[6]*b[10] + a[7]*b[14]);
	result.data[7] = (a[4]*b[3] + a[5]*b[7] + a[6]*b[11] + a[7]*b[15]);

	result.data[8] = (a[8]*b[0] + a[9]*b[4] + a[10]*b[8] + a[11]*b[12]);
	result.data[9] = (a[8]*b[1] + a[9]*b[5] + a[10]*b[9] + a[11]*b[13]);
	result.data[10] = (a[8]*b[2] + a[9]*b[6] + a[10]*b[10] + a[11]*b[14]);
	result.data[11] = (a[8]*b[3] + a[9]*b[7] + a[10]*b[11] + a[11]*b[15]);

	result.data[12] = (a[12]*b[0] + a[13]*b[4] + a[14]*b[8] + a[15]*b[12]);
	result.data[13] = (a[12]*b[1] + a[13]*b[5] + a[14]*b[9] + a[15]*b[13]);
	result.data[14] = (a[12]*b[2] + a[13]*b[6] + a[14]*b[10] + a[15]*b[14]);
	result.data[15] = (a[12]*b[3] + a[13]*b[7] + a[14]*b[11] + a[15]*b[15]);

	return result;
}

Quaternion::Quaternion() {
	w = 1;
	x = 0;
	y = 0;
	z = 0;
}

Quaternion::Quaternion(float _x, float _y, float _z, float _w) {
	w = _w;
	x = _x;
	y = _y;
	z = _z;
}

float Quaternion::magnitude() {
	return sqrtf(w*w + x*x + y*y + z*z);
}

Quaternion Quaternion::getConjugate() const {
	return Quaternion(-x, -y, -z, w);
}

void Quaternion::rotate(float a, float _x, float _y, float _z) {
	Quaternion rot;
	float sin_alpha = sinf(a/2.0f);

	rot.x = _x * sin_alpha;
	rot.y = _y * sin_alpha;
	rot.z = _z * sin_alpha;
	rot.w = cos(a/2.0f);

	rot.normalize();

	Quaternion result = rot * *this;
	x = result.x;
	y = result.y;
	z = result.z;
	w = result.w;
}

void Quaternion::normalize() {
	float premag = (w*w + x*x + y*y + z*z);
	if (fabs(premag) > 0.000001 && fabs(premag - 1.0f) > 0.000001) { //Only do it if necessary
		float mag = sqrt(premag);
		w /= mag;
		x /= mag;
		y /= mag;
		z /= mag;
	}
}

Quaternion & Quaternion::operator*=(const Quaternion &rhs) {
	Quaternion result = (*this)*rhs;
	x = result.x;
	y = result.y;
	z = result.z;
	w = result.w;

	return *this;
}

Quaternion Quaternion::operator*(const Quaternion &other) const {
	Quaternion result = *this;
	/*result.w = (w*other.w - x*other.x - y*other.y - z*other.z);
	result.x = (w*other.x + x*other.w + y*other.z - z*other.y);
	result.y = (w*other.y - x*other.z + y*other.w + z*other.x);
	result.z = (w*other.z + x*other.y - y*other.x + z*other.w);*/
	result.x = (w * other.x + x * other.w + y * other.z - z * other.y);
	result.y = (w * other.y + y * other.w + z * other.x - x * other.z);
	result.z = (w * other.z + z * other.w + x * other.y - y * other.x);
	result.w = (w * other.w - x * other.x - y * other.y - z * other.z);
	//result.normalize();
	return result;
}

Vector3f Quaternion::operator*(const Vector3f &vec) const {
	Vector3f vn(vec);
	vn.normalize();

	Quaternion vecQuat, resQuat;
	vecQuat.x = vn.x;
	vecQuat.y = vn.y;
	vecQuat.z = vn.z;
	vecQuat.w = 0.0f;

	resQuat = vecQuat * getConjugate();
	resQuat = *this * resQuat;

	return (Vector3f(resQuat.x, resQuat.y, resQuat.z));
}

Matrix4f Quaternion::toMatrix() {
	normalize();
	float data[16];

	data[0] = 1.0f - 2.0f*(y*y) - 2.0f*(z*z);
	data[1] = (2.0f*(x*y) - 2.0f*(w*z));
	data[2] = (2.0f*(x*z) + 2.0f*(w*y));
	data[3] = 0.0f;
	data[4] = (2.0f*(x*y) + 2.0f*(w*z));
	data[5] = 1.0f - 2.0f*(x*x) - 2.0f*(z*z);
	data[6] = 2.0f*(y*z) + 2.0f*(w*x);
	data[7] = 0.0f;
	data[8] = 2.0f*(x*z) - 2.0f*(w*y);
	data[9] = 2.0f*(y*z) - 2.0f*(w*x);
	data[10] = 1.0f - 2.0f*(x*x) - 2.0f*(y*y);
	data[11] = 0.0f;
	data[12] = 0.0f;
	data[13] = 0.0f;
	data[14] = 0.0f;
	data[15] = 1.0f;

	return Matrix4f(data);
}