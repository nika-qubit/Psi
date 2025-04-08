# CMakeList.txt : CMake project for NAS, include source and define
# project specific logic here.
#

# Create the subproject library
add_library(nas_lib NAS.cpp NAS.h)

# Set include directories for the subproject
target_include_directories(nas_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# TODO: Add tests and install targets if needed.
