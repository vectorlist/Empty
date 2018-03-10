#pragma once

//========================================================================================
// Matrix4x4 : Row Major order
//		 col
// row | m00 m01 m02..
//	   | m04 m05 ...
//	   | m08 m09 ..
//
// data alignment : m00 m01 m02 m03 m04.... m015 (must be transpose for opengl) 
//
// Note : rotation and perspective parameters are all degree angle
//
// vectolist@gmail.com
// 
//========================================================================================

#include <math/math.h>
#include <math/vec3f.h>
#include <math/vec4f.h>
#include <assert.h>
#include <iostream>
#include <iomanip>
#include <memory>


class Matrix4x4
{
public:
	enum Axis
	{
		AxisX = 0,
		AxisY,
		AxisZ
	};
	Matrix4x4();
	Matrix4x4(const Matrix4x4 &other);
	Matrix4x4(float other[16]);
	Matrix4x4(const double other[16]);
	Matrix4x4(float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33);
	~Matrix4x4() {};

	union{
		struct {
			float m[4][4];
		};
		struct{
			float mData[16];
		};
	};
	
	Matrix4x4		operator*(const Matrix4x4 &other) const;
	Matrix4x4&		operator*=(const Matrix4x4 &other);
	vec3f			operator*(const vec3f &v) const;
	vec4f			operator*(const vec4f &v) const;
	Matrix4x4		operator*(float f) const;
	Matrix4x4&		operator*=(float f);

	const float*	operator[](int i) const;
	float*			operator[](int i);

	void			SetToIdentity();

	vec3f			Normal(const vec3f &n) const;
	Matrix4x4&		Transpose();
	Matrix4x4		Transposed() const;
	Matrix4x4		Inverted() const;

	void			Scale(const vec3f &s);
	void			Scale(float s);
	void			Translate(const vec3f &t);
	void			Translate(float x, float y, float z);
	void			Rotate(Matrix4x4::Axis axis, float angle);

	vec3f			GetT() const;
	vec3f			GetScale() const;

	float*			data();
	const float*	constData() const;

	static Matrix4x4	RotatedX(float angle);
	static Matrix4x4	RotatedY(float angle);
	static Matrix4x4	RotatedZ(float angle);
	static Matrix4x4	RotationAxis(const vec3f& vector, float angle);
	static void			Normal(vec3f &n, const Matrix4x4 &M);
	static Matrix4x4	VulkanClip();

	//static grphics utils
	static Matrix4x4 PerspectiveRH(float fovy, float aspect, float znear, float zfar);
	static Matrix4x4 PerspectiveLH(float fovy, float aspect, float znear, float zfar);
	static Matrix4x4 PerspectiveVKRH(float fovy, float aspect, float znear, float zfar);
	static Matrix4x4 PerspectiveDXLH(float fovy, float aspect, float znear, float zfar);

	static Matrix4x4 LookAtRH(const vec3f& eye, const vec3f& center, const vec3f& upvector);
	static Matrix4x4 LookAtLH(const vec3f& eye, const vec3f& center, const vec3f& upvector);
	static Matrix4x4 LookAtVK(const vec3f& eye, const vec3f& center, const vec3f& up);
	static Matrix4x4 LookAtDXLH(const vec3f& eye, const vec3f& center, const vec3f& upvector);

	static Matrix4x4 OrthoRH(float left, float right, float bottom, float top);
	static Matrix4x4 OrthoRH(float left, float right, float bottom, float top, float znear, float zfar);

	static Matrix4x4 OrthoLH(float width, float height, float znear, float zfar);
	static Matrix4x4 OrthoDXLH(float left, float right, float bottom, float top, float znear, float zfar);

	static Matrix4x4 RotationYawPitchRoll(float yaw, float pitch, float roll);
	static Matrix4x4 NDCToScreen(int width, int hegith);
	static Matrix4x4 NDCToScreenZeroToOne(int width, int hegith);

	friend std::ostream& operator<<(std::ostream &o, const Matrix4x4 &mat);
};

inline Matrix4x4::Matrix4x4()
{
	SetToIdentity();
}

inline Matrix4x4::Matrix4x4(const Matrix4x4 &other)
{
	std::memcpy(m, other.m, sizeof(m));
}

