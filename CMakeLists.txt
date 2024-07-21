cmake_minimum_required(VERSION 3.27.7)

project("SimplePlatformer")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})

add_compile_options(-Wall -Wextra -Wpedantic -Werror)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_options(${PROJECT_NAME} PRIVATE "-g3")
    add_compile_options(-Wno-cpp)
else()
    target_compile_definitions(${PROJECT_NAME} PRIVATE NDEBUG)
    target_compile_options(${PROJECT_NAME} PRIVATE "-O3")
endif()
find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)

target_link_libraries(${PROJECT_NAME}  PRIVATE vulkan glfw)
