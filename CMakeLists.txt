cmake_minimum_required(VERSION 2.8.3)
project(eecs466_project)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(PCL 1.7 REQUIRED COMPONENTS common io kdtree features surface search)
find_package(CGAL COMPONENTS Core)
find_package(GLEW REQUIRED)

include(${CGAL_USE_FILE})
include(CGAL_CreateSingleSourceCGALProgram)

include_directories(
${PCL_INCLUDE_DIRS}
${OpenGL_INCLUDE_DIRS}
${GLUT_INCLUDE_DIRS}
${GLEW_INCLUDE_DIR}
)

link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})


set(PCL_LIBRARIES ${PCL_COMMON_LIBRARIES} ${PCL_IO_LIBRARIES} ${PCL_KDTREE_LIBRARIES} ${PCL_FEATURES_LIBRARIES} ${PCL_SURFACE_LIBRARIES} ${PCL_SEARCH_LIBRARIES})

set(LIBS ${PCL_LIBRARIES} ${OpenGL_LIBRARIES} ${GLUT_LIBRARIES} GLU ${GLEW_LIBRARY} ${CGAL_LIBRARIES} m)

if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-std=gnu++0x)
endif()

add_executable(glut_example src/glut_example.cpp)
target_link_libraries(glut_example ${LIBS})

include_directories(src/voronoi_diagram)
include_directories(libraries/tinyobjloader)

add_library(tinyobjloader libraries/tinyobjloader/tiny_obj_loader.cc)

###########################
# My Voronoi and Delaunay #
###########################

add_library(voronoi_diagram src/voronoi_diagram/beachline_node.cpp src/voronoi_diagram/voronoi_diagram.cpp src/voronoi_diagram/animated_voronoi_diagram.cpp)
target_link_libraries(voronoi_diagram ${LIBS})

add_executable(animated_voronoi_diagram_example src/voronoi_diagram/main.cpp)
target_link_libraries(animated_voronoi_diagram_example ${LIBS} voronoi_diagram)

add_executable(voronoi_and_delaunay src/voronoi_diagram/voronoi_and_delaunay.cpp)
target_link_libraries(voronoi_and_delaunay ${LIBS} voronoi_diagram)

add_executable(voronoi_and_delaunay_surface_reconstruction src/voronoi_diagram/voronoi_and_delaunay_surface_reconstruction.cpp)
target_link_libraries(voronoi_and_delaunay_surface_reconstruction ${LIBS} voronoi_diagram)

##########################
# Mesh Sampling Programs #
##########################

add_library(mesh_sampler_utils src/mesh_samplers/mesh_sampler_utils.cpp)
target_link_libraries(mesh_sampler_utils tinyobjloader)

add_executable(mesh_surface_sampler src/mesh_samplers/mesh_surface_sampler.cpp)
target_link_libraries(mesh_surface_sampler mesh_sampler_utils)

add_executable(mesh_triangle_sampler src/mesh_samplers/mesh_triangle_sampler.cpp)
target_link_libraries(mesh_triangle_sampler mesh_sampler_utils)

add_executable(mesh_sample_viewer src/mesh_samplers/mesh_sample_viewer.cpp)
target_link_libraries(mesh_sample_viewer ${LIBS} mesh_sampler_utils)

##############################
# PCL Surface Reconstruction #
##############################

add_executable(mesh_samples_to_pcd src/pcl_reconstruction/mesh_samples_to_pcd)
target_link_libraries(mesh_samples_to_pcd ${PCL_LIBRARIES} mesh_sampler_utils)

add_executable(pcl_greedy_triangulation src/pcl_reconstruction/pcl_greedy_triangulation.cpp)
target_link_libraries(pcl_greedy_triangulation ${PCL_LIBRARIES})

add_executable(pcl_marching_cubes src/pcl_reconstruction/pcl_marching_cubes.cpp)
target_link_libraries(pcl_marching_cubes ${PCL_LIBRARIES})

add_executable(pcl_poisson src/pcl_reconstruction/pcl_poisson.cpp)
target_link_libraries(pcl_poisson ${PCL_LIBRARIES})

########################
# CGAL Implementations #
########################

add_executable(array_convex_hull_2 src/cgal/array_convex_hull_2.cpp)
target_link_libraries(array_convex_hull_2 ${LIBS})

add_executable(delaunay_surface src/cgal/delaunay_surface.cpp)
target_link_libraries(delaunay_surface ${LIBS})


####################
# Mesh Comparisons #
####################

add_executable(hausdorf_mesh_vertices src/mesh_comparison/hausdorf_mesh_vertices.cpp)
target_link_libraries(hausdorf_mesh_vertices tinyobjloader)

###################
# Powercrust Code #
###################

add_subdirectory(libraries/powercrust powercrust)