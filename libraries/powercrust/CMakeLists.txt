cmake_minimum_required(VERSION 2.8.3)
project(powercrust)

add_custom_target(
  powercrust_package ALL
  COMMAND make
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(
  powercrust_binary ALL
  COMMAND mv ${CMAKE_CURRENT_SOURCE_DIR}/powercrust ${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS powercrust_package
)

add_custom_target(
  simplify_binary ALL
  COMMAND mv ${CMAKE_CURRENT_SOURCE_DIR}/simplify ${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS powercrust_package
)

add_custom_target(
  orient_binary ALL
  COMMAND mv ${CMAKE_CURRENT_SOURCE_DIR}/orient ${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS powercrust_package
)

add_custom_target(
  powercrust_clean ALL
  COMMAND make clean
  DEPENDS powercrust_binary simplify_binary orient_binary
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
