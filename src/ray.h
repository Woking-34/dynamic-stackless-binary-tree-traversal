//
//  ray.h
//  tracer
//
//  Created by Rasmus Barringer on 2012-10-06.
//  Copyright (c) 2012 Lund University. All rights reserved.
//

#ifndef tracer_ray_h
#define tracer_ray_h

#include "vectormath.h"

struct Ray {
	float3 origin, dir, invDir;
	float minT, maxT;
};

#endif
