#pragma once

#include <string>

#include <nlohmann/json.hpp>

namespace nika::nas {

class Exif {
public:
  nlohmann::json read(std::string file) const;
};

}  // namespace nika::nas
