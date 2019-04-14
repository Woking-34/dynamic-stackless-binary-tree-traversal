//
//  mesh.cpp
//  tracer
//
//  Created by Rasmus Barringer on 2012-10-06.
//  Copyright (c) 2012 Lund University. All rights reserved.
//

#include "mesh.h"
#include <string>
#include <fstream>
#include <iostream>
#include <stdexcept>

using namespace std;

Mesh::Mesh(const char* filename, bool flipFaces) {
	ifstream file(filename);
	
	if (!file)
		throw runtime_error((string("cannot find ") + filename).c_str());
	
	string line;
	
	vector<int3> fileTriangles;
	vector<float3> filePositions;

	while (getline(file, line)) {
		float x, y, z;
		int a[3], b[3], c[3], d[3];
		
		if (sscanf(line.c_str(), "v %f %f %f", &x, &y, &z) == 3) {
			filePositions.push_back(float3(x, y, z));
		}
		else if (sscanf(line.c_str(), "f %d %d %d", &a[0], &b[0], &c[0]) == 3) {
			fileTriangles.push_back(int3(c[0]-1, b[0]-1, a[0]-1));
		}
		else if (sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d", &a[0], &a[1], &a[2], &b[0], &b[1], &b[2], &c[0], &c[1], &c[2], &d[0], &d[1], &d[2]) == 12 ||
				 sscanf(line.c_str(), "f %d//%d %d//%d %d//%d %d//%d", &a[0], &a[2], &b[0], &b[2], &c[0], &c[2], &d[0], &d[2]) == 8) {
			fileTriangles.push_back(int3(a[0]-1, b[0]-1, c[0]-1));
			fileTriangles.push_back(int3(a[0]-1, c[0]-1, d[0]-1));
		}
		else if (sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d", &a[0], &a[1], &a[2], &b[0], &b[1], &b[2], &c[0], &c[1], &c[2]) == 9 ||
				 sscanf(line.c_str(), "f %d//%d %d//%d %d//%d", &a[0], &a[2], &b[0], &b[2], &c[0], &c[2]) == 6) {
			fileTriangles.push_back(int3(a[0]-1, b[0]-1, c[0]-1));
		}
	}
	
	for (size_t i = 0; i < fileTriangles.size(); ++i) {
		int3 p = fileTriangles[i];
		
		float3 p0 = filePositions[p.x];
		float3 p1 = filePositions[p.y];
		float3 p2 = filePositions[p.z];
		
		if (flipFaces)
			std::swap(p0, p1);

		float3 a = p1 - p0;
		float3 b = p2 - p0;
		
		float3 n = normalize(cross(a, b));
		
		Triangle t = { p0, p1, p2, n };
		
		triangles.push_back(t);
	}
}

std::vector<Triangle, AlignedAllocator<Triangle, 64> >& Mesh::getTriangles() {
	return triangles;
}
