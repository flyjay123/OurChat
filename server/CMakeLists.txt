cmake_minimum_required(VERSION 3.10)
project(server)

set(CMAKE_CXX_STANDARD 17)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} SRC_LIST)

file(GLOB_RECURSE HEADER_FILES *.h *.hpp *.ini)
add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SRC_LIST})
message(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/thirdparty/SQLiteCpp)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/thirdparty/json-develop)

# Assuming the libraries are installed in the standard system path
# If not, you should use target_link_directories() to add the directories containing these libraries
target_link_libraries(${PROJECT_NAME}
    PRIVATE pthread SQLiteCpp sqlite3 nlohmann_json
)
