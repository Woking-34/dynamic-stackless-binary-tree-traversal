//
//  implicitaaccelerator.cpp
//  tracer
//
//  Created by Rasmus Barringer on 2012-10-07.
//  Copyright (c) 2012 Lund University. All rights reserved.
//

#include "implicitaaccelerator.h"

ImplicitAAccelerator::ImplicitAAccelerator(Bvh& bvh) : bvh(bvh) {
	bvh.makeImplicit();
}

const Triangle* ImplicitAAccelerator::trace(Ray& ray) const {
	const Triangle* nearest = 0;
	
	const Triangle* triangles = bvh.getTriangles();
	const Bvh::Node* nodes = bvh.getNodes();

	unsigned node = 1;
	unsigned swapMask = 0;
	
	do {
		unsigned idx = node - 1 + swapMask - ((node & swapMask) << 1);
		const Bvh::Node& n = nodes[idx];
		
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
			
			int firsthit = t0 != maxT;
			int lasthit = t1 != maxT;
			
			if (firsthit | lasthit) {
				int single = firsthit ^ lasthit;
				swapMask += swapMask + ((t0 >= t1) ^ single);
				node += node + single;
				continue;
			}
		}

		++node;
		
#ifdef _WIN32
		int up;
		if (!_BitScanForward((unsigned long*)&up, node))
			up = 0;
#else
		int up = __builtin_ctz(node);
#endif
		
		node >>= up;
		swapMask >>= up;
	}
	while (node > 1);
	
	return nearest;
}
