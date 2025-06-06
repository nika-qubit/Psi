#pragma once

#include <iostream>
#include <string>

#include "absl/strings/string_view.h"
#include "absl/time/time.h"
#include <nlohmann/json.hpp>

namespace nika::nas {

struct Metadata {
  absl::Time original_date_time;
  std::string unparsed_date;
};

inline std::ostream& operator<<(std::ostream& os, const Metadata& val) {
  os << "original_date_time: "
      << absl::FormatTime(
          absl::RFC3339_full, val.original_date_time, absl::LocalTimeZone())
      << ", unparsed_date: " << val.unparsed_date << std::endl;
  return os;
}

class Exif {
public:
  nlohmann::json Read(absl::string_view file) const;
  Metadata Distill(nlohmann::json& json_metadata) const;
};

}  // namespace nika::nas
