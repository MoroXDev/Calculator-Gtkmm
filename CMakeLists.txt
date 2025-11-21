cmake_minimum_required(VERSION 3.15)

find_package(PkgConfig)

pkg_check_modules(GTKMM gtkmm-4.0)

project(
  Calculator
  LANGUAGES CXX)

link_directories(
    ${GTKMM_LIBRARY_DIRS}  )

include_directories(
    ${GTKMM_INCLUDE_DIRS}  )

add_executable(${CMAKE_PROJECT_NAME} main.cpp)

target_link_libraries(${CMAKE_PROJECT_NAME} 
    ${GTKMM_LIBRARIES}  )