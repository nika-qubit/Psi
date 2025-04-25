#include "exif.h"

#include <iostream>
#include <exiv2/exiv2.hpp>
#include <nlohmann/json.hpp>
#include "absl/log/log.h"
#include "absl/strings/substitute.h"

namespace nika::nas {

namespace {

using Exiv2::ExifData;
using Exiv2::ImageFactory;
using nlohmann::json;

}  // namespace


json Exif::read(std::string file) const {
  json exif_data;
  try {
    auto image = ImageFactory::open(file);
    if (image.get() == 0) {
      throw Exiv2::Error(Exiv2::ErrorCode::kerErrorMessage, "Could not open file.");
    }
    image->readMetadata();
    Exiv2::ExifData& exifData = image->exifData();
    Exiv2::ExifData::const_iterator end = exifData.end();
    for (Exiv2::ExifData::const_iterator i = exifData.begin(); i != end; ++i) {
        std::cout << std::setw(44) << std::setfill(' ') << std::left
                  << i->key() << " "
                  << "0x" << std::setw(4) << std::setfill('0') << std::right
                  << std::hex << i->tag() << " "
                  << std::setw(9) << std::setfill(' ') << std::left
                  << i->typeName() << " "
                  << std::dec << std::setw(3)
                  << std::right << i->count() << "  "
                  << std::dec << i->value()
                  << std::endl;
    }
  } catch (const Exiv2::Error& ex) {
    LOG(ERROR) << absl::Substitute("Failed to read EXIF data from file $0, error: $1.", file, ex.what());
  }
  return exif_data;
}

}  // namespace nika::nas
