add_library(
  exif_lib
  exif.cpp exif.h
)

target_link_libraries(
  exif_lib
  absl::strings
  absl::log
  Exiv2::exiv2lib
  nlohmann_json::nlohmann_json
  ${AVFORMAT_LIBRARIES}
  ${AVCODEC_LIBRARIES}
  ${AVUTIL_LIBRARIES}
)

target_include_directories(exif_lib
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_INSTALL_PREFIX}/include/exiv2
  ${AVFORMAT_INCLUDE_DIRS}
  ${AVCODEC_INCLUDE_DIRS}
  ${AVUTIL_INCLUDE_DIRS}
)