inline Matrix4x4::Matrix4x4(float other[16])
{
	std::memcpy(m, other, sizeof(m));
}

inline Matrix4x4::Matrix4x4(const double other[16])
{
	m[0][0] = static_cast<float>(other[0]);
	m[0][1] = static_cast<float>(other[1]);
	m[0][2] = static_cast<float>(other[2]);
	m[0][3] = static_cast<float>(other[3]);

	m[1][0] = static_cast<float>(other[4]);
	m[1][1] = static_cast<float>(other[5]);
	m[1][2] = static_cast<float>(other[6]);
	m[1][3] = static_cast<float>(other[7]);

	m[2][0] = static_cast<float>(other[8]);
	m[2][1] = static_cast<float>(other[9]);
	m[2][2] = static_cast<float>(other[10]);
	m[2][3] = static_cast<float>(other[11]);

	m[3][0] = static_cast<float>(other[12]);
	m[3][1] = static_cast<float>(other[13]);
	m[3][2] = static_cast<float>(other[14]);
	m[3][3] = static_cast<float>(other[15]);
}

inline Matrix4x4::Matrix4x4(float m00, float m01, float m02, float m03,
	float m10, float m11, float m12, float m13,
	float m20, float m21, float m22, float m23,
	float m30, float m31, float m32, float m33)
{
	m[0][0] = m00; m[0][1] = m01; m[0][2] = m02; m[0][3] = m03;
	m[1][0] = m10; m[1][1] = m11; m[1][2] = m12; m[1][3] = m13;
	m[2][0] = m20; m[2][1] = m21; m[2][2] = m22; m[2][3] = m23;
	m[3][0] = m30; m[3][1] = m31; m[3][2] = m32; m[3][3] = m33;
}

inline Matrix4x4 Matrix4x4::operator*(const Matrix4x4 &other) const
{
	Matrix4x4 M;

	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			M.m[i][j] =
				m[i][0] * other.m[0][j] +
				m[i][1] * other.m[1][j] +
				m[i][2] * other.m[2][j] +
				m[i][3] * other.m[3][j];
		}
	}
	return M;
}

inline Matrix4x4& Matrix4x4::operator*=(const Matrix4x4 &other)
{
	*this = operator*(other);
	return *this;
}

inline vec3f Matrix4x4::operator*(const vec3f &v) const
{
	float x, y, z;
	x = v.x * m[0][0] + v.y * m[0][1] + v.z * m[0][2] + m[0][3];
	y = v.x * m[1][0] + v.y * m[1][1] + v.z * m[1][2] + m[1][3];
	z = v.x * m[2][0] + v.y * m[2][1] + v.z * m[2][2] + m[2][3];

	return vec3f(x, y, z);
}

inline vec4f Matrix4x4::operator*(const vec4f &v) const
{
	float x, y, z, w;
	x = v.x * m[0][0] + v.y * m[0][1] + v.z * m[0][2] + v.w * m[0][3];
	y = v.x * m[1][0] + v.y * m[1][1] + v.z * m[1][2] + v.w * m[1][3];
	z = v.x * m[2][0] + v.y * m[2][1] + v.z * m[2][2] + v.w * m[2][3];
	w = v.x * m[3][0] + v.y * m[3][1] + v.z * m[3][2] + v.w * m[3][3];
	return vec4f(x, y, z, w);
}

inline Matrix4x4 Matrix4x4::operator*(float f) const
{
	Matrix4x4 res;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			res[i][j] = f * m[i][j];
		}
	}
	return res;
}

inline Matrix4x4& Matrix4x4::operator*=(float f)
{
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			m[i][j] *= f;
		}
	}
	return *this;
}

inline const float* Matrix4x4::operator[](int i) const {
	assert(i >= 0 && i < 4);
	return m[i];
}

inline float* Matrix4x4::operator[](int i) {
	assert(i >= 0 && i < 4);
	return m[i];
}

inline void Matrix4x4::SetToIdentity()
{
	memset(m, 0, sizeof(m));
	m[0][0] = 1.0f;
	m[1][1] = 1.0f;
	m[2][2] = 1.0f;
	m[3][3] = 1.0f;
}

inline Matrix4x4& Matrix4x4::Transpose()
{
	for (int row = 0; row < 4; ++row)
		for (int col = row + 1; col < 4; ++col)
		{
			float tmp = m[row][col];
			m[row][col] = m[col][row];
			m[col][row] = tmp;
		}

	return *this;
}

