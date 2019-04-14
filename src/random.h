//
//  random.h
//  tracer
//
//  Created by Rasmus Barringer on 2012-10-06.
//  Copyright (c) 2012 Lund University. All rights reserved.
//

#ifndef tracer_random_h
#define tracer_random_h

class Random {
private:
	unsigned int w;
	unsigned int z;

public:
	Random(int seed) {
		w = seed + 1;
		z = seed * seed + seed + 2;
	}
	
	int nextInt() {
		z = 36969 * (z & 65535) + (z >> 16);
		w = 18000 * (w & 65535) + (w >> 16);
		
		return (z << 16) + w;
	}
	
	float nextFloat() {
		return (nextInt() & 0xffff) * (1.0f / 0xffff);
	}
};

#endif
