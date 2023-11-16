cmake_minimum_required(VERSION 3.27.7)

project("SimplePlatformer")
add_executable(${PROJECT_NAME} "src/main.cpp")
target_link_libraries(${PROJECT_NAME} -lglfw -lvulkan)