inline Matrix4x4 Matrix4x4::Transposed() const
{
	Matrix4x4 M;
	for (int Row = 0; Row < 4; ++Row)
		for (int Col = 0; Col < 4; ++Col)
			M.m[Row][Col] = m[Col][Row];

	return M;
}

inline Matrix4x4 Matrix4x4::RotatedX(float angle)
{
	float theta = angle * RADIANS;
	float sinx = sin(theta);
	float cosx = cos(theta);

	Matrix4x4 M = Matrix4x4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, cosx, -sinx, 0.0f,
		0.0f, sinx, cosx, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	return M;
}

inline Matrix4x4 Matrix4x4::RotatedY(float angle)
{
	float theta = angle * RADIANS;
	float siny = sin(theta);
	float cosy = cos(theta);

	Matrix4x4 M = Matrix4x4(
		 cosy, 0.0f, siny, 0.0f,
	 	 0.0f, 1.0f, 0.0f, 0.0f,
		-siny, 0.0f, cosy, 0.0f,
		 0.0f, 0.0f, 0.0f, 1.0f);

	return M;
}

inline Matrix4x4 Matrix4x4::RotatedZ(float angle)
{
	float theta = angle * RADIANS;
	float sinz = sin(theta);
	float cosz = cos(theta);

	Matrix4x4 M = Matrix4x4(
		cosz, -sinz,  0.0f,  0.0f,
		sinz,  cosz,  0.0f,  0.0f,
		0.0f,  0.0f,  1.0f,  0.0f,
		0.0f,  0.0f,  0.0f,  1.0f);

	return M;
}

inline void Matrix4x4::Scale(const vec3f &s)
{
	Matrix4x4 M;
	M.m[0][0] *= s.x;
	M.m[1][1] *= s.y;
	M.m[2][2] *= s.z;

	*this = *this * M;
}

inline void Matrix4x4::Scale(float s)
{
	Matrix4x4 M;
	M.m[0][0] *= s;
	M.m[1][1] *= s;
	M.m[2][2] *= s;

	*this = *this * M;
}

inline void Matrix4x4::Translate(const vec3f &t)
{
	Matrix4x4 M;

	M.m[0][3] = t.x;
	M.m[1][3] = t.y;
	M.m[2][3] = t.z;

	*this = *this * M;
}
inline void	Matrix4x4::Translate(float x, float y, float z)
{
	Matrix4x4 T;
	T[0][3] = x;
	T[1][3] = y;
	T[2][3] = z;

	*this = *this * T;
}

inline void Matrix4x4::Rotate(Matrix4x4::Axis axis, float angle)
{
	float T = angle;
	Matrix4x4 RotM;
	switch (axis)
	{
	case AxisX:
		RotM[0][0] = 1.f;
		RotM[1][1] = cos(T);
		RotM[1][2] = -sin(T);
		RotM[2][1] = sin(T);
		RotM[2][2] = cos(T);
		RotM[3][3] = 1.f;
		break;
	case AxisY:
		RotM[0][0] = cos(T);
		RotM[0][2] = sin(T);
		RotM[1][1] = 1.f;
		RotM[2][0] = -sin(T);
		RotM[2][2] = cos(T);
		RotM[3][3] = 1.f;
		break;
	case AxisZ:
		RotM[0][0] = cos(T);
		RotM[0][1] = -sin(T);
		RotM[1][0] = sin(T);
		RotM[1][1] = cos(T);
		RotM[2][2] = 1.f;
		break;
	}

	*this = *this * RotM;
}

inline vec3f Matrix4x4::GetT() const
{
	vec3f T;
	T.x = m[0][3];
	T.y = m[1][3];
	T.z = m[2][3];
	return T;
}

inline vec3f Matrix4x4::GetScale() const
{
	vec3f S;
	S.x = vec3f(m[0][0], m[1][0], m[2][0]).length();
	S.y = vec3f(m[0][1], m[1][1], m[2][1]).length();
	S.z = vec3f(m[0][2], m[1][2], m[2][2]).length();
	return S;
}

inline float* Matrix4x4::data()
{
	return *m;
}

inline const float* Matrix4x4::constData() const
{
	return *m;
}

