cmake_minimum_required(VERSION 3.10)
project(Example_06_05)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_STATIC_RUNTIME ON)

find_package(Boost REQUIRED COMPONENTS system filesystem)
include_directories(${Boost_INCLUDE_DIRS})

add_library(common_lib STATIC library.cpp)

add_library(shared_v1 SHARED lib_v1.cpp)
set_target_properties(shared_v1 PROPERTIES
        LIBRARY_OUTPUT_NAME "shared_v1"
        PREFIX "lib"
        SUFFIX ".so"
)

add_library(shared_v2 SHARED lib_v2.cpp)
set_target_properties(shared_v2 PROPERTIES
        LIBRARY_OUTPUT_NAME "shared_v2"
        PREFIX "lib"
        SUFFIX ".so"
)

add_executable(06.05 06.05.cpp)
target_link_libraries(06.05 common_lib ${Boost_LIBRARIES} dl)

add_dependencies(06.05 shared_v1 shared_v2)

add_custom_command(TARGET shared_v1 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:shared_v1> ${CMAKE_BINARY_DIR}/libshared_v1.so
)

add_custom_command(TARGET shared_v2 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:shared_v2> ${CMAKE_BINARY_DIR}/libshared_v2.so
)
