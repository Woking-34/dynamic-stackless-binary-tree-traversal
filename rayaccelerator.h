//
//  rayaccelerator.h
//  tracer
//
//  Created by Rasmus Barringer on 2012-10-06.
//  Copyright (c) 2012 Lund University. All rights reserved.
//

#ifndef tracer_rayaccelerator_h
#define tracer_rayaccelerator_h

#include "ray.h"
#include "mesh.h"

class RayAccelerator {
public:
	virtual const Triangle* trace(Ray& ray) const = 0;
	virtual ~RayAccelerator() {}
};

#endif
