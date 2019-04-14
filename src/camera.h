//
//  camera.h
//  tracer
//
//  Created by Rasmus Barringer on 2012-10-06.
//  Copyright (c) 2012 Lund University. All rights reserved.
//

#ifndef tracer_camera_h
#define tracer_camera_h

#include "ray.h"

class Camera {
private:
	float3 origin;
	float3 view;
	float3 right;
	float3 up;
	
public:
	void lookAt(const float3& origin, const float3& target, const float3& up, float fov, float near, float far, int width, int height);
	
	Ray generateRay(float px, float py) const;
};

#endif
