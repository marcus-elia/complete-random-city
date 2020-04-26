cmake_minimum_required(VERSION 3.8)
project(complete-random-city)

if (WIN32)
    set(FREEGLUT_INCLUDE_DIRS "C:/Program\ Files/Common\ Files/freeglut/include")
    set(FREEGLUT_LIBRARY_DIRS "C:/Program\ Files/Common\ Files/freeglut/lib")
endif (WIN32)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++14 -Wno-deprecated -Werror=return-type")

find_package (OpenGL REQUIRED)

if (APPLE)
    find_package(GLUT REQUIRED)
endif (APPLE)

if (WIN32)
    include_directories(${OPENGL_INCLUDE_DIR}  ${FREEGLUT_INCLUDE_DIRS})
    link_directories(${FREEGLUT_LIBRARY_DIRS})
elseif (APPLE)
    include_directories(${OPENGL_INCLUDE_DIR}  ${GLUT_INCLUDE_DIRS})
endif ()

file(GLOB SOURCE_FILES
        *.cpp
        *.h
        )

add_executable(graphics graphics.h graphics.cpp gameManager.h gameManager.cpp player.h player.cpp
        chunk.h chunk.cpp perlinNoiseGenerator.h perlinNoiseGenerator.cpp plot.cpp plot.h roadPlot.cpp
        roadPlot.h structs.h emptyPlot.cpp emptyPlot.h solid.cpp solid.h recPrism.cpp recPrism.h
        triPrism.h triPrism.cpp frustum.h frustum.cpp cylinder.h cylinder.cpp building.cpp building.h
        buildingPlot.cpp buildingPlot.h multiPlot.cpp multiPlot.h movable.cpp movable.h movableComponent.cpp
        movableComponent.h vehicle.cpp vehicle.h car.cpp car.h rightTrapezoid.cpp rightTrapezoid.h
        ellipsoid.cpp ellipsoid.h dirigible.cpp dirigible.h airport.cpp airport.h airplane.cpp airplane.h)

if (WIN32)
    target_link_libraries (graphics ${OPENGL_LIBRARIES} freeglut)
elseif (APPLE)
    target_link_libraries (graphics ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
endif ()