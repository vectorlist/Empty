#pragma once


#include <math/matrix4x4.h>

class Quaternion
{
public:
	Quaternion();
	Quaternion(float x, float y, float z, float w);
	Quaternion(const vec3f &v, float w);
	//Quaternion(const vec3f &axis, float angle);
	Quaternion(const Matrix4x4 &m);
	~Quaternion(){};

	Quaternion			operator*(const Quaternion &other);
	vec3f				operator*(const vec3f &p) const;
	Quaternion			operator*(float f) const;
	Quaternion&			operator*=(float f);


	float				Norm() const;
	Quaternion			Conjugate() const;
	Quaternion&			Noramlize();
	Quaternion			Normalized() const;
	Quaternion&			Invert();
	Quaternion			Inverted() const;

	static Quaternion	FromAxisAngle(const vec3f &axis, float angle);
	static Quaternion	RotationYawPitchRoll(float yaw, float pitch, float rool);
	Matrix4x4			GetMatrix() const;

	static Quaternion	EulerToQuat(float x, float y, float z);
	static vec3f		QuatToEuler(const Quaternion &q);
	static void			Interpolate(Quaternion& res, const Quaternion& start, const Quaternion& end, float factor);
	vec3f v;
	float w;
};

inline Quaternion::Quaternion() : v(0.0f), w(1.0f) {}

inline Quaternion::Quaternion(float x, float y, float z,float w) : v(x, y, z), w(w) {}

inline Quaternion::Quaternion(const vec3f &v, float w) : v(v), w(w) {}

//inline Quaternion::Quaternion(const vec3f &axis, float angle)
//{
//	//must be deggree to radians
//	float t2 = angle / 360.f * M_PI * 2.f;
//	float t = t2 / 2.0f;
//	vec3f tmpAxis = axis.normalized();
//
//	float sinT = std::sin(t);
//
//	v = tmpAxis * sinT;
//	w = std::cos(t);
//}

inline Quaternion::Quaternion(const Matrix4x4 &m)
{
	float trace = m[0][0] + m[1][1] + m[2][2];
	if (trace > 0.0f)
	{
		float s = sqrtf(trace + 1.0f);
		w = s / 2.0f;
		s = 0.5f / s;
		v.x = (m[2][1] - m[1][2]) * s;
		v.y = (m[0][2] - m[2][0]) * s;
		v.z = (m[1][0] - m[0][1]) * s;
	}
	else
	{
		const int nxt[3] = { 1, 2, 0 };
		float q[3];
		int i = 0;
		if (m[1][1] > m[0][0]) i = 1;
		if (m[2][2] > m[i][i]) i = 2;
		int j = nxt[i];
		int k = nxt[j];
		float s = sqrtf((m[i][i] - (m[j][j] + m[k][k])) + 1.0f);
		q[i] = s * 0.5f;
		if (s != 0.f) s = 0.5f / s;
		w = (m[k][j] - m[j][k]) * s;
		q[j] = (m[j][i] + m[i][j]) * s;
		q[k] = (m[k][i] + m[i][k]) * s;
		v.x = q[0];
		v.y = q[1];
		v.z = q[2];
	}
}

inline Quaternion Quaternion::operator*(const Quaternion &other)
{
	//http://math.ucr.edu/~huerta/introquaternions.pdf
	float scalar = (w * other.w) - vec3f::dot(v, other.v);
	vec3f vv =
		w *			other.v +
		other.w			* v +
		vec3f::cross(v, other.v);	

	return Quaternion(vv, scalar);
}

inline vec3f Quaternion::operator*(const vec3f &p) const
{
	vec3f uv = vec3f::cross(v, p);
	vec3f uuv = vec3f::cross(v, uv);
	uv *= (2.f * w);
	uuv *= 2.f;
	return p + uv + uuv;
}

inline Quaternion& Quaternion::operator*=(float f)
{
	v *= f;
	w *= f;
	return *this;
}

inline Quaternion Quaternion::operator*(float f) const
{
	Quaternion Q = *this;
	Q.v *= f;
	Q.w *= f;
	return Q;
}


inline Quaternion Quaternion::Conjugate() const
{
	//Qr =  ( Qr.scalar, -Qr.vector )
	return Quaternion(-v, w);
}

inline float Quaternion::Norm() const
{
	//The square root of the product of a Quaternion with 
	//its conjugate is called its norm and is denoted ||q|| 
	//sqrt w2 + x2 + y2  + z2
	return (w * w) + v.length2();
}

inline Quaternion& Quaternion::Noramlize()
{
	const float mag = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z + w * w);
	if (mag)
	{
		const float inv = 1.0f / mag;
		v *= inv;
		w *= inv;
	}
	return *this;
}

inline Quaternion Quaternion::Normalized() const
{
	Quaternion Q = *this;
	float len = sqrt(this->Norm());
	float inv_len = 1.f / len;

	Q.v *= inv_len;
	Q.w *= inv_len;
	return Q;
}

inline Quaternion& Quaternion::Invert()
{
	*this = Conjugate().Noramlize();
	return *this;
}

