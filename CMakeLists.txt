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
	src/aabb.h
	src/alignedallocator.h
	src/bvh.h
	src/bvh.cpp
	src/camera.h
	src/camera.cpp
	src/hapalaaccelerator.h
	src/hapalaaccelerator.cpp
	src/implicitaaccelerator.h
	src/implicitaaccelerator.cpp
	src/implicitbaccelerator.h
	src/implicitbaccelerator.cpp
	src/laineaccelerator.h
	src/mesh.h	
	src/mesh.cpp
	src/random.h
	src/ray.h
	src/rayaccelerator.h
	src/sparseaccelerator.h
	src/sparseaccelerator.cpp
	src/stackaccelerator.h
	src/stackaccelerator.cpp
	src/tracer.h
	src/tracer.cpp
	src/triangle.h
	src/vectormath.h
	
	src/main.cpp
)

add_executable(stackless_bvh ${STACKLESSBVH_SRC})
target_include_directories(stackless_bvh PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)