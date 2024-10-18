include(FetchContent)

macro(LinkGLUT TARGET ACCESS)
    FetchContent_Declare(
        glut
        GIT_REPOSITORY https://github.com/freeglut/freeglut.git
        GIT_TAG v3.6.0
    )

    FetchContent_GetProperties(glut)

    if (NOT glut_POPULATED)
        FetchContent_Populate(glut)
    endif()

    target_include_directories(${TARGET} ${ACCESS} ${glut_SOURCE_DIR}/include)
    target_include_directories(${TARGET} ${ACCESS} ${glut_SOURCE_DIR})
endmacro()