inline Matrix4x4 Matrix4x4::Inverted() const
{
	Matrix4x4 inv;
	float m00 = m[0][0];
	float m01 = m[0][1];
	float m02 = m[0][2];
	float m03 = m[0][3];
	float m10 = m[1][0];
	float m11 = m[1][1];
	float m12 = m[1][2];
	float m13 = m[1][3];
	float m20 = m[2][0];
	float m21 = m[2][1];
	float m22 = m[2][2];
	float m23 = m[2][3];
	float m30 = m[3][0];
	float m31 = m[3][1];
	float m32 = m[3][2];
	float m33 = m[3][3];

	float R23C23 = m22 * m33 - m23 * m32;
	float R23C13 = m21 * m33 - m23 * m31;
	float R23C12 = m21 * m32 - m22 * m31;
	float R23C03 = m20 * m33 - m23 * m30;
	float R23C02 = m20 * m32 - m22 * m30;
	float R23C01 = m20 * m31 - m21 * m30;

	inv[0][0] = +(m11 * R23C23 - m12 * R23C13 + m13 * R23C12);
	inv[1][0] = -(m10 * R23C23 - m12 * R23C03 + m13 * R23C02);
	inv[2][0] = +(m10 * R23C13 - m11 * R23C03 + m13 * R23C01);
	inv[3][0] = -(m10 * R23C12 - m11 * R23C02 + m12 * R23C01);

	float det = m00 * inv[0][0] + m01 * inv[1][0] +
		m02 * inv[2][0] + m03 * inv[3][0];
	if (fabs(det) < MATRIX_EPSILON) {

	}
	float invDet = 1.0f / det;

	inv[0][1] = -(m01 * R23C23 - m02 * R23C13 + m03 * R23C12);
	inv[1][1] = +(m00 * R23C23 - m02 * R23C03 + m03 * R23C02);
	inv[2][1] = -(m00 * R23C13 - m01 * R23C03 + m03 * R23C01);
	inv[3][1] = +(m00 * R23C12 - m01 * R23C02 + m02 * R23C01);

	float R13C23 = m12 * m33 - m13 * m32;
	float R13C13 = m11 * m33 - m13 * m31;
	float R13C12 = m11 * m32 - m12 * m31;
	float R13C03 = m10 * m33 - m13 * m30;
	float R13C02 = m10 * m32 - m12 * m30;
	float R13C01 = m10 * m31 - m11 * m30;

	inv[0][2] = +(m01 * R13C23 - m02 * R13C13 + m03 * R13C12);
	inv[1][2] = -(m00 * R13C23 - m02 * R13C03 + m03 * R13C02);
	inv[2][2] = +(m00 * R13C13 - m01 * R13C03 + m03 * R13C01);
	inv[3][2] = -(m00 * R13C12 - m01 * R13C02 + m02 * R13C01);

	float R12C23 = m12 * m23 - m13 * m22;
	float R12C13 = m11 * m23 - m13 * m21;
	float R12C12 = m11 * m22 - m12 * m21;
	float R12C03 = m10 * m23 - m13 * m20;
	float R12C02 = m10 * m22 - m12 * m20;
	float R12C01 = m10 * m21 - m11 * m20;

	inv[0][3] = -(m01 * R12C23 - m02 * R12C13 + m03 * R12C12);
	inv[1][3] = +(m00 * R12C23 - m02 * R12C03 + m03 * R12C02);
	inv[2][3] = -(m00 * R12C13 - m01 * R12C03 + m03 * R12C01);
	inv[3][3] = +(m00 * R12C12 - m01 * R12C02 + m02 * R12C01);

	inv *= invDet;
	return inv;
}

inline Matrix4x4 Matrix4x4::VulkanClip()
{
	Matrix4x4 Clip = {
		1.0f,  0.0f,  0.0f,  0.0f,
		0.0f, -1.0f,  0.0f,  0.0f,
		0.0f,  0.0f,  0.5f,  0.5f,
		0.0f,  0.0f,  0.0f,  1.0f
	};
	return Clip;
}

inline vec3f Matrix4x4::Normal(const vec3f &n) const
{
	float x, y, z;
	x = n.x * m[0][0] + n.y * m[0][1] + n.z * m[0][2];
	y = n.x * m[1][0] + n.y * m[1][1] + n.z * m[1][2];
	z = n.x * m[2][0] + n.y * m[2][1] + n.z * m[2][2];

	return vec3f(x, y, z).normalized();
}

