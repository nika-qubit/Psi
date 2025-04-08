# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Psi")

# Include sub-projects.
add_subdirectory (NAS)

add_executable(Psi main.cpp) 

# Import Abseil's CMake target.
include(FetchContent)
FetchContent_Declare(
  absl 
  GIT_REPOSITORY https://github.com/abseil/abseil-cpp.git
  GIT_TAG        20250127.1
)
FetchContent_MakeAvailable(absl)

target_link_libraries(Psi absl::strings nas_lib)

target_include_directories(Psi PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/NAS)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Psi PROPERTY CXX_STANDARD 20)
endif()

