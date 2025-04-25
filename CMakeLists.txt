# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.12)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

project ("Psi")

# Include sub-projects.
add_subdirectory (NAS)
add_subdirectory (exif)
add_subdirectory (third_party)

add_executable(Psi main.cpp) 

target_link_libraries(
  Psi
  nas_lib
  exif_lib
  absl::strings
  absl::log
  absl::log_initialize
  re2
  Exiv2::exiv2lib
  nlohmann_json::nlohmann_json
)

target_include_directories(Psi
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/NAS
  ${CMAKE_CURRENT_SOURCE_DIR}/exif
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Psi PROPERTY CXX_STANDARD 20)
endif()

