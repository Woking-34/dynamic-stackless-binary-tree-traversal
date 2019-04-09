//
//  sparseaccelerator.h
//  tracer
//
//  Created by Rasmus Barringer on 2012-10-07.
//  Copyright (c) 2012 Lund University. All rights reserved.
//

#ifndef tracer_sparseaccelerator_h
#define tracer_sparseaccelerator_h

#include "bvh.h"
#include "rayaccelerator.h"

class SparseAccelerator : public RayAccelerator {
private:
	Bvh& bvh;
	
public:
	SparseAccelerator(Bvh& bvh);
	
	virtual const Triangle* trace(Ray& ray) const;
};

#endif
