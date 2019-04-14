//
//  laineaccelerator.h
//  tracer
//
//  Created by Rasmus Barringer on 2012-10-10.
//  Copyright (c) 2012 Lund University. All rights reserved.
//

#ifndef tracer_laineaccelerator_h
#define tracer_laineaccelerator_h

#include "bvh.h"
#include "rayaccelerator.h"

template<unsigned ShortStackSize>
class LaineAccelerator : public RayAccelerator {
private:
	Bvh& bvh;
	
public:
	LaineAccelerator(Bvh& bvh) : bvh(bvh) {
	}
	
	virtual const Triangle* trace(Ray& ray) const {
		unsigned shortStack[ShortStackSize > 0 ? ShortStackSize : 1];
		unsigned stackHead = 0;
		
		const Triangle* nearest = 0;
		
		const Triangle* triangles = bvh.getTriangles();
		const Bvh::Node* nodes = bvh.getNodes();
		
		unsigned level = 0x80000000;
		unsigned popLevel = 0;
		unsigned trail = 0;
		
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
				
				if (t0 != maxT && t1 != maxT) {
					unsigned near = n.first;
					unsigned far = n.last;
					
					if (t0 > t1)
						std::swap(near, far);
					
					level >>= 1;
					
					if (trail & level) {
						node = far;
					}
					else {
						node = near;
						
						if (ShortStackSize > 0) {
							// Push far to short stack.
							if (stackHead < ShortStackSize) {
								shortStack[stackHead++] = far;
							}
							else {
								for (unsigned i = 0; i < ShortStackSize-1; ++i)
									shortStack[i] = shortStack[i+1];
								shortStack[ShortStackSize-1] = far;
							}
						}
					}
					continue;
				}
				else if (t0 != maxT || t1 != maxT) {
					level >>= 1;
					
					if (level != popLevel) {
						trail |= level;
						node = t0 != maxT ? n.first : n.last;
						continue;
					}
				}
			}
			
			trail &= (unsigned)(-(int)level);
			trail += level;
			
			unsigned temp = trail >> 1;
			level = (((temp-1) ^ temp) + 1);
			
			if (trail & 0x80000000)
				break;
			
			popLevel = level;
			
			if (ShortStackSize > 0 && stackHead > 0) {
				// Pop short stack.
				node = shortStack[--stackHead];
			}
			else {
				node = 0;
				level = 0x80000000;
			}
		}
		
		return nearest;
	}
};

#endif