inline Matrix4x4 Matrix4x4::RotationAxis(const vec3f& v, float angle)
{
	float theta = angle * RADIANS;
	float cosx = cos(v.x * theta);
	float cosy = cos(v.y * theta);
	float cosz = cos(v.z * theta);
	float sinx = sin(v.x * theta);
	float siny = sin(v.y * theta);
	float sinz = sin(v.z * theta);

	Matrix4x4 mat = {
		cosy*cosz + siny*sinx *sinz,  cosy*-sinz + siny*sinx*cosz,  siny*cosx,  0.0f,
		cosx*sinz,                    cosx*cosz,                    -sinx,      0.0f,
		-siny*cosz + cosy*sinx*sinz,  -siny*-sinz + cosy*sinx*cosz, cosy*cosx,  0.0f,
		0.0f,                         0.0f,                         0.0f,       1.0f
	};
	return mat;
}

inline void Matrix4x4::Normal(vec3f &n, const Matrix4x4 &m)
{
	float x, y, z;
	x = n.x * m[0][0] + n.y * m[0][1] + n.z * m[0][2];
	y = n.x * m[1][0] + n.y * m[1][1] + n.z * m[1][2];
	z = n.x * m[2][0] + n.y * m[2][1] + n.z * m[2][2];
	n.x = x;
	n.y = y;
	n.z = z;
}



inline Matrix4x4 Matrix4x4::PerspectiveRH(float fovY, float aspect, float znear, float zfar)
{
	float const tanFov = tan(RADIANS * fovY * 0.5f);

	Matrix4x4 proj;
	proj[0][0] = 1.f / (aspect* tanFov);
	proj[1][1] = 1.f / tanFov;
	proj[3][2] = -1.f;

	proj[2][2] = -(zfar + znear) / (zfar - znear);
	proj[2][3] = -(2.f * zfar * znear) / (zfar - znear);

	return proj;
}

inline Matrix4x4 Matrix4x4::PerspectiveLH(float fovY, float aspect, float znear, float zfar)
{
	float const tanHalfFov = tan(RADIANS * fovY * 0.5f);

	Matrix4x4 M;
	M[0][0] = 1.f / (aspect* tanHalfFov);
	M[1][1] = 1.f / tanHalfFov;
	M[3][2] = 1.f;

	M[2][2] = (zfar + znear) / (zfar - znear);
	M[2][3] = -(2.f * zfar * znear) / (zfar - znear);

	return M;
}

inline Matrix4x4 Matrix4x4::PerspectiveVKRH(float fovy, float aspect, float znear, float zfar)
{
	Matrix4x4 proj = PerspectiveRH(fovy, aspect, znear, zfar);
	Matrix4x4 clip = {
		1.0f,	0.0f,	0.0f,	0.0f,
		0.0f,  -1.0f,	0.0f,	0.0f,
		0.0f,	0.0f,	0.5f,	0.5f,
		0.0f,	0.0f,	0.0f,	1.0f
	};
	return clip * proj;
}

inline Matrix4x4 Matrix4x4::PerspectiveDXLH(float fovy, float aspect, float znear, float zfar)
{
	float const tanFov = tan(RADIANS * fovy * 0.5f);

	float A = 1.0f / (aspect * tanFov);
	float B = 1.0f / tanFov;
	float C = zfar / (zfar - znear);
	float D = 1.0f;
	float E = -znear * (zfar / (zfar - znear));
	Matrix4x4 proj = {
		A,      0.0f,   0.0f,   0.0f,
		0.0f,   B,      0.0f,   0.0f,
		0.0f,   0.0f,   C,      E,
		0.0f,   0.0f,   D,      0.0f
	};
	return proj;
}

inline Matrix4x4 Matrix4x4::LookAtRH(const vec3f &eye, const vec3f& center, const vec3f& upvector)
{
	vec3f const forward = (center - eye).normalized();
	vec3f const right = vec3f::cross(forward, upvector).normalized();
	vec3f const up = vec3f::cross(right, forward);

	float x = -vec3f::dot(right, eye);
	float y = -vec3f::dot(up, eye);
	float z = vec3f::dot(forward, eye);

	Matrix4x4 view = {
			right.x,          right.y,		     right.z,			x,
			    up.x,				up.y,				up.z,			y,
		-forward.x,       -forward.y,         -forward.z,		    z,
			    0.0f,             0.0f,               0.0f,         1.0f };
	return view;
}

