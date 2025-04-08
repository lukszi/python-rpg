cmake_minimum_required(VERSION 3.15)
project(genpoly_rpg LANGUAGES C)

# Find Python and NumPy
find_package(Python REQUIRED COMPONENTS Interpreter Development.Module NumPy)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)

# C flags
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "-Wall ${CMAKE_C_FLAGS}")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -gdwarf-2 -g3 -DDEBUG -O0")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} -O2 -DNDEBUG")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Important RPATH settings for the extension module
if(UNIX)
    set(CMAKE_INSTALL_RPATH "$ORIGIN")
    set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
endif()

# Create directory for gitversion.h
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/src)
execute_process(
    COMMAND ${CMAKE_COMMAND} 
        -Dlocal_dir=${CMAKE_CURRENT_SOURCE_DIR}
        -Doutput_dir=${CMAKE_CURRENT_BINARY_DIR}/src
        -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/gitversion.cmake
)

# Define RPG library sources - list all the source files from the RPG project
file(GLOB RPG_LIB_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/genpoly-rpg/src/*.c)

# Create RPG shared library
add_library(rpg SHARED ${RPG_LIB_SRCS})
target_include_directories(rpg PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/genpoly-rpg/src
    ${CMAKE_CURRENT_BINARY_DIR}/src
)

# Link with math library
find_library(M_LIB m)
if(M_LIB)
    target_link_libraries(rpg ${M_LIB})
endif()

# Create Python extension module
Python_add_library(_genpoly_rpg MODULE genpoly_rpg.c WITH_SOABI)
target_link_libraries(_genpoly_rpg PRIVATE rpg)
target_include_directories(_genpoly_rpg PRIVATE
    ${Python_NumPy_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/genpoly-rpg/src
    ${CMAKE_CURRENT_BINARY_DIR}/src
)

# Install the extension module to Python's package directory
install(TARGETS _genpoly_rpg DESTINATION genpoly_rpg)

# Install the shared library alongside the extension
if(UNIX)
    install(TARGETS rpg DESTINATION genpoly_rpg)
endif()
