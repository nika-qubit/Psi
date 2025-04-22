# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("Psi")

# Include sub-projects.
add_subdirectory (NAS)
add_subdirectory (third_party)

add_executable(Psi main.cpp) 

target_link_libraries(
  Psi
  nas_lib
  absl::strings
  absl::log
  absl::log_initialize
)

target_include_directories(Psi PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/NAS)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Psi PROPERTY CXX_STANDARD 20)
endif()

