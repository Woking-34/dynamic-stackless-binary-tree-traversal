//
//  hapalaaccelerator.h
//  tracer
//
//  Created by Rasmus Barringer on 2012-10-07.
//  Copyright (c) 2012 Lund University. All rights reserved.
//

#ifndef tracer_hapalaaccelerator_h
#define tracer_hapalaaccelerator_h

#include "bvh.h"
#include "rayaccelerator.h"

class HapalaAccelerator : public RayAccelerator {
private:
	Bvh& bvh;
	
public:
	HapalaAccelerator(Bvh& bvh);
	
	virtual const Triangle* trace(Ray& ray) const;
};

#endif
