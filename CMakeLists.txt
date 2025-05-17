cmake_minimum_required(VERSION 3.22)
project(
  OCLW
  DESCRIPTION "a class for more convenient use of the openсl library at the basic level"
  LANGUAGES C CXX
)

find_package(CUDAToolkit REQUIRED)
find_package(OpenCL REQUIRED)

add_library(${PROJECT_NAME} STATIC "${CMAKE_CURRENT_SOURCE_DIR}/src/oclw.cpp")
target_compile_definitions(${PROJECT_NAME} PRIVATE OCLW_EXPORTS=1)
target_link_libraries(${PROJECT_NAME} OpenCL::OpenCL)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)