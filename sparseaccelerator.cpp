//
//  sparseaccelerator.cpp
//  tracer
//
//  Created by Rasmus Barringer on 2012-10-07.
//  Copyright (c) 2012 Lund University. All rights reserved.
//

#include "sparseaccelerator.h"

inline int sibling(int node) {
	return node + ((node & 1) << 1) - 1;
}

SparseAccelerator::SparseAccelerator(Bvh& bvh) : bvh(bvh) {
}

const Triangle* SparseAccelerator::trace(Ray& ray) const {
	const Triangle* nearest = 0;
	
	const Triangle* triangles = bvh.getTriangles();
	const Bvh::Node* nodes = bvh.getNodes();

	unsigned levelIndex = 0;
	unsigned node = 0;
	
	do {
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
				levelIndex += levelIndex;
				if (t0 < t1) {
					node = n.first;
					if (t1 == maxT)
						++levelIndex;
				}
				else {
					node = n.last;
					if (t0 == maxT)
						++levelIndex;
				}
				continue;
			}
		}
		
		++levelIndex;
		
		while ((levelIndex & 1) == 0) {
			node = nodes[node].parent;
			levelIndex >>= 1;
		}
		
		node = sibling(node);
	}
	while (node != 0xffffffff);
	
	return nearest;
}
