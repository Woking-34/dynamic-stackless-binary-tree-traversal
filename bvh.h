//
//  bvh.h
//  tracer
//
//  Created by Rasmus Barringer on 2012-10-06.
//  Copyright (c) 2012 Lund University. All rights reserved.
//

#ifndef tracer_bvh_h
#define tracer_bvh_h

#include "mesh.h"
#include "aabb.h"
#include "alignedallocator.h"

class Bvh {
public:
	struct Node {
		unsigned kind, parent;
		unsigned first, last;
		Aabb left, right;
	};
	
private:
	unsigned depth;
	Mesh& mesh;
	std::vector<Node, AlignedAllocator<Node, 64> > nodes;
	
public:
	Bvh(Mesh& mesh);
	
	unsigned getDepth() const;
	
	const Node* getNodes() const;
	
	const Triangle* getTriangles() const;
	
	void makeImplicit();
	
private:
	void buildImplicit(int node, Bvh::Node* implicitNodes, int depth, int levelIndex);
	
	void build(const Aabb& bounds, unsigned node, unsigned first, unsigned last, Triangle* triangles, unsigned depth);
	
	unsigned medianPartition(const Aabb& bounds, unsigned first, unsigned last, Triangle* triangles, unsigned depth, unsigned& kind);
	
	unsigned sahPartition(const Aabb& parentBounds, unsigned first, unsigned last, Triangle* triangles, unsigned depth, unsigned& kind);
};

#endif
