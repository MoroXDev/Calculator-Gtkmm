cmake_minimum_required(VERSION 3.15)

find_package(PkgConfig)

pkg_check_modules(GTKMM gtkmm-4.0)

project(
  Calculator
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

link_directories(
    ${GTKMM_LIBRARY_DIRS}  )

include_directories(
    ${GTKMM_INCLUDE_DIRS}  )

file(GLOB_RECURSE source src/*.cpp)

add_executable(${CMAKE_PROJECT_NAME} ${source})

target_link_libraries(${CMAKE_PROJECT_NAME} 
    ${GTKMM_LIBRARIES}  )

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)