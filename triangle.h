//
//  triangle.h
//  tracer
//
//  Created by Rasmus Barringer on 2012-10-06.
//  Copyright (c) 2012 Lund University. All rights reserved.
//

#ifndef tracer_triangle_h
#define tracer_triangle_h

#include "ray.h"

struct Triangle {
	float3 p0, p1, p2;
	float3 n;
	float padding[4];
	
	float3 center() const {
		return (p0 + p1 + p2) * (1.0f / 3.0f);
	}
	
	inline bool intersect(Ray& ray) const {
		const float epsilon = 1e-6f;
		
		float3 d = ray.dir;
		float3 p = ray.origin;
		
		float3 e1 = p1 - p0;
		float3 e2 = p2 - p0;
		
		float3 n = cross(e1, e2);
		float s = -dot(d, n);
		
		if (std::fabs(s) < epsilon)
			return false;
		
		s = 1.0f / s;
		
		float3 r = p - p0;
		float t = dot(r, n) * s;
		
		if (t <= ray.minT || t >= ray.maxT)
			return false;
		
		float3 q = -cross(d, r);
		float v = dot(q, e2) * s;
		
		if (v < 0.0f)
			return false;
		
		float w = -dot(e1, q) * s;
		
		if (w < 0.0f || v+w > 1.0f)
			return false;
		
		ray.maxT = t;
		return true;
	}
};

#endif