inline Matrix4x4 Matrix4x4::LookAtLH(const vec3f &eye, const vec3f& center, const vec3f& upvector)
{
	vec3f const forward = (center - eye).normalized();
	vec3f const right = vec3f::cross(upvector, forward).normalized();
	vec3f const up = vec3f::cross(forward, right);

	float x = -vec3f::dot(right, eye);
	float y = -vec3f::dot(up, eye);
	float z = -vec3f::dot(forward, eye);

	Matrix4x4 view = {
			right.x,     right.y,	 right.z,		x,
			    up.x,        up.y,        up.z,		y,
		-forward.x,  -forward.y,  -forward.z,		z,
			    0.0f,        0.0f,        0.0f,     1.0f };
	return view;
}

//for vulkan
inline Matrix4x4 Matrix4x4::LookAtVK(const vec3f &eye, const vec3f &center, const vec3f &up)
{
	vec3f forward = (eye - center).normalized();
	vec3f right = vec3f::cross(up, forward).normalized();
	vec3f upvector = vec3f::cross(forward, right).normalized();

	float x = -vec3f::dot(eye, right);
	float y = -vec3f::dot(eye, upvector);
	float z = -vec3f::dot(eye, forward);

	Matrix4x4 view = {
		right.x,upvector.x,forward.x,0.0f,
		right.y,upvector.y,forward.y,0.0f,
		right.z,upvector.z,forward.z,0.0f,
		x,y,z,1.0f
	};
	return view;
}

inline Matrix4x4 Matrix4x4::LookAtDXLH(const vec3f &eye, const vec3f& center, const vec3f& upvector)
{
	vec3f const forward = (center - eye).normalized();
	vec3f const right = vec3f::cross(upvector, forward).normalized();
	vec3f const up = vec3f::cross(forward, right);

	float x = -vec3f::dot(right, eye);
	float y = -vec3f::dot(up, eye);
	float z = -vec3f::dot(forward, eye);

	Matrix4x4 view = {
		right.x,     right.y,	  right.z,		x,
		up.x,        up.y,        up.z,			y,
		forward.x,   forward.y,   forward.z,	z,
		0.0f,        0.0f,        0.0f,			1.0f };
	return view;
}


inline Matrix4x4 Matrix4x4::OrthoRH(float left, float right, float bottom, float top)
{
	Matrix4x4 proj;
	proj[0][0] = 2.f / (right - left);
	proj[1][1] = 2.f / (top - bottom);
	proj[2][2] = -1.f;
	proj[0][3] = -(right + left) / (right - left);
	proj[1][3] = -(top + bottom) / (top - bottom);
	return proj;
}

inline Matrix4x4 Matrix4x4::OrthoRH(
	float left, float right, float bottom, float top, float znear, float zfar)
{
	Matrix4x4 res;
	res[0][0] = 2.f / (right - left);
	res[1][1] = 2.f / (top - bottom);
	res[2][2] = -(2.f) / (zfar - znear);
	res[0][3] = -(right + left) / (right - left);
	res[1][3] = -(top + bottom) / (top - bottom);
	res[2][3] = -(zfar + znear) / (zfar - znear);
	return res;
}

inline Matrix4x4 Matrix4x4::OrthoLH(float width, float height, float znear, float zfar)
{
	//float A = 2.0f / width;
	//float B = 2.0f / height;
	//float C = 1.0f / (zfar - znear);
	//float D = -znear / (zfar - znear);

	//Matrix4x4 m =
	//{
	//	A   , 0.0f, 0.0f, 0.0f,
	//	0.0f, B   , 0.0f, 0.0f,
	//	0.0f, 0.0f, C   , D   ,
	//	0.0f, 0.0f,	0.0f, 1.0f
	//};

	float A = 2.0f / width;
	float B = 2.0f / height;
	float C = 2.0f / (zfar - znear);

	float D = -(zfar + znear) / (zfar - znear);
	float E = -width / width;
	float F = -height / height;

	Matrix4x4 m =
	{
		A   , 0.0f, 0.0f, E   ,
		0.0f, B   , 0.0f, F   ,
		0.0f, 0.0f, C   , D   ,
		0.0f, 0.0f,	0.0f, 1.0f
	};

	return m;
}

