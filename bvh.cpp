//
//  bvh.cpp
//  tracer
//
//  Created by Rasmus Barringer on 2012-10-06.
//  Copyright (c) 2012 Lund University. All rights reserved.
//

#include "bvh.h"
#include <algorithm>

struct Predicate {
	unsigned axis;
	
	bool operator () (const Triangle& i, const Triangle& j) const {
		return i.center()[axis] < j.center()[axis];
	}
};

Bvh::Bvh(Mesh& mesh) : mesh(mesh) {
	depth = 0;

	Node root = { 0, 0xffffffff };
	nodes.push_back(root);
	
	std::vector<Triangle, AlignedAllocator<Triangle, 64> >& triangles = mesh.getTriangles();
	
	Aabb bounds;
	bounds.reset();

	for (unsigned i = 0; i < (unsigned)triangles.size(); ++i)
		bounds.include(triangles[i]);
	
	build(bounds, 0, 0, (unsigned)triangles.size(), &triangles[0], 0);
}

unsigned Bvh::getDepth() const {
	return depth;
}

const Bvh::Node* Bvh::getNodes() const {
	return &nodes[0];
}

const Triangle* Bvh::getTriangles() const {
	return &mesh.getTriangles()[0];
}

void Bvh::makeImplicit() {
	std::vector<Bvh::Node, AlignedAllocator<Bvh::Node, 64> > implicitNodes;
	implicitNodes.resize((1 << (depth+1)) - 1);
	
	buildImplicit(0, &implicitNodes[0], 0, 0);
	std::swap(nodes, implicitNodes);
}

void Bvh::buildImplicit(int node, Bvh::Node* implicitNodes, int depth, int levelIndex) {
	Node n = nodes[node];
	
	n.parent = (1 << (depth-1)) - 1 + (levelIndex >> 1);
	
	if (n.kind) {
		n.first = (1 << (depth+1)) - 1 + (levelIndex << 1);
		n.last = (1 << (depth+1)) + (levelIndex << 1);
	}
	
	implicitNodes[(1 << depth) - 1 + levelIndex] = n;
	
	if (n.kind) {
		buildImplicit(nodes[node].first, implicitNodes, depth+1, levelIndex << 1);
		buildImplicit(nodes[node].last, implicitNodes, depth+1, (levelIndex << 1) + 1);
	}
}

void Bvh::build(const Aabb& bounds, unsigned node, unsigned first, unsigned last, Triangle* triangles, unsigned depth) {
	if (this->depth < depth)
		this->depth = depth;
	
	if (last - first < 8) {
		nodes[node].first = first;
		nodes[node].last = last;
		return;
	}

	unsigned pivot;
	
	if (depth > 8)
		pivot = medianPartition(bounds, first, last, triangles, depth, nodes[node].kind);
	else
		pivot = sahPartition(bounds, first, last, triangles, depth, nodes[node].kind);
	
	if (pivot == 0xffffffff) {
		nodes[node].first = first;
		nodes[node].last = last;
		return;
	}
	
	unsigned left = (unsigned)nodes.size();
	unsigned right = (unsigned)nodes.size()+1;
	
	Aabb leftBounds, rightBounds;
	leftBounds.reset();
	rightBounds.reset();
	
	for (unsigned i = first; i < pivot; ++i)
		leftBounds.include(triangles[i]);
	
	for (unsigned i = pivot; i < last; ++i)
		rightBounds.include(triangles[i]);
	
	Node child = { 0, node };
	nodes.push_back(child);
	nodes.push_back(child);
	
	nodes[node].first = left;
	nodes[node].last = right;
	nodes[node].left = leftBounds;
	nodes[node].right = rightBounds;
	
	build(leftBounds, left, first, pivot, triangles, depth+1);
	build(rightBounds, right, pivot, last, triangles, depth+1);
}

unsigned Bvh::medianPartition(const Aabb& bounds, unsigned first, unsigned last, Triangle* triangles, unsigned depth, unsigned& kind) {
	unsigned pivot = (first + last) / 2;
	
	unsigned bestDim = bounds.largestAxis();
	float bestSeparation = -1e+10f;
	
	for (unsigned dim = 0; dim < 3; ++dim) {
		Predicate predicate = { dim };
		std::sort(triangles + first, triangles + last, predicate);
		
		Aabb left, right;
		left.reset();
		right.reset();

		for (unsigned i = first; i < pivot; ++i)
			left.include(triangles[i]);
		
		for (unsigned i = pivot; i < last; ++i)
			right.include(triangles[i]);
		
		float separation = right.center()[dim] - left.center()[dim];
		
		if (separation > bestSeparation) {
			bestDim = dim;
			bestSeparation = separation;
		}
	}
	
	kind = bestDim + 1;
	Predicate predicate = { bestDim };
	std::sort(triangles + first, triangles + last, predicate);
	return pivot;
}

unsigned Bvh::sahPartition(const Aabb& parentBounds, unsigned first, unsigned last, Triangle* triangles, unsigned depth, unsigned& kind) {
	const float traversalCost = 2.0f;
	const float intersectionCost = 1.0f;
	
	static std::vector<float> accumulatedArea;
	
	float divPsa = 1.0f / parentBounds.surfaceArea();
	
	float bestSah = 1e+10f;
	unsigned bestDim = 0xffffffff;
	unsigned bestSplit = 0xffffffff;
	
	for (unsigned dim = 0; dim < 3; ++dim) {
		Predicate predicate = { dim };
		std::sort(triangles + first, triangles + last, predicate);
		
		Aabb bounds;
		bounds.reset();
		accumulatedArea.resize(0);
		
		for (unsigned i = first; i < last-1; ++i) {
			bounds.include(triangles[i]);
			accumulatedArea.push_back(bounds.surfaceArea());
		}
		
		bounds.reset();
		
		for (int i = (int)last - 1; i > (int)first; --i) {
			bounds.include(triangles[i]);
			
			float lsa = accumulatedArea[i-first - 1];
			float rsa = bounds.surfaceArea();
			
			float sah = traversalCost + intersectionCost * divPsa * // Constant cost.
			(lsa * ((float)(i - first)) +	// Left child.
			 rsa * ((float)(last - i)));	// Right child.
			
			if (sah < bestSah) {
				bestSah = sah;
				bestSplit = i;
				bestDim = dim;
			}
		}
	}
	
	if (bestSah > (float)(last-first) * intersectionCost)
		return 0xffffffff;
	
	kind = bestDim + 1;
	Predicate predicate = { bestDim };
	std::sort(triangles + first, triangles + last, predicate);
	return bestSplit;
}
