cmake_minimum_required(VERSION 2.6)
project(intel-stackless-bvh)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/stackless-bvh-bin)

IF(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
ENDIF()

string(TOUPPER ${CMAKE_CXX_COMPILER_ID} CXX_COMPILER_ID_UPPER)
if(${CXX_COMPILER_ID_UPPER} STREQUAL MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj /MP")
endif()

IF(WIN32)
	add_definitions(-DWIN32_LEAN_AND_MEAN)
	add_definitions(-DNOMINMAX)
	add_definitions(-D_USE_MATH_DEFINES)
ENDIF(WIN32)

set(STACKLESSBVH_SRC
	aabb.h
	alignedallocator.h
	bvh.h
	bvh.cpp
	camera.h
	camera.cpp
	hapalaaccelerator.h
	hapalaaccelerator.cpp
	implicitaaccelerator.h
	implicitaaccelerator.cpp
	implicitbaccelerator.h
	implicitbaccelerator.cpp
	laineaccelerator.h
	mesh.h	
	mesh.cpp
	random.h
	ray.h
	rayaccelerator.h
	sparseaccelerator.h
	sparseaccelerator.cpp
	stackaccelerator.h
	stackaccelerator.cpp
	tracer.h
	tracer.cpp
	triangle.h
	vectormath.h
	
	main.cpp
)

add_executable(stackless_bvh ${STACKLESSBVH_SRC})