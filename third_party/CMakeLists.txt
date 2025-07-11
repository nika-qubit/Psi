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
FetchContent_Declare(
  exiv2
  GIT_REPOSITORY https://github.com/Exiv2/exiv2.git
  GIT_TAG        v0.28.5
)
FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG        v3.12.0
)
set(ABSL_ENABLE_INSTALL        ON)
FetchContent_MakeAvailable(
  absl
  re2
  exiv2
  json
)

# Exiv2 setup for Linux.
# sudo apt install --yes build-essential ccache clang cmake git google-mock libbrotli-dev libcurl4-openssl-dev libexpat1-dev libgtest-dev libinih-dev libssh-dev libxml2-utils libz-dev python3 zlib1g-dev
# Install it manually following:
# https://github.com/Exiv2/exiv2?tab=readme-ov-file#build-install-use-exiv2-on-a-unix-like-system.
# Below automated the process. But it has to be executed with sudo cmake and the first-time build will fail because source code is being generated after the build starts.
add_custom_target(PreInstallExiv2
  cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
  COMMAND cmake --build build
  COMMAND sudo cmake --install build
  WORKING_DIRECTORY ${exiv2_SOURCE_DIR}
)
add_custom_target(ExportExifHeader
  ALL DEPENDS PreInstallExiv2
)

# FFmpeg setup for Linux.
# sudo apt install --yes libavformat-dev libavcodec-dev libavutil-dev libswscale-dev
# Use pkg-config to find FFmpeg
find_package(PkgConfig REQUIRED)
# Find individual FFmpeg components
pkg_check_modules(AVFORMAT REQUIRED libavformat)
pkg_check_modules(AVCODEC REQUIRED libavcodec)
pkg_check_modules(AVUTIL REQUIRED libavutil)
