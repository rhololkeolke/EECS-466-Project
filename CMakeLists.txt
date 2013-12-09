cmake_minimum_required(VERSION 2.8.3)
project(eecs466_project)

find_package(PCL REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories(
${PCL_INCLUDE_DIRS}
${OpenGL_INCLUDE_DIRS}
${GLUT_INCLUDE_DIRS}
)

message("OpenGL_LIBRARIES: ${OpenGL_LIBRARIES}")
message("GLUT_LIBRARIES: ${GLUT_LIBRARIES}")

add_executable(glut_example src/glut_example.cpp)
target_link_libraries(glut_example ${PCL_LIBRARIES} ${OpenGL_LIBRARIES} ${GLUT_LIBRARIES} GLU)