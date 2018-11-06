cmake_minimum_required(VERSION 3.6)
project(realtimeRenderingOfFur)

set(CMAKE_CXX_STANDARD 11)

## If we want some cmake modules
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

## Set name of folders that will be in use
set(PROJECT_BUILD_DIR ${PROJECT_SOURCE_DIR}/build)
set(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(PROJECT_EXTERNAL_DIR ${PROJECT_SOURCE_DIR}/external)
set(PROJECT_SOURCES_DIR ${PROJECT_SOURCE_DIR}/src)
set(PROJECT_LIB_DIR ${PROJECT_SOURCE_DIR}/lib)


# 32bit or 64bit
if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    MESSAGE( "64 bits compiler detected")
    SET(EX_PLATFORM 64)
    SET(EX_PLATFORM_NAME "x64")
        if(MINGW)
            set(GLEW_LIB_DIR ${PROJECT_EXTERNAL_DIR}/glew-2.0.0/lib/Release/x64)
        endif(MINGW)
else (CMAKE_SIZEOF_VOID_P EQUAL 8)
    MESSAGE( "32 bits compiler detected")
    SET(EX_PLATFORM 32)
    SET(EX_PLATFORM_NAME "x86")
    if(MINGW)
        set(GLEW_LIB_DIR ${PROJECT_EXTERNAL_DIR}/glew-2.0.0/lib/Release/Win32)
    endif(MINGW)
endif (CMAKE_SIZEOF_VOID_P EQUAL 8)

#######################################################################

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -DGL_GLEXT_PROTOTYPES")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE} -O2")
#set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -lGLEW -lGL -lX11 -lpthread -lXrandr -lXi")

if (APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGLFW_INCLUDE_GLCOREARB")
endif (APPLE)

## Define if in debug mode or not (comment out line below to disable logging -> faster)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMY_DEBUG")
if (CMAKE_BUILD_TYPE EQUAL "DEBUG")
    message(WARNING "Debug mode")
elseif (CMAKE_BUILD_TYPE EQUAL "DEBUG")
    message(WARNING "Release mode")
endif (CMAKE_BUILD_TYPE EQUAL "DEBUG")


##########################################
#######     Set Libraries     ############
##########################################


### OPENGL ###
find_package(OpenGL REQUIRED)
set(ALL_LIBRARIES ${ALL_LIBRARIES} ${OPENGL_LIBRARIES})
set(ALL_LIBRARIES ${ALL_LIBRARIES} ${OPENGL_glu_LIBRARY})

### GLEW on Mac
if(APPLE)
    find_package(GLEW REQUIRED)
    set(GLEW_INCLUDE_DIR ${PROJECT_INCLUDE_DIR}/GL)
    set(EXTERNAL_INCLUDE_DIRS ${EXTERNAL_INCLUDE_DIRS} ${GLEW_INCLUDE_DIR})
    set(ALL_LIBRARIES ${ALL_LIBRARIES} ${GLEW_LIBRARIES})
endif(APPLE)

### GLEW on Windows
if(MINGW)
    set(GLEW_INCLUDE_DIR ${PROJECT_EXTERNAL_DIR}/glew-2.0.0/include)
    set(GLEW_LIBRARIES ${GLEW_LIB_DIR}/glew32s.lib )
    set(EXTERNAL_INCLUDE_DIRS ${EXTERNAL_INCLUDE_DIRS} ${GLEW_INCLUDE_DIR})
    set(ALL_LIBRARIES ${ALL_LIBRARIES} ${GLEW_LIBRARIES})
endif (MINGW)


### GLFW ###
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(${PROJECT_EXTERNAL_DIR}/glfw-3.2.1/)
set(ALL_LIBRARIES ${ALL_LIBRARIES} glfw)

### GLM ###
set(EXTERNAL_INCLUDE_DIRS ${EXTERNAL_INCLUDE_DIRS} ${PROJECT_EXTERNAL_DIR}/glm)


####################################################
###### Include directories, add executables ########
######          and link libraries          ########
####################################################

# Lump all external- and project includes into one variable
set(ALL_INCLUDES ${EXTERNAL_INCLUDE_DIRS} ${PROJECT_INCLUDE_DIR} ${CMAKE_MODULE_PATH})

# Set the include directories so that #include statements work
include_directories(${ALL_INCLUDES})
message( "All include dirs: ${ALL_INCLUDES}")

# Get all source files by traversing the source directory recursively
file(GLOB_RECURSE PROJECT_CPP_FILES ${PROJECT_SOURCES_DIR}/*.cpp)

# Adds executable files
set(SOURCE_FILES main.cpp ${PROJECT_CPP_FILES} include/ShaderProgram.hpp include/FileReader.hpp include/Camera.h include/MeshObject.h src/MeshObject.cpp include/Texture.h src/Texture.cpp)
add_executable(realtimeRenderingOfFur ${SOURCE_FILES})

# Links libraries
target_link_libraries(realtimeRenderingOfFur ${ALL_LIBRARIES})
message("All include libraries: ${ALL_LIBRARIES}")


