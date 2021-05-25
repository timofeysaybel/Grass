cmake_minimum_required (VERSION 3.0)
project (grass)

find_package(OpenGL REQUIRED)

add_subdirectory (external)

cmake_policy(SET CMP0026 OLD)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/external/cmake-modules-fe2273")
include(MSVCMultipleProcessCompile)

if(INCLUDE_DISTRIB)
	add_subdirectory(distrib)
endif(INCLUDE_DISTRIB)

include_directories(
	external/glfw-3.1.2/include/
	external/glm-0.9.7.1/
	external/glew-1.13.0/include/
	.
)

set(ALL_LIBS
	${OPENGL_LIBRARY}
	glfw
	GLEW_1130
)

add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)

add_executable(main
	source/main.cpp

	source/Shader.cpp
	include/Shader.hpp
	external/stb_image.cpp
	external/stb_image.hpp
)

target_link_libraries(main
	${ALL_LIBS}
)
