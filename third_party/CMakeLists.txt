# Import Abseil's CMake target.
include(FetchContent)
FetchContent_Declare(
  absl 
  GIT_REPOSITORY https://github.com/abseil/abseil-cpp.git
  GIT_TAG        20250127.1
)
FetchContent_MakeAvailable(absl)
