//
//  vectormath.h
//  tracer
//
//  Created by Rasmus Barringer on 2012-10-06.
//  Copyright (c) 2012 Lund University. All rights reserved.
//

#ifndef tracer_vectormath_h
#define tracer_vectormath_h

#include <cmath>
#include <algorithm>

#ifdef M_PI
#undef M_PI
#endif
#define M_PI 3.14159265f

struct float3 {
	float x, y, z;
	
	float3() {}
	
	float3(float x) : x(x), y(x), z(x) {}
	
	float3(float x, float y, float z) : x(x), y(y), z(z) {}
	
	float operator [] (unsigned i) const {
		return (&x)[i];
	}
	
	float& operator [] (unsigned i) {
		return (&x)[i];
	}
	
	float3 operator + () const {
		return float3(x, y, z);
	}
	
	float3 operator - () const {
		return float3(-x, -y, -z);
	}
	
	float3 operator + (const float3& rhs) const {
		return float3(x + rhs.x, y + rhs.y, z + rhs.z);
	}
	
	float3 operator - (const float3& rhs) const {
		return float3(x - rhs.x, y - rhs.y, z - rhs.z);
	}
	
	float3 operator * (float rhs) const {
		return float3(x * rhs, y * rhs, z * rhs);
	}
	
	float length2() const {
		return x*x + y*y + z*z;
	}
	
	float length() const {
		return std::sqrt(length2());
	}
};

struct int3 {
	int x, y, z;
	
	int3() {}
	
	int3(int x, int y, int z) : x(x), y(y), z(z) {}
};

inline float3 normalize(const float3& v) {
	return v * (1.0f / v.length());
}

inline float dot(const float3& a, const float3& b) {
	return a.x*b.x + a.y*b.y + a.z*b.z;
}

inline float3 cross(const float3& a, const float3& b) {
	return float3(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
}

#endif
