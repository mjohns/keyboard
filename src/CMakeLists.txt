cmake_minimum_required(VERSION 3.1)

# set the project name
project(keyboard)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(glm)
add_subdirectory(util)

foreach (k example gamepad_v1)
  add_executable(${k} ${k}.cc)
  target_link_libraries(${k} PUBLIC glm_static)
  target_link_libraries(${k} PUBLIC util)
  target_include_directories(${k} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
  target_include_directories(${k} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/util)
endforeach()
