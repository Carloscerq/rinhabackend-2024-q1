cmake_minimum_required(VERSION 3.12)
set(PROJECT_NAME rinhabackend)
project(${PROJECT_NAME})

file(GLOB SOURCE_FILES "src/*.c" "src/*.h")
add_executable("${PROJECT_NAME}.out" ${SOURCE_FILES})
