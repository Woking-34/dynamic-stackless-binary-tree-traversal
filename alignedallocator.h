//
//  alignedallocator.h
//  tracer
//
//  Created by Rasmus Barringer on 2013-02-20.
//  Copyright (c) 2013 Lund University. All rights reserved.
//

#ifndef tracer_alignedallocator_h
#define tracer_alignedallocator_h

#include <new>
#include <cstdio>
#include <xmmintrin.h>

template <typename T, std::size_t Alignment>
class AlignedAllocator {
public:
	typedef T* pointer;
	typedef const T* const_pointer;
	typedef T& reference;
	typedef const T& const_reference;
	typedef T value_type;
	typedef std::size_t size_type;
	typedef ptrdiff_t difference_type;
	
	template <typename U>
	struct rebind {
		typedef AlignedAllocator<U, Alignment> other;
	};
	
	AlignedAllocator() {}
	
	AlignedAllocator(const AlignedAllocator&) {}
	
	template <typename U>
	AlignedAllocator(const AlignedAllocator<U, Alignment>&) {}
	
	std::size_t max_size() const {
		return (static_cast<std::size_t>(0) - static_cast<std::size_t>(1)) / sizeof(T);
	}
	
	T* address(T& r) const {
		return &r;
	}
	
	const T* address(const T& s) const {
		return &s;
	}
	
	void construct(T* const p, const T& t) const {
		void* const pv = static_cast<void*>(p);
		new (pv) T(t);
	}
	
	void destroy(T* const p) const {
		p->~T();
	}

	bool operator == (const AlignedAllocator& other) const {
		return true;
	}
	
	bool operator != (const AlignedAllocator& other) const {
		return !(*this == other);
	}

	T* allocate(const std::size_t n) const {
		if (n == 0) {
			return NULL;
		}

		if (n > max_size())
			throw std::bad_alloc();

		void* const pv = _mm_malloc(n * sizeof(T), Alignment);

		if (pv == NULL)
			throw std::bad_alloc();
		
		return static_cast<T *>(pv);
	}
	
	void deallocate(T * const p, const std::size_t n) const {
		_mm_free(p);
	}

	template <typename U>
	T * allocate(const std::size_t n, const U*) const {
		return allocate(n);
	}
};

#endif
