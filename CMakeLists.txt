cmake_minimum_required( VERSION 3.16.1 )

## Load CMAKE configuration from environment variables
set( CMAKE_MODULE_PATH $ENV{CMAKE_MODULE_PATH} )
set( CMAKE_PREFIX_PATH $ENV{CMAKE_PREFIX_PATH} )
set( CMAKE_EXPORT_COMPILE_COMMANDS 1 )

set( BUILDNO 1 )

project( hape_c )


## Set the build type
if( NOT CMAKE_BUILD_TYPE )
  set( CMAKE_BUILD_TYPE "Debug" 
       CACHE STRING "Build configuration 'Release' or 'Debug'."
       FORCE )
endif()
#set( CMAKE_BUILD_TYPE "Release" )

## Install the executable to <install_directory>/bin
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
## Install libraries (if any) to <install_directory>/bin
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin )


if(MSVC)
    add_compile_options( /W4 )
else()
    add_compile_options( -Wall -Wextra -pedantic -Werror )
    #    add_link_options( -pg )
endif()


add_subdirectory( mem )
add_subdirectory( io )
add_subdirectory( cnt )
add_subdirectory( fmt )
add_subdirectory( err )
##add_subdirectory( storage )
add_subdirectory( con )
