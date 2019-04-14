//
//  camera.cpp
//  tracer
//
//  Created by Rasmus Barringer on 2012-10-06.
//  Copyright (c) 2012 Lund University. All rights reserved.
//

#include "camera.h"

void Camera::lookAt(const float3& origin, const float3& target, const float3& up, float fov, float near, float far, int width, int height) {
	float3 forward = normalize(target - origin);
	float3 right = normalize(cross(forward, up));
	float3 cameraUp = cross(right, forward);
	
	float aspect = (float)height / (float)width;
	float imageExtentX = std::tan(0.5f * fov * (M_PI/180.0f));
	float imageExtentY = std::tan(0.5f * fov * aspect * (M_PI/180.0f));
	
	this->origin = origin;
	this->right = right * ( 2.0f/(float)width  * imageExtentX);
	this->up = cameraUp * (-2.0f/(float)height * imageExtentY);
	this->view = forward - right * imageExtentX + cameraUp * imageExtentY;
}

Ray Camera::generateRay(float px, float py) const {
	float3 d = normalize(view + right*px + up*py);
	
	Ray r;
	r.origin = origin;
	r.dir = d;
	r.invDir = float3(1.0f/d.x, 1.0f/d.y, 1.0f/d.z);
	r.minT = 1e-3f;
	r.maxT = 1e+6;
	return r;
}
