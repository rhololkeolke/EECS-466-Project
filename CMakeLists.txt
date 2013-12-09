cmake_minimum_required(VERSION 2.8.3)
project(eecs466_project)

set(CMAKE_BUILD_TYPE Debug)

find_package(PCL REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(CGAL COMPONENTS Core)

include(${CGAL_USE_FILE})
include(CGAL_CreateSingleSourceCGALProgram)

include_directories(
${PCL_INCLUDE_DIRS}
${OpenGL_INCLUDE_DIRS}
${GLUT_INCLUDE_DIRS}
)

set(LIBS ${PCL_LIBRARIES} ${OpenGL_LIBRARIES} ${GLUT_LIBRARIES} ${CGAL_LIBRARIES} GLU m )

if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-std=gnu++0x)
endif()

add_executable(glut_example src/glut_example.cpp)
target_link_libraries(glut_example ${LIBS})

include_directories(src/voronoi_diagram)

add_library(voronoi_diagram src/voronoi_diagram/beachline_node.cpp src/voronoi_diagram/voronoi_diagram.cpp src/voronoi_diagram/animated_voronoi_diagram.cpp)
target_link_libraries(voronoi_diagram ${LIBS})

add_executable(animated_voronoi_diagram_example src/voronoi_diagram/main.cpp)
target_link_libraries(animated_voronoi_diagram_example ${LIBS} voronoi_diagram)

add_executable(voronoi_and_delaunay src/voronoi_and_delaunay.cpp)
target_link_libraries(voronoi_and_delaunay ${LIBS} voronoi_diagram)

add_executable(terrain_mesh_reconstruction src/terrain_mesh_reconstruction.cpp)
target_link_libraries(terrain_mesh_reconstruction ${LIBS} voronoi_diagram)

# CGAL

add_executable(array_convex_hull_2 src/cgal_examples/array_convex_hull_2.cpp)
target_link_libraries(array_convex_hull_2 ${LIBS})