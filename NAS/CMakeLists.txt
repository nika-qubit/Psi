# CMakeList.txt : CMake project for NAS, include source and define
# project specific logic here.
#

# Create the subproject library
add_library(
  nas_lib
  NAS.h
  fs_nas.cpp fs_nas.h
  labelled_fstab_nas.cpp labelled_fstab_nas.h
)

target_link_libraries(
  nas_lib
  absl::strings
  absl::log
)

# Set include directories for the subproject
target_include_directories(nas_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# TODO: Add tests and install targets if needed.
