#pragma once

#include "absl/strings/string_view.h"
#include "re2/re2.h"

namespace nika::nas {

constexpr absl::string_view kRootDir = "/media";
constexpr absl::string_view kSambaPath = "/nas/public";
constexpr absl::string_view kStaging = "/staging";
constexpr LazyRE2 kYearDirMatcher = {R"re(^[0-9]{4})re"};
constexpr LazyRE2 kMonthDirMatcher = {R"re(^[0|1][0-9])re"};

}  // namespace nika::nas