inline Matrix4x4 Matrix4x4::OrthoDXLH(
	float left, float right, float bottom, float top, float znear, float zfar)
{
	float A = 2.f / (right - left);
	float B = 2.f / (top - bottom);
	float C = 1.f / (zfar - znear);

	float D = znear / (znear - zfar);
	float E = (left + right) / (left - right);
	float F = (top + bottom) / (bottom - top);

	Matrix4x4 res = {
		A   , 0.0f, 0.0f, E  ,
		0.0f, B   , 0.0f, F  ,
		0.0f, 0.0f, C   , D  ,
		0.0f, 0.0f, 0.0f, 1.0f
	};
	return res;
}

inline Matrix4x4 Matrix4x4::RotationYawPitchRoll(float yaw, float pitch, float roll)
{
	//https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles

	//          yaw               pitch                roll
	// |  cosy   0  siny     1      0      0  |     | cosz  -sinz    0 |
	// |     0   1     0  *  0   cosx  -sinx  |  *  | sinz   cosz    0 |
	// | -siny   0  cosy     0   sinx   cosx  |     |    0      0    1 |
	//
	//            (yaw * pitch)                       roll
	// |  cosy   siny*sinx   siny*cosx  |     | cosz  -sinz    0 |
	// |   0     cosx       -sinx	    |  *  | sinz   cosz    0 |
	// | -siny   cosy*sinx   cosy*cosx  |     |    0      0    1 |
	//
	//							Yaw * Pitch * Roll
	// | cosy*cosz + siny*sinx *sinz   cosy*-sinz+ siny*sinx*cosz   siny*cosx |
	// | cosx*sinz                     cosx*cosz                    -sinx	  |
	// | -siny*cosz+cosy*sinx*sinz     -siny*-sinz+cosy*sinx*cosz   cosy*cosx |

	yaw	  *= RADIANS;
	pitch *= RADIANS;
	roll  *= RADIANS;

	float cosx = cos(pitch);
	float cosy = cos(yaw);
	float cosz = cos(roll);
	float sinx = sin(pitch);
	float siny = sin(yaw);
	float sinz = sin(roll);

	Matrix4x4 mat = {
			cosy*cosz + siny*sinx *sinz,  cosy*-sinz + siny*sinx*cosz,  siny*cosx,  0.0f,
			cosx*sinz,                    cosx*cosz,                    -sinx,      0.0f,
			-siny*cosz + cosy*sinx*sinz,  -siny*-sinz + cosy*sinx*cosz, cosy*cosx,  0.0f,
			0.0f,                         0.0f,                         0.0f,       1.0f
	};

	return mat;
}

inline Matrix4x4 Matrix4x4::NDCToScreen(int width, int hegith)
{
	float sx = 2.0f / (float)width;
	float sy = 2.0f / (float)hegith;
	Matrix4x4 screen = {
		sx,   0.0f, 0.0f, 0.0f,
		0.0f, -sy,  0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
	return screen;
}

inline Matrix4x4 Matrix4x4::NDCToScreenZeroToOne(int width, int hegith)
{
	float sx = 2.0f / (float)width;
	float sy = 2.0f / (float)hegith;
	Matrix4x4 screen = {
		sx,   0.0f, 0.0f, -1.0f,
		0.0f, -sy,  0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
	return screen;
}


inline std::ostream& operator<<(std::ostream &o, const Matrix4x4 &mat)
{
#define FM std::setw(12)
	//row major order
	o << "Matrix4x4[" << "\n" <<
		FM << mat[0][0] << FM << mat[0][1] << FM << mat[0][2] << FM << mat[0][3] << "\n" <<
		FM << mat[1][0] << FM << mat[1][1] << FM << mat[1][2] << FM << mat[1][3] << "\n" <<
		FM << mat[2][0] << FM << mat[2][1] << FM << mat[2][2] << FM << mat[2][3] << "\n" <<
		FM << mat[3][0] << FM << mat[3][1] << FM << mat[3][2] << FM << mat[3][3] << " ]" << "\n";
#undef FM
	return o;
}

