//
//  stackaccelerator.cpp
//  tracer
//
//  Created by Rasmus Barringer on 2012-10-06.
//  Copyright (c) 2012 Lund University. All rights reserved.
//

#include "stackaccelerator.h"

StackAccelerator::StackAccelerator(Bvh& bvh) : bvh(bvh) {
}

const Triangle* StackAccelerator::trace(Ray& ray) const {
	const Triangle* nearest = 0;
	
	const Triangle* triangles = bvh.getTriangles();
	const Bvh::Node* nodes = bvh.getNodes();
	
	unsigned stack[32];
	unsigned* stackPtr = stack;
	
	unsigned node = 0;
	
	for (;;) {
		const Bvh::Node& n = nodes[node];
		
		if (!n.kind) {
			for (unsigned i = n.first; i < n.last; ++i) {
				if (triangles[i].intersect(ray))
					nearest = &triangles[i];
			}
		}
		else {
			float maxT = ray.maxT;
			
			float t0 = n.left.intersect(ray);
			float t1 = n.right.intersect(ray);
			
			if (t0 != maxT || t1 != maxT) {
				if (t0 < t1) {
					node = n.first;
					if (t1 != maxT)
						*(stackPtr++) = n.last;
				}
				else {
					node = n.last;
					if (t0 != maxT)
						*(stackPtr++) = n.first;
				}
				continue;
			}
		}
		
		if (stack == stackPtr)
			break;
		
		node = *(--stackPtr);
	}
	
	return nearest;
}
