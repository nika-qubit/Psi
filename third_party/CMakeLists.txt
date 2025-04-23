# Import Abseil's CMake target.
include(FetchContent)
FetchContent_Declare(
  absl 
  GIT_REPOSITORY https://github.com/abseil/abseil-cpp.git
  GIT_TAG        20250127.1
)
FetchContent_Declare(
  re2
  GIT_REPOSITORY https://github.com/google/re2.git
  GIT_TAG        2024-07-02
)
set(ABSL_ENABLE_INSTALL ON)
FetchContent_MakeAvailable(
  absl
  re2
)
