//
//  aabb.h
//  tracer
//
//  Created by Rasmus Barringer on 2012-10-06.
//  Copyright (c) 2012 Lund University. All rights reserved.
//

#ifndef tracer_aabb_h
#define tracer_aabb_h

#include "triangle.h"
#include "ray.h"

struct Aabb {
	float3 min, max;
	
	void reset() {
		min = float3(1e8f, 1e8f, 1e8f);
		max = float3(-1e8f, -1e8f, -1e8f);
	}
	
	float3 center() const {
		return (min + max) * 0.5f;
	}
	
	void include(const float3& v) {
		min.x = std::min(min.x, v.x);
		min.y = std::min(min.y, v.y);
		min.z = std::min(min.z, v.z);
		
		max.x = std::max(max.x, v.x);
		max.y = std::max(max.y, v.y);
		max.z = std::max(max.z, v.z);
	}
	
	void include(const Triangle& t) {
		include(t.p0);
		include(t.p1);
		include(t.p2);
	}
	
	void include(const Aabb& aabb) {
		min.x = std::min(min.x, aabb.min.x);
		min.y = std::min(min.y, aabb.min.y);
		min.z = std::min(min.z, aabb.min.z);
		
		max.x = std::max(max.x, aabb.max.x);
		max.y = std::max(max.y, aabb.max.y);
		max.z = std::max(max.z, aabb.max.z);
	}
	
	unsigned largestAxis() const {
		float3 d = max - min;
		return (d.x > d.y && d.x > d.z) ? 0 : (d.y > d.z ? 1 : 2);
	}
	
	float surfaceArea() const {
		float3 d = max - min;
		return 2.0f * (d.x*d.y + d.x*d.z + d.y*d.z);
	}
	
	inline float intersect(const Ray& ray) const {
		float t0 = ray.minT;
		float t1 = ray.maxT;

		for (int i = 0; i < 3; ++i) {
			float near = (min[i] - ray.origin[i]) * ray.invDir[i];
			float far  = (max[i] - ray.origin[i]) * ray.invDir[i];
			
			if (near > far)
				std::swap(near, far);
			
			if (near > t0) t0 = near;
			if (far < t1) t1 = far;
			
			if (t0 > t1)
				return ray.maxT;
		}
		
		return t0;
	}
};

#endif
