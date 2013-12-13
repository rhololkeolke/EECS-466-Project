cmake_minimum_required(VERSION 2.8.3)
project(eecs466_project)

set(CMAKE_BUILD_TYPE Debug)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(PCL 1.7 REQUIRED COMPONENTS common io kdtree features surface search)

include_directories(
${PCL_INCLUDE_DIRS}
${OpenGL_INCLUDE_DIRS}
${GLUT_INCLUDE_DIRS}
)

link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})


set(PCL_LIBRARIES ${PCL_COMMON_LIBRARIES} ${PCL_IO_LIBRARIES} ${PCL_KDTREE_LIBRARIES} ${PCL_FEATURES_LIBRARIES} ${PCL_SURFACE_LIBRARIES} ${PCL_SEARCH_LIBRARIES})

set(LIBS ${PCL_LIBRARIES} ${OpenGL_LIBRARIES} ${GLUT_LIBRARIES} GLU m)

if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-std=gnu++0x)
endif()

add_executable(glut_example src/glut_example.cpp)
target_link_libraries(glut_example ${LIBS})

include_directories(src/voronoi_diagram)
include_directories(libraries/tinyobjloader)

add_library(tinyobjloader libraries/tinyobjloader/tiny_obj_loader.cc)

add_library(voronoi_diagram src/voronoi_diagram/beachline_node.cpp src/voronoi_diagram/voronoi_diagram.cpp src/voronoi_diagram/animated_voronoi_diagram.cpp)
target_link_libraries(voronoi_diagram ${LIBS})

add_library(mesh_sampler_utils src/mesh_sampler_utils.cpp)
target_link_libraries(mesh_sampler_utils tinyobjloader)

add_executable(mesh_surface_sampler src/mesh_surface_sampler.cpp)
target_link_libraries(mesh_surface_sampler mesh_sampler_utils)

add_executable(mesh_triangle_sampler src/mesh_triangle_sampler.cpp)
target_link_libraries(mesh_triangle_sampler mesh_sampler_utils)

add_executable(mesh_sample_viewer src/mesh_sample_viewer.cpp)
target_link_libraries(mesh_sample_viewer ${LIBS} mesh_sampler_utils)

add_executable(animated_voronoi_diagram_example src/voronoi_diagram/main.cpp)
target_link_libraries(animated_voronoi_diagram_example ${LIBS} voronoi_diagram)

add_executable(voronoi_and_delaunay src/voronoi_and_delaunay.cpp)
target_link_libraries(voronoi_and_delaunay ${LIBS} voronoi_diagram)

add_executable(terrain_mesh_reconstruction src/terrain_mesh_reconstruction.cpp)
target_link_libraries(terrain_mesh_reconstruction ${LIBS} voronoi_diagram)

add_executable(mesh_samples_to_pcd src/mesh_samples_to_pcd)
target_link_libraries(mesh_samples_to_pcd ${PCL_LIBRARIES} mesh_sampler_utils)

add_executable(pcl_greedy_triangulation src/pcl_greedy_triangulation.cpp)
target_link_libraries(pcl_greedy_triangulation ${PCL_LIBRARIES})

add_executable(pcl_marching_cubes src/pcl_marching_cubes.cpp)
target_link_libraries(pcl_marching_cubes ${PCL_LIBRARIES})