cmake_minimum_required(VERSION 3.5)

project(CsvReader LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file (GLOB_RECURSE SOURCES "*.cpp")
file (GLOB_RECURSE HEADERS "*.h")
file (GLOB_RECURSE UI "*.ui")
file (GLOB_RECURSE RESOURCE "*.qrc" "*.rc")

add_executable(${PROJECT_NAME} ${WIN} ${HEADERS} ${SOURCES} ${UI} ${RESOURCE})
