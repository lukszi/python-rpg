cmake_minimum_required(VERSION 2.8.12)
project(python_rpg)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Import settings from original RPG project
set(CMAKE_C_FLAGS "-Wall ${CMAKE_C_FLAGS}")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -gdwarf-2 -g3 -DDEBUG -O0")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} -O2 -DNDEBUG")

# Add position independent code flag - needed for shared library
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Ensure gitversion.h is created before building
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/src)
execute_process(
    COMMAND ${CMAKE_COMMAND} 
        -Dlocal_dir=${CMAKE_SOURCE_DIR}
        -Doutput_dir=${CMAKE_BINARY_DIR}/src
        -P ${CMAKE_SOURCE_DIR}/cmake/gitversion.cmake
)

# Build the RPG source as a shared library
set(RPG_LIB_SRCS
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/M2optPoly.c
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/M2optPolyII.c
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/MCPolygon.c
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/MbouncePoly.c
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/MgrowingPoly.c
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/MrejectPoly.c
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/MsearchPoly.c
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/MspPoly.c
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/MstarPoly.c
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/MtriangPoly.c
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/MxmonoPoly.c
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/analysis.c
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/analysisII.c
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/argEval.c
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/basicAVL.c
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/basicDoubleArray.c
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/basicGeom.c
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/basicHeap.c
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/basicInits.c
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/basicIntArray.c
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/basicIo.c
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/basicLine.c
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/basicPoly.c
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/basicPolyLine.c
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/basicStack.c
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/basicVoidArray.c
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/binArray.c
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/calcPoly.c
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/convexHull.c
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/edgeIncArray.c
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/elapsed.c
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/eralloc.c
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/error.c
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/forcedEdge.c
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/geom.c
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/geom2.c
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/geomHash.c
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/global.c
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/incConvexHull.c
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/indexAVL.c
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/intIncArray.c
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/io.c
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/ipeWrite.c
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/isectArray.c
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/lineHeap.c
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/lineInfo.c
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/linkPoly.c
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/mem.c
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/merge.c
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/nodeList.c
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/outWrite.c
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/pathTree.c
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/pointArray.c
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/pointOps.c
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/pointIncArray.c
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/pointList.c
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/poly.c
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/polyOps.c
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/poly2.c
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/polyAVL.c
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/polyArray.c
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/polyIncArray.c
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/qhull.c
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/quadEdge.c
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/set.c
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/stateLine.c
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/smoothPoly.c
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/sortPoly.c
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/starKernel.c
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/starPoly.c
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/stat.c
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/sweepArrange.c
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/triangAVL.c
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/triangArray.c
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/uLongArray.c
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/user.c
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src/voidIncArray.c
)

# Create shared library
add_library(rpg SHARED ${RPG_LIB_SRCS})
target_include_directories(rpg PUBLIC 
    ${CMAKE_SOURCE_DIR}/genpoly-rpg/src
    ${CMAKE_BINARY_DIR}/src
)

# Link with math library
find_library(M_LIB m)
if(M_LIB)
    target_link_libraries(rpg ${M_LIB})
endif()