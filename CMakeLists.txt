cmake_minimum_required(VERSION 3.1)

project(rhymer)

set(VERSION_MAJOR 1)
set(VERSION_MINOR 6)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(examples)
add_subdirectory(include)

install(FILES
        ${PROJECT_SOURCE_DIR}/include/rhymer.h
    DESTINATION
        include)

