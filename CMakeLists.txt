cmake_minimum_required(VERSION 3.27.7)

project("SimplePlatformer")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()


add_executable(${PROJECT_NAME} "src/main.cpp")

if(CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG_MODE)
endif()

target_link_libraries(${PROJECT_NAME}  PRIVATE glfw vulkan)
