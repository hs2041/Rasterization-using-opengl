# CMake entry point
cmake_minimum_required (VERSION 3.0)
project (Assignment2)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-pthread")
# find_package(OpenGL REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED COMPONENTS OpenGL)
find_package(Boost REQUIRED system)
find_package(GLEW REQUIRED)
# include_directories(${PROJECT_SOURCE_DIR})
include_directories(
	${EIGEN3_INCLUDE_DIRS}
	${Boost_INCLUDE_DIRS}
	.
	include/
	include_mesh/
)

file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB SRC_M_FILES ${PROJECT_SOURCE_DIR}/src_mesh/*.cpp)
add_executable(assign3 ${SRC_FILES}
						)

target_link_libraries(assign3 glfw 
		${BOOST_LIBRARIES} 
		${OPENGL_LIBRARIES} 
		${GLEW_LIBRARIES}
		${GLUT_LIBRARY}
		${EIGEN3_}
		)

add_executable(create_mesh ${SRC_M_FILES})