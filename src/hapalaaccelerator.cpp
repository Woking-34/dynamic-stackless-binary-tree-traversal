//
//  hapalaaccelerator.cpp
//  tracer
//
//  Created by Rasmus Barringer on 2012-10-07.
//  Copyright (c) 2012 Lund University. All rights reserved.
//

#include "hapalaaccelerator.h"

inline unsigned nearChild(const Bvh::Node& node, const Ray& ray) {
	if (ray.dir[node.kind-1] > 0.0f)
		return node.first;
	else
		return node.last;
}

HapalaAccelerator::HapalaAccelerator(Bvh& bvh) : bvh(bvh) {
}

const Triangle* HapalaAccelerator::trace(Ray& ray) const {
	const Triangle* nearest = 0;
	
	const Triangle* triangles = bvh.getTriangles();
	const Bvh::Node* nodes = bvh.getNodes();
	
	unsigned last = 0xffffffff;
	unsigned current = 0;
	
	do {
		const Bvh::Node& n = nodes[current];

		if (!n.kind) {
			for (unsigned i = n.first; i < n.last; ++i) {
				if (triangles[i].intersect(ray))
					nearest = &triangles[i];
			}
			
			// Go up.
			last = current;
			current = n.parent;
			continue;
		}
		
		unsigned near = nearChild(n, ray);
		unsigned far = near == n.first ? n.last : n.first;
		
		if (last == far) {
			// Already returned from far child − traverse up.
			last = current;
			current = n.parent;
			continue;
		}
		
		// If coming from parent, try near child, else far child.
		unsigned tryChild = (last == n.parent) ? near : far;
		const Aabb& bb = tryChild == n.first ? n.left : n.right;
		
		float maxT = ray.maxT;
		
		if (bb.intersect(ray) != maxT) {
			// If box was hit, descend.
			last = current;
			current = tryChild;
			continue;
		}

		if (tryChild == near) {
			// Next is far.
			last = near;
		}
		else {
			// Go up instead.
			last = current;
			current = n.parent;
		}
	}
	while (current != 0xffffffff);
	
	return nearest;
}
