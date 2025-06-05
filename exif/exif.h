#pragma once

#include <string>

#include <nlohmann/json.hpp>

namespace nika::nas {

// Relavent Exiv2::ExifData
// Exif.Photo.DateTimeOriginal                  0x9003 Ascii      20  2025:04:01 17:05:14
// Exif.GPSInfo.GPSLatitudeRef                  0x0001 Ascii       2  N
// Exif.GPSInfo.GPSLatitude                     0x0002 Rational    3  47/1 42/1 1133/100
// Exif.GPSInfo.GPSLongitudeRef                 0x0003 Ascii       2  W
// Exif.GPSInfo.GPSLongitude                    0x0004 Rational    3  122/1 7/1 4141/100
// Exif.GPSInfo.GPSAltitudeRef                  0x0005 Byte        1  0
// Exif.GPSInfo.GPSAltitude                     0x0006 Rational    1  5359/100


class Exif {
public:
  nlohmann::json read(std::string file) const;
};

}  // namespace nika::nas
