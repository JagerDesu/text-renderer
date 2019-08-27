//// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//// PARTICULAR PURPOSE.
////
//// Copyright (c) Microsoft Corporation. All rights reserved

#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <cstring>

// This header defines math and matrix helper functions and structures used 
// by DirectX SDK samples.

// Common Constants

#define PI_F 3.1415927f

namespace Math {

	// Template Vector & Matrix Classes

	template <class T> struct Vector2
	{
		union
		{
			struct
			{
				T x;
				T y;
			};
			struct
			{
				T r;
				T g;
			};
			struct
			{
				T u;
				T v;
			};
		};

		T& operator[](size_t index)
		{
			return reinterpret_cast<T*>(this)[index];
		}

		Vector2(T _x = 0, T _y = 0) : x(_x), y(_y) { }
	};

	template <class T> struct Vector3
	{
		union
		{
			struct
			{
				T x;
				T y;
				T z;
			};
			struct
			{
				T r;
				T g;
				T b;
			};
			struct
			{
				T u;
				T v;
				T w;
			};
		};

		T& operator[](size_t index)
		{
			return reinterpret_cast<T*>(this)[index];
		}

		Vector3(T _x = 0, T _y = 0, T _z = 0) : x(_x), y(_y), z(_z) { }
	};

	template <class T> struct Vector4
	{
		union
		{
			struct
			{
				T x;
				T y;
				T z;
				T w;
			};
			struct
			{
				T r;
				T g;
				T b;
				T a;
			};
		};

		T& operator[](size_t index)
		{
			return reinterpret_cast<T*>(this)[index];
		}

		Vector4(T _x = 0, T _y = 0, T _z = 0, T _w = 0) : x(_x), y(_y), z(_z), w(_w) { }
	};

	template<typename T>
	struct Matrix4x4 {
		static const size_t NumRows = 4, NumColumns = 4;
	public:
		Matrix4x4(T value = 0) {
			for (size_t i = 0; i < NumRows; i++)
				rows[i] = Vector4<T>(value, value, value);
		}

		Matrix4x4(T* value) {
			memcpy(rows, value, sizeof(rows));
		}

		template <typename U>
		Matrix4x4(U a, U b, U c, U d) {
			rows[0] = a;
			rows[1] = b;
			rows[2] = c;
			rows[3] = d;
		}

		Matrix4x4(const Vector4<T>* value[NumRows]) {
			for (size_t i = 0; i < NumRows; i++)
				rows[i] = value[i];
		}

		Vector4<T>& operator[](size_t index) {
			return rows[index];
		}

		const Vector4<T>& operator[](size_t index) const {
			return rows[index];
		}

	private:
		Vector4<T> rows[NumRows];
	};

	template<typename T>
	struct Matrix3x3 {
		static const size_t NumRows = 3, NumColumns = 3;
	public:
		Matrix3x3(T value = 0) {
			for (size_t i = 0; i < NumRows; i++)
				rows[i] = Vector3<T>(value, value, value);
		}

		template <typename U>
		Matrix3x3(U a, U b, U c) {
			rows[0] = a;
			rows[1] = b;
			rows[2] = c;
		}

		Matrix3x3(const Vector3<T> value[NumRows]) {
			for (size_t i = 0; i < NumRows; i++)
				rows[i] = value[i];
		}

		Vector3<T>& operator[](size_t index) {
			return rows[index];
		}

		const Vector3<T>& operator[](size_t index) const {
			return rows[index];
		}

	private:
		Vector3<T> rows[NumRows];
	};

	// Template Vector Operations

	template <class T>
	T Dot(Vector2<T> a, Vector2<T> b)
	{
		return a.x * b.x + a.y * b.y;
	}

