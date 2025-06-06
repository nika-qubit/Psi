#include "exif.h"

#include <iostream>
#include <sstream>
#include <string>

#include "absl/log/log.h"
#include "absl/strings/str_cat.h"
#include "absl/strings/string_view.h"
#include "absl/strings/substitute.h"
#include "absl/time/time.h"
#include <exiv2/exiv2.hpp>
#include <nlohmann/json.hpp>

namespace nika::nas {

namespace {

using Exiv2::ExifData;
using Exiv2::ImageFactory;
using nlohmann::json;

// Relavent Exiv2::ExifData
// Exif.Photo.DateTimeOriginal                  0x9003 Ascii      20  2025:04:01 17:05:14
// Exif.Photo.OffsetTimeOriginal                0x9011 Ascii       7  -07:00
// Exif.GPSInfo.GPSLatitudeRef                  0x0001 Ascii       2  N
// Exif.GPSInfo.GPSLatitude                     0x0002 Rational    3  47/1 42/1 1133/100
// Exif.GPSInfo.GPSLongitudeRef                 0x0003 Ascii       2  W
// Exif.GPSInfo.GPSLongitude                    0x0004 Rational    3  122/1 7/1 4141/100
// Exif.GPSInfo.GPSAltitudeRef                  0x0005 Byte        1  0
// Exif.GPSInfo.GPSAltitude                     0x0006 Rational    1  5359/100
constexpr absl::string_view kDateTimeOriginal = "Exif.Photo.DateTimeOriginal";
constexpr absl::string_view kOffsetTimeOriginal = "Exif.Photo.OffsetTimeOriginal";
constexpr absl::string_view kDateTimeWithOffsetFormat = "%Y:%m:%d %H:%M:%S%Ez";

}  // namespace

Metadata Exif::Distill(json& json_metadata) const {
  Metadata metadata;
  std::string date_time_with_offset = absl::StrCat(
      json_metadata[kDateTimeOriginal].template get<std::string>(),
      json_metadata[kOffsetTimeOriginal].template get<std::string>());
  // Substr the Y:m:d part of the unparsed date time.
  metadata.unparsed_date = date_time_with_offset.substr(0, 10);
  absl::Time date_time;
  std::string error;
  if (absl::ParseTime(kDateTimeWithOffsetFormat, date_time_with_offset, &date_time, &error)) {
    metadata.original_date_time = date_time;
  } else {
    LOG(ERROR) << "Error parsing " << date_time_with_offset << " error: " << error;
  }
  return metadata;
}

json Exif::Read(absl::string_view file) const {
  json exif_data;
  try {
    auto image = ImageFactory::open(std::string(file));
    if (image.get() == 0) {
      throw Exiv2::Error(Exiv2::ErrorCode::kerErrorMessage, "Could not open file.");
    }
    image->readMetadata();
    const Exiv2::ExifData& exifData = image->exifData();
    Exiv2::ExifData::const_iterator end = exifData.end();
    std::ostringstream oss;
    for (Exiv2::ExifData::const_iterator i = exifData.begin(); i != end; ++i) {
        VLOG(10) << std::setw(44) << std::setfill(' ') << std::left
                  << i->key() << " "
                  << "0x" << std::setw(4) << std::setfill('0') << std::right
                  << std::hex << i->tag() << " "
                  << std::setw(9) << std::setfill(' ') << std::left
                  << i->typeName() << " "
                  << std::dec << std::setw(3)
                  << std::right << i->count() << "  "
                  << std::dec << i->value()
                  << std::endl;
        oss.str("");
        oss.clear();
        i->value().write(oss);
        exif_data[i->key()] = oss.str();
    }
  } catch (const Exiv2::Error& ex) {
    LOG(ERROR) << absl::Substitute("Failed to read EXIF data from file $0, error: $1.", file, ex.what());
  }
  return exif_data;
}

}  // namespace nika::nas
