cmake_minimum_required(VERSION 3.12)
set(PROJECT_NAME rinhabackend)
project(${PROJECT_NAME})


file(GLOB SOURCE_FILES "src/*.c" "src/*.h")
add_executable("${PROJECT_NAME}.out" ${SOURCE_FILES})


find_package(PostgreSQL REQUIRED)
include_directories(${PostgreSQL_INCLUDE_DIRS})

find_package(cJSON REQUIRED)
include_directories(${CJSON_INCLUDE_DIRS})

target_link_libraries("${PROJECT_NAME}.out" ${PostgreSQL_LIBRARIES} ${CJSON_LIBRARIES})
