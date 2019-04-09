# Dynamic Stackless Binary Tree Traversal

## Abstract
A fundamental part of many computer algorithms involves traversing a binary tree. One notable example is traversing a space-partitioning acceleration structure when computing ray-traced images. Traditionally, the traversal requires a stack to be temporarily stored for each ray, which results in both additional storage and memory bandwidth usage. We present a novel algorithm for traversing a binary tree that does not require a stack and, unlike previous approaches, works with dynamic descent direction without restarting. Our algorithm will visit exactly the same sequence of nodes as a stack-based counterpart with extremely low computational overhead. No additional memory accesses are made for implicit binary trees. For sparse trees, parent links are used to backtrack the shortest path. We evaluate our algorithm using a ray tracer with a bounding volume hierarchy for which source code is supplied.

<p align="center"><img src="data/battlefield.jpg" width="800" /></p>
<p align="center"><b>CHESS scene rendered with stackless binary tree traversal algorithm
</b></p>

## Authors
* Rasmus Barringer
* Tomas Akenine-Moller

## Changes
* CMake project & build support
* MSVC compatibility fixes

## References
 * [Intel - Dynamic Stackless Binary Tree Traversal](https://software.intel.com/en-us/articles/dynamic-stackless-binary-tree-traversal)
 * [JCGT - Dynamic Stackless Binary Tree Traversal](http://jcgt.org/published/0002/01/03/)