inline Quaternion Quaternion::Inverted() const
{
	Quaternion q = Conjugate().Noramlize();
	return q;
}

inline Quaternion Quaternion::FromAxisAngle(const vec3f &axis, float angle)
{
	float half = angle * 0.5f * VML_PI / 180.f;
	float sinT = sin(half);
	Quaternion Q;
	Q.v = axis.normalized() * sinT;
	Q.w = cos(half);
	return Q.Normalized();
}

inline Quaternion Quaternion::RotationYawPitchRoll(float yaw, float pitch, float roll)
{
	yaw *= 0.5f * RADIANS;
	pitch *= 0.5f * RADIANS;
	roll *= 0.5f * RADIANS;

	const float syaw = sin(yaw);
	const float cyaw = cos(yaw);
	const float spitch = sin(pitch);
	const float cpitch = cos(pitch);
	const float sroll = sin(roll);
	const float croll = cos(roll);

	return Quaternion(
		syaw * cpitch * sroll + cyaw * spitch * croll,
		syaw * cpitch * croll - cyaw * spitch * sroll,
		cyaw * cpitch * sroll - syaw * spitch * croll,
		cyaw * cpitch * croll + syaw * spitch * sroll);
}

inline Matrix4x4 Quaternion::GetMatrix() const
{
	//http://www.j3d.org/matrix_faq/matrfaq_latest.html#Q56
	float x2 = 2.0f * v.x;
	float y2 = 2.0f * v.y;
	float z2 = 2.0f * v.z;

	float xx2 = x2 * v.x;
	float xy2 = x2 * v.y;
	float xz2 = x2 * v.z;
	float xw2 = x2 * w;

	float yy2 = y2 * v.y;
	float yz2 = y2 * v.z;
	float yw2 = y2 * w;

	float zz2 = z2 * v.z;
	float zw2 = z2 * w;

	Matrix4x4 M(
		1.f - yy2 - zz2, xy2 - zw2, xz2 + yw2, 0.f,			//0 row
		xy2 + zw2, 1.f - xx2 - zz2, yz2 - xw2, 0.f,			//1 row 
		xz2 - yw2, yz2 + xw2, 1.f - xx2 - yy2, 0.f,         //2 row
		0.f, 0.f, 0.f, 1.0f);

	return M;
}

inline Quaternion Quaternion::EulerToQuat(float x, float y, float z)
{
	x *= (0.5f * RADIANS);
	y *= (0.5f * RADIANS);
	z *= (0.5f * RADIANS);

	float sinx = sinf(x);
	float siny = sinf(y);
	float sinz = sinf(z);
	float cosx = cosf(x);
	float cosy = cosf(y);
	float cosz = cosf(z);

	Quaternion q;
	q.v.x = sinx * cosy * cosz + cosx * siny * sinz;
	q.v.y = cosx * siny * cosz - sinx * cosy * sinz;
	q.v.z = cosx * cosy * sinz - sinx * siny * cosz;
	q.w	  = cosx * cosy * cosz + sinx * siny * sinz;
	return q;
}

inline vec3f Quaternion::QuatToEuler(const Quaternion& q)
{
	vec3f v;

	v.x = atan2(2.f * q.v.x * q.w - 2 * q.v.y * q.v.z, 1 - 2 * q.v.x*q.v.x - 2 * q.v.z*q.v.z) * DEGREES;
	v.y = atan2(2 * q.v.y *q.w - 2 * q.v.x * q.v.z , 1 - 2 * q.v.y*q.v.y - 2 * q.v.z*q.v.z) * DEGREES;
	v.z = asin(2 * q.v.x*q.v.y + 2 * q.v.z * q.w) * DEGREES;

	return v;
}

inline void Quaternion::Interpolate(Quaternion& res, const Quaternion& start, const Quaternion& end, float factor)
{
	// calc cosine theta
	float cosom = start.v.x * end.v.x + start.v.y * end.v.y
		+ start.v.z * end.v.z + start.w * end.w;

	// adjust signs (if necessary)
	Quaternion newEnd = end;
	if (cosom < 0.0f)
	{
		cosom = -cosom;
		newEnd.v.x = -newEnd.v.x;
		newEnd.v.y = -newEnd.v.y;
		newEnd.v.z = -newEnd.v.z;
		newEnd.w = -newEnd.w;
	}

	// Calculate coefficients
	float sclp, sclq;
	if ((1.0f - cosom) > 0.0001f) // 0.0001 -> some epsillon
	{
		// Standard case (slerp)
		float omega, sinom;
		omega = std::acos(cosom);
		sinom = std::sin(omega);
		sclp = std::sin((1.0f - factor) * omega) / sinom;
		sclq = std::sin(factor * omega) / sinom;
	}
	else
	{
		// Very close, do linear interp (because it's faster)
		sclp = 1.0f - factor;
		sclq = factor;
	}

	res.v.x = sclp * start.v.x + sclq * newEnd.v.x;
	res.v.y = sclp * start.v.y + sclq * newEnd.v.y;
	res.v.z = sclp * start.v.z + sclq * newEnd.v.z;
	res.w = sclp * start.w + sclq * newEnd.w;
}

