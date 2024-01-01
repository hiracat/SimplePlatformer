cmake_minimum_required(VERSION 3.27.7)

project("SimplePlatformer")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})

if(CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_options(${PROJECT_NAME} PRIVATE "-g")
else()
    target_compile_definitions(${PROJECT_NAME} PRIVATE NDEBUG)
endif()
find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)

target_link_libraries(${PROJECT_NAME}  PRIVATE glfw vulkan)
