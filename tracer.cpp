//
//  tracer.cpp
//  tracer
//
//  Created by Rasmus Barringer on 2012-10-06.
//  Copyright (c) 2012 Lund University. All rights reserved.
//

#include "tracer.h"

Tracer::Tracer(Camera& camera, RayAccelerator& accelerator) : rnd(0), camera(camera), accelerator(accelerator) {
}

Tracer::HitPoint Tracer::samplePrimary(float px, float py) {
	HitPoint h = { false };
	
	const Triangle* tri;
	Ray r = camera.generateRay(px, py);
	
	if ((tri = accelerator.trace(r)) != 0) {
		h.hit = true;
		h.p = r.origin + r.dir * r.maxT;
		h.n = tri->n;
	}
	return h;
}

float Tracer::sampleAO(const Tracer::HitPoint& h) {
	const float radius = 32.0f;

	float3 up = std::fabs(h.n.x) > 0.1f ? float3(0.0f, 1.0f, 0.0f) : float3(1.0f, 0.0f, 0.0f);
	
	float3 u = normalize(cross(up, h.n));
	float3 v = cross(h.n, u);
	
	float f = 0.0f;
	
	for (int sy = 0; sy < aoSamplesRoot; ++sy) {
		for (int sx = 0; sx < aoSamplesRoot; ++sx) {
			float r1 = 2.0f*M_PI*(sx + rnd.nextFloat()) * (1.0f/aoSamplesRoot);
			float r2 = (sy + rnd.nextFloat()) * (1.0f/aoSamplesRoot);
			float r2s = std::sqrt(r2);
			
			float3 d = u*(std::cos(r1)*r2s) + v*(std::sin(r1)*r2s) + h.n*std::sqrt(1.0f-r2);
			d = normalize(d);
			
			Ray r;
			r.origin = h.p;
			r.dir = d;
			r.invDir = float3(1.0f/d.x, 1.0f/d.y, 1.0f/d.z);
			r.minT = 1e-3f;
			r.maxT = radius;
			
			if (accelerator.trace(r))
				f += 1.0f;
		}
	}
	
	return 1.0f - f * (1.0f / (aoSamplesRoot*aoSamplesRoot));
}
