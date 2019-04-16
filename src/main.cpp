//
//  main.cpp
//  tracer
//
//  Created by Rasmus Barringer on 2012-10-06.
//  Copyright (c) 2012 Lund University. All rights reserved.
//

#include "bvh.h"
#include "tracer.h"
#include "stackaccelerator.h"
#include "laineaccelerator.h"
#include "sparseaccelerator.h"
#include "hapalaaccelerator.h"
#include "implicitaaccelerator.h"
#include "implicitbaccelerator.h"
#include <iostream>
#include <cassert>
#include <fstream>
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

static double seconds() {
#ifdef _WIN32
	return GetTickCount64()*(1.0/1000.0);
#else
	struct timeval tv;
	gettimeofday(&tv, 0);
	return tv.tv_sec + tv.tv_usec*(1.0/1000000.0);
#endif
}

static RayAccelerator* createAccelerator(Bvh& bvh, const char* name) {
	std::cout << "Accelerator: " << name << "." << std::endl;
	
	if (strcmp(name, "stack") == 0)
		return new StackAccelerator(bvh);
	else if (strcmp(name, "sparse") == 0)
		return new SparseAccelerator(bvh);
	else if (strcmp(name, "implicita") == 0)
		return new ImplicitAAccelerator(bvh);
	else if (strcmp(name, "implicitb") == 0)
		return new ImplicitBAccelerator(bvh);
	else if (strcmp(name, "laine0") == 0)
		return new LaineAccelerator<0>(bvh);
	else if (strcmp(name, "laine1") == 0)
		return new LaineAccelerator<1>(bvh);
	else if (strcmp(name, "laine2") == 0)
		return new LaineAccelerator<2>(bvh);
	else if (strcmp(name, "laine4") == 0)
		return new LaineAccelerator<4>(bvh);
	else if (strcmp(name, "laine8") == 0)
		return new LaineAccelerator<8>(bvh);
	else if (strcmp(name, "hapala") == 0)
		return new HapalaAccelerator(bvh);

	std::cout << "ERROR: Invalid accelerator." << std::endl;
	std::cout << "Pick one of: stack, implicita, implicitb, sparse, hapala, laine0,1,2,4,8." << std::endl;
	
	exit(1);
	return 0;
}

/** file path helper */
bool findFullPath(const std::string& root, std::string& filePath)
{
	bool fileFound = false;
	const std::string resourcePath = root;

	filePath = resourcePath + filePath;
	for (unsigned int i = 0; i < 16; ++i)
	{
		std::ifstream file;
		file.open(filePath.c_str());
		if (file.is_open())
		{
			fileFound = true;
			break;
		}

		filePath = "../" + filePath;
	}

	return fileFound;

}

int main(int argc, const char* argv[]) {
	assert(sizeof(Triangle) == 64);
	assert(sizeof(Bvh::Node) == 64);
	assert(sizeof(Tracer::HitPoint) == 32);

	std::string rootStr = "data/";
	std::string filePath = "battlefield.obj";
	bool fileFound = findFullPath(rootStr, filePath);
	if (fileFound == false)
	{
		std::cout << "Mesh file not found! Exiting..." << std::endl;
	}

	std::cout << "Loading obj..." << std::flush;
	Mesh mesh(filePath.c_str(), false);
	std::cout << "done." << std::endl;
	
	std::cout << "Building bvh..." << std::flush;
	Bvh bvh(mesh);
	std::cout << "done." << std::endl;

	const int width = 800;
	const int height = 300;
	const int samplesRoot = 4;
	
	Camera camera;
	RayAccelerator* accelerator = createAccelerator(bvh, argc > 1 ? argv[1] : "");
	Tracer tracer(camera, *accelerator);

	camera.lookAt(float3(300.0f, 100.0f, 300.0f), float3(0.0f, 10.0f, 0.0f), float3(0.0f, 1.0f, 0.0f), 50.0f, 1.0f, 1000.0f, width, height);

	std::vector<Tracer::HitPoint, AlignedAllocator<Tracer::HitPoint, 32> > hitPoints(width*height*samplesRoot*samplesRoot);
	std::vector<float> image(width*height);
	
	Random rnd(31251);

	std::cout << "Primary rays: " << std::flush;
	
	double primStart = seconds();
	int currentHitPoint = 0;
	
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			for (int sy = 0; sy < samplesRoot; ++sy) {
				for (int sx = 0; sx < samplesRoot; ++sx) {
					float px = j + (sx + rnd.nextFloat()) * (1.0f / samplesRoot);
					float py = i + (sy + rnd.nextFloat()) * (1.0f / samplesRoot);
					
					hitPoints[currentHitPoint++] = tracer.samplePrimary(px, py);
				}
			}
		}
	}
	
	double primEnd = seconds();
	std::cout << ((width*height*samplesRoot*samplesRoot)/(primEnd - primStart)) << " rays/sec." << std::endl;
	
	std::cout << "AO rays: " << std::flush;
	double aoStart = seconds();

	int aoRays = 0;
	currentHitPoint = 0;
	
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			float c = 0.0f;
			
			for (int sy = 0; sy < samplesRoot; ++sy) {
				for (int sx = 0; sx < samplesRoot; ++sx) {
					Tracer::HitPoint h = hitPoints[currentHitPoint++];
					
					if (!h.hit) {
						c += 1.0f;
						continue;
					}
					
					aoRays += Tracer::aoSamplesRoot*Tracer::aoSamplesRoot;
					c += tracer.sampleAO(h);
				}
			}
			
			image[i*width + j] = c * (1.0f / (samplesRoot*samplesRoot));
		}
	}
	
	double aoEnd = seconds();
	std::cout << (aoRays/(aoEnd - aoStart)) << " rays/sec." << std::endl;
	
	std::cout << "Total time: " << ((primEnd - primStart) + (aoEnd - aoStart)) << " sec." << std::endl;
	
	std::cout << "Saving image.ppm..." << std::flush;
	
	FILE* f = fopen("image.ppm", "w");
	fprintf(f, "P3\n%d %d\n%d\n", width, height, 255);
	for (int i = 0; i < width*height; ++i) {
		int c = (int)(image[i]*255.0f + 0.5f);
		fprintf(f,"%d %d %d ", c, c, c);
	}
	fclose(f);
	
	std::cout << "done." << std::endl;
	delete accelerator;
	return 0;
}