	template <class T>
	T Dot(Vector3<T> a, Vector3<T> b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

	template <class T>
	T Dot(Vector4<T> a, Vector4<T> b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z + a.w + b.w;
	}

	template <class T>
	T Length(Vector2<T> a)
	{
		return sqrt(a.x * a.x + a.y * a.y);
	}

	template <class T>
	T Length(Vector3<T> a)
	{
		return static_cast<T>(sqrt(a.x * a.x + a.y * a.y + a.z * a.z));
	}

	template <class T>
	T Length(Vector4<T> a)
	{
		return static_cast<T>(sqrt(a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w));
	}

	template <class T>
	Vector3<T> Cross(Vector3<T> a, Vector3<T> b)
	{
		return Vector3<T>((a.y * b.z) - (a.z * b.y), (a.z * b.x) - (a.x * b.z), (a.x * b.y) - (a.y * b.x));
	}

	template <class T>
	Vector2<T> Normalize(Vector2<T> a)
	{
		T len = Length(a);
		return Vector2<T>(a.x / len, a.y / len);
	}

	template <class T>
	Vector3<T> Normalize(Vector3<T> a)
	{
		T len = Length(a);
		return Vector3<T>(a.x / len, a.y / len, a.z / len);
	}

	template <class T>
	Vector4<T> Normalize(Vector4<T> a)
	{
		T len = Length(a);
		return Vector4<T>(a.x / len, a.y / len, a.z / len, a.w / len);
	}

	// Template Vector Operators

	template <class T>
	Vector2<T> operator-(Vector2<T> a, Vector2<T> b)
	{
		return Vector2<T>(a.x - b.x, a.y - b.y);
	}

	template <class T>
	Vector2<T> operator-(Vector2<T> a)
	{
		return Vector2<T>(-a.x, -a.y);
	}

	template <class T>
	Vector3<T> operator-(Vector3<T> a, Vector3<T> b)
	{
		return Vector3<T>(a.x - b.x, a.y - b.y, a.z - b.z);
	}

	template <class T>
	Vector3<T> operator-(Vector3<T> a)
	{
		return Vector3<T>(-a.x, -a.y, -a.z);
	}

	template <class T>
	Vector4<T> operator-(Vector4<T> a, Vector4<T> b)
	{
		return Vector4<T>(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
	}

	template <class T>
	Vector4<T> operator-(Vector4<T> a)
	{
		return Vector4<T>(-a.x, -a.y, -a.z, -a.w);
	}

	template <class T>
	Vector2<T> operator+(Vector2<T> a, Vector2<T> b)
	{
		return Vector2<T>(a.x + b.x, a.y + b.y);
	}

	template <class T>
	Vector3<T> operator+(Vector3<T> a, Vector3<T> b)
	{
		return Vector3<T>(a.x + b.x, a.y + b.y, a.z + b.z);
	}

	template <class T>
	Vector4<T> operator+(Vector4<T> a, Vector4<T> b)
	{
		return Vector4<T>(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
	}

	template <class T>
	Vector2<T> operator*(Vector2<T> a, T s)
	{
		return Vector2<T>(a.x * s, a.y * s);
	}

	template <class T>
	Vector2<T> operator*(T s, Vector2<T> a)
	{
		return a * s;
	}

	template <class T>
	Vector2<T> operator*(Vector2<T> a, Vector2<T> b)
	{
		return Vector2<T>(a.x * b.x, a.y * b.y);
	}

	template <class T>
	Vector2<T> operator/(Vector2<T> a, T s)
	{
		return Vector2<T>(a.x / s, a.y / s);
	}

	template <class T>
	Vector3<T> operator*(Vector3<T> a, T s)
	{
		return Vector3<T>(a.x * s, a.y * s, a.z * s);
	}

	template <class T>
	Vector3<T> operator*(T s, Vector3<T> a)
	{
		return a * s;
	}

	template <class T>
	Vector3<T> operator*(Vector3<T> a, Vector3<T> b)
	{
		return Vector3<T>(a.x * b.x, a.y * b.y, a.z * b.z);
	}

	template <class T>
	Vector3<T> operator/(Vector3<T> a, T s)
	{
		return Vector3<T>(a.x / s, a.y / s, a.z / s);
	}

	template <class T>
	Vector4<T> operator*(Vector4<T> a, T s)
	{
		return Vector4<T>(a.x * s, a.y * s, a.z * s, a.w * s);
	}

	template <class T>
	Vector4<T> operator*(T s, Vector4<T> a)
	{
		return a * s;
	}

	template <class T>
	Vector4<T> operator*(Vector4<T> a, Vector4<T> b)
	{
		return Vector4<T>(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
	}

	template <class T>
	Vector4<T> operator/(Vector4<T> a, T s)
	{
		return Vector4<T>(a.x / s, a.y / s, a.z / s, a.w / s);
	}

	template <class T>
	Matrix4x4<T> operator*(Matrix4x4<T> value, T scalar) {
		Matrix4x4<T> out = value;
		for (size_t i = 0; i < Matrix4x4<T>::NumRows; i++)
			value[i] = value[i] * scalar;
		return out;
	}

	template <class T>
	Vector4<T> operator*(Vector4<T> a, Matrix4x4<T> b) {
		Vector4<T> out;
		out.x = Dot(a, Vector4<T>(b[0][0], b[1][0], b[2][0], b[3][0]));
		out.y = Dot(a, Vector4<T>(b[0][1], b[1][1], b[2][1], b[3][1]));
		out.z = Dot(a, Vector4<T>(b[0][2], b[1][2], b[2][2], b[3][2]));
		out.w = Dot(a, Vector4<T>(b[0][3], b[1][3], b[2][3], b[3][3]));
		return out;
	}

	template <class T>
	Matrix4x4<T> operator*(Matrix4x4<T> a, Matrix4x4<T> b) {
		Matrix4x4<T> out;
		for (size_t r = 0; r < Matrix4x4<T>::NumRows; r++) {
			for (size_t c = 0; c < Matrix4x4<T>::NumColumns; c++) {
				out[r][c] = 0;
				for (size_t k = 0; k < Matrix4x4<T>::NumRows; k++)
					out[r][c] += a[r][k] * b[k][c];
			}
		}
		return out;
	}


	template <class T>
	Matrix3x3<T> operator*(Matrix3x3<T> value, T scalar) {
		Matrix3x3<T> out = value;
		for (size_t i = 0; i < Matrix3x3<T>::NumRows; i++)
			value[i] = value[i] * scalar;
		return out;
	}


	template <class T>
	Matrix3x3<T> operator*(Matrix3x3<T> a, Matrix3x3<T> b)
	{
		Matrix3x3<T> out;
		for (size_t r = 0; r < Matrix3x3<T>::NumRows; r++) {
			for (size_t c = 0; c < Matrix3x3<T>::NumColumns; c++) {
				out[r][c] = 0;
				for (size_t k = 0; k < Matrix3x3<T>::NumRows; k++)
					out[r][c] += a[r][k] * b[k][c];
			}
		}
		return out;
	}

	// Template Matrix Operations

	template <class T>
	void Transpose(const Matrix4x4<T> in, Matrix4x4<T>& out) {
		for (size_t r = 0; r < Matrix4x4<T>::NumRows; r++) {
			for (size_t c = 0; c < Matrix4x4<T>::NumColumns; c++) {
				((float*)& out[r])[c] = ((float*)& in[c])[r];
			}
		}
	}

	template <class T>
	void Transpose(const Matrix3x3<T>& in, Matrix3x3<T>& out) {
		out = Matrix3x3<T>(
			Vector3<T>(in[0].x, in[1].y, in[2].z),
			Vector3<T>(in[0].x, in[1].y, in[2].z),
			Vector3<T>(in[0].x, in[1].y, in[2].z)
			);
	}

	// Common HLSL-compatible vector typedefs

	typedef unsigned int uint;

	typedef Vector2<float> float2;
	typedef Vector3<float> float3;
	typedef Vector4<float> float4;

	typedef Matrix4x4<float> float4x4;

	template<typename T>
	inline void Identity(Matrix4x4<T>& out) {
		out[0] = Vector4<T>(1, 0, 0, 0);
		out[1] = Vector4<T>(0, 1, 0, 0);
		out[2] = Vector4<T>(0, 0, 1, 0);
		out[3] = Vector4<T>(0, 0, 0, 1);
	}

	template<typename T>
	inline void Identity(Matrix3x3<T>& out) {
		out[0] = Vector3<T>(1, 0, 0);
		out[1] = Vector3<T>(0, 1, 0);
		out[2] = Vector3<T>(0, 0, 1);
	}

	template<typename T>
	inline void Translation(const Vector3<T>& position, Matrix4x4<T>& out) {
		out[0] = Vector4<T>(1, 0, 0, 0);
		out[1] = Vector4<T>(0, 1, 0, 0);
		out[2] = Vector4<T>(0, 0, 1, 0);
		out[3] = Vector4<T>(position.x, position.y, position.z, 1);
	}

	template<typename T>
	inline void Translation(const Vector2<T>& position, Matrix3x3<T>& out) {
		out[0] = Vector3<T>(1, 0, 0);
		out[1] = Vector3<T>(0, 1, 0);
		out[2] = Vector3<T>(position.x, position.y, 1);
	}

	template<typename T>
	inline void Scale(const Vector3<T>& scale, Matrix4x4<T>& out) {
		out[0] = Vector4<T>(scale.x, 0, 0, 0);
		out[1] = Vector4<T>(0, scale.y, 0, 0);
		out[2] = Vector4<T>(0, 0, scale.z, 0);
		out[3] = Vector4<T>(0, 0, 0, 1);
	}

	template<typename T>
	inline void Scale(const Vector2<T>& scale, Matrix3x3<T>& out) {
		out[0] = Vector2<T>(scale.x, 0, 0);
		out[1] = Vector2<T>(0, scale.y, 0);
		out[2] = Vector2<T>(0, 0, 1);
	}

	template<typename T>
	inline void RotationX(T angleInRadians, Matrix4x4<T>& output) {
		T sinAngle = sinf(angleInRadians);
		T cosAngle = cosf(angleInRadians);

		T m[] =
		{
			1, 0,        0,         0.0,
			0, cosAngle, -sinAngle, 0.0,
			0, sinAngle, cosAngle,  0.0,
			0, 0,        0,         1.0
		};

		memcpy(&output, m, sizeof(m));
	}

	template <typename T>
	inline void RotationY(T angleInRadians, Matrix4x4<T>& output) {
		T sinAngle = sinf(angleInRadians);
		T cosAngle = cosf(angleInRadians);

		T m[] = {
			cosAngle,  0, sinAngle, 0,
			0,         1, 0,        0,
			-sinAngle, 0, cosAngle, 0,
			0,         0, 0,        1
		};

		memcpy(&output, m, sizeof(m));
	}

	template <typename T>
	inline void RotationZ(float angleInRadians, Matrix4x4<T>& output) {
		T sinAngle = sinf(angleInRadians);
		T cosAngle = cosf(angleInRadians);

		T m[] = {
			cosAngle, -sinAngle, 0, 0,
			sinAngle, cosAngle,  0, 0,
			0,        0,         1, 0,
			0,        0,         0, 1
		};

		memcpy(&output, m, sizeof(m));
	}

	// 3D Rotation matrix for an arbitrary axis specified by x, y and z 
	template <typename T>
	inline void RotationArbitrary(Vector3<T> axis, T angleInRadians, Matrix4x4<T>& output) {
		axis = Normalize(axis);

		T sinAngle = sinf(angleInRadians);
		T cosAngle = cosf(angleInRadians);
		T oneMinusCosAngle = 1 - cosAngle;

		T m[16] = {};

		m[0] = 1.0f + oneMinusCosAngle * (axis.x * axis.x - 1.0f);
		m[1] = axis.z * sinAngle + oneMinusCosAngle * axis.x * axis.y;
		m[2] = -axis.y * sinAngle + oneMinusCosAngle * axis.x * axis.z;
		m[3] = 0.0f;

		m[4] = -axis.z * sinAngle + oneMinusCosAngle * axis.y * axis.x;
		m[5] = 1.0f + oneMinusCosAngle * (axis.y * axis.y - 1.0f);
		m[6] = axis.x * sinAngle + oneMinusCosAngle * axis.y * axis.z;
		m[7] = 0.0f;

		m[8] = axis.y * sinAngle + oneMinusCosAngle * axis.z * axis.x;
		m[9] = -axis.x * sinAngle + oneMinusCosAngle * axis.z * axis.y;
		m[10] = 1.0f + oneMinusCosAngle * (axis.z * axis.z - 1.0f);
		m[11] = 0.0f;

		m[12] = 0.0f;
		m[13] = 0.0f;
		m[14] = 0.0f;
		m[15] = 1.0f;

		memcpy(&output, m, sizeof(m));
	}

	template<typename T>
	void BuildOrthoMatrix(
		Matrix4x4<T>& output,
		T width,
		T height,
		T znear,
		T zfar
	) {
		T m[] = {
			2 / width, 0,        0,                  0,
			0,       2 / height, 0,                  0,
			0,       0,        1 / (znear - zfar),     0,
			0,       0,        znear / (znear - zfar), 1
		};

		memcpy(&output, m, sizeof(m));
	}

	template <typename T>
	void BuildProjMatrix(
		Matrix4x4<T>& output,
		T aspect,
		T fov,
		T znear,
		T zfar
	) {
		T yScale = /*cotan*/(T)(1 / tan(fov / 2));
		T xScale = (T)yScale / aspect;
		T m[16] = {
			xScale, 0,      0,                              0,
			0,      yScale, 0,                              0,
			0,      0,      zfar / (znear - zfar),         -1,
			0,      0,      znear * zfar / (znear - zfar),  0
		};
		output = *(Matrix4x4<T>*) & m;
	}

	template <typename T>
	void BuildLookAtMatrix(
		Matrix4x4<T>& output,
		const Math::Vector3<T> eye,
		const Math::Vector3<T> at,
		const Math::Vector3<T> up
	) {
		Math::Vector3<T> zaxis = Normalize(eye - at);
		Math::Vector3<T> xaxis = Normalize(Cross(up, zaxis));
		Math::Vector3<T> yaxis = Cross(zaxis, xaxis);

		output[0] = Math::Vector4<T>(xaxis.x, yaxis.x, zaxis.x, 0);
		output[1] = Math::Vector4<T>(xaxis.y, yaxis.y, zaxis.y, 0);
		output[2] = Math::Vector4<T>(xaxis.z, yaxis.z, zaxis.z, 0);
		output[3] = Math::Vector4<T>(-Dot(xaxis, eye), -Dot(yaxis, eye), -Dot(zaxis, eye), 1);
	}

	inline float ToRadians(float degrees) {
		return (degrees * PI_F) / 180.0f;
	}

	using Vector2f = Vector2<float>;
	using Vector3f = Vector3<float>;
	using Vector4f = Vector4<float>;

	using Matrix4x4f = Matrix4x4<float>;
	using Matrix3x3f = Matrix4x4<float>;

	using Vector4i = Vector4<int>;
	using Vector3i = Vector3<int>;
	using Vector2i = Vector2<int>;

}