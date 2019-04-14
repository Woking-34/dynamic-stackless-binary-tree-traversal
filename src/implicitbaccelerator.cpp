//
//  implicitbaccelerator.cpp
//  tracer
//
//  Created by Rasmus Barringer on 2013-02-14.
//  Copyright (c) 2013 Lund University. All rights reserved.
//

#include "implicitbaccelerator.h"

#ifdef _WIN32
#include <intrin.h>
#endif

inline unsigned sibling(unsigned levelIndex) {
	return levelIndex + 1 - ((levelIndex & 1) << 1);
}

ImplicitBAccelerator::ImplicitBAccelerator(Bvh& bvh) : bvh(bvh) {
	bvh.makeImplicit();
}

const Triangle* ImplicitBAccelerator::trace(Ray& ray) const {
	const Triangle* nearest = 0;
	
	const Triangle* triangles = bvh.getTriangles();
	const Bvh::Node* nodes = bvh.getNodes();

	unsigned node = 1;
	unsigned traversal = 0;

	for (;;) {
		const Bvh::Node& n = nodes[node-1];
		
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
				node += node + (t0 >= t1);
				traversal += traversal + single;
				continue;
			}
		}

		++traversal;
		
#ifdef _WIN32
		int up;
		if (!_BitScanForward((unsigned long*)&up, traversal))
			up = 0;
#else
		int up = __builtin_ctz(traversal);
#endif
		
		traversal >>= up;
		node >>= up;
		
		unsigned level = node;
		
		node = sibling(node);

		if (level <= 1)
			break;
	}
	
	return nearest;
}
