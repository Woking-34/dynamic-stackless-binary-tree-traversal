//
//  implicitaaccelerator.h
//  tracer
//
//  Created by Rasmus Barringer on 2012-10-07.
//  Copyright (c) 2012 Lund University. All rights reserved.
//

#ifndef tracer_implicitaaccelerator_h
#define tracer_implicitaaccelerator_h

#include "bvh.h"
#include "rayaccelerator.h"

class ImplicitAAccelerator : public RayAccelerator {
private:
	Bvh& bvh;
	
public:
	ImplicitAAccelerator(Bvh& bvh);
	
	virtual const Triangle* trace(Ray& ray) const;
};

#endif
