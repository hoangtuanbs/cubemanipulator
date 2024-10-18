cmake_minimum_required(VERSION 3.11 FATAL_ERROR)
project(CubeManipulator CXX)

set(GLAD_OUT_DIR        "${CMAKE_CURRENT_BINARY_DIR}" CACHE STRING "Output directory" FORCE)
set(GLAD_PROFILE        "core" CACHE STRING "OpenGL profile" FORCE)
set(GLAD_API            "gl=3.3" CACHE STRING "API type/version pairs, like \"gl=3.2,gles=\", no version means latest" FORCE)
set(GLAD_GENERATOR      "c" CACHE STRING "Language to generate the binding for" FORCE)
set(GLAD_EXTENSIONS     "" CACHE STRING "Path to extensions file or comma separated list of extensions, if missing all extensions are included" FORCE)
set(GLAD_SPEC           "gl" CACHE STRING "Name of the spec" FORCE)
set(GLAD_ALL_EXTENSIONS OFF CACHE BOOL "Include all extensions instead of those specified by GLAD_EXTENSIONS" FORCE)
set(GLAD_NO_LOADER      OFF CACHE BOOL "No loader" FORCE)
set(GLAD_REPRODUCIBLE   OFF CACHE BOOL "Reproducible build" FORCE)

#===========================================================================================

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Set directory paths
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/source)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(sources ${SOURCE_DIR}/main.cpp
    ${SOURCE_DIR}/texture.cpp
    ${SOURCE_DIR}/shader.cpp
    ${SOURCE_DIR}/engine.cpp
    ${SOURCE_DIR}/model.cpp)

set(CMAKE_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
include_directories(${CMAKE_INCLUDE_DIR})

add_executable(cube-man ${sources} ${includes})

# Perform dependency linkage
include(${CMAKE_DIR}/LinkGLFW.cmake)
LinkGLFW(cube-man PRIVATE)

include(${CMAKE_DIR}/LinkGLAD.cmake)
LinkGLAD(cube-man PRIVATE)

include(${CMAKE_DIR}/LinkGLM.cmake)
LinkGLM(cube-man PRIVATE)

include(${CMAKE_DIR}/LinkGLUT.cmake)
LinkGLUT(cube-man PRIVATE)

include(${CMAKE_DIR}/LinkSTB.cmake)
LinkSTB(cube-man PRIVATE)

find_package(OpenGL REQUIRED)
if (OpenGL_FOUND)
    target_include_directories(cube-man PRIVATE ${OPENGL_INCLDUE_DIRS})
    target_link_libraries(cube-man PRIVATE ${OPENGL_LIBRARIES})
else()
    message(FATAL_ERROR "OpenGL was not found on the system")
endif()

find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    target_include_directories(cube-man PRIVATE ${GLEW_INCLDUE_DIRS})
    target_link_libraries(cube-man PRIVATE ${GLEW_LIBRARIES})
else()
    message(FATAL_ERROR "GLEW was not found on the system. Please install libglew-dev")
endif()

# Handle the optional libraries

# we don't want to copy if we're building in the source dir
if (NOT CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)

    # list of files for which we add a copy rule
    set(RESOURCE_DIRS resources)

    foreach(item IN LISTS RESOURCE_DIRS)
        message(STATUS ${item})

        file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/${item}" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
    endforeach()
endif()

# Enable C++17
set_target_properties(cube-man PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO)

# Set project folders
set_target_properties(cube-man PROPERTIES FOLDER ${PROJECT_NAME})