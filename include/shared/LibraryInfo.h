#ifndef LIBRARY_INFO_HEADER_H
#define LIBRARY_INFO_HEADER_H

#include <string>

#include "shared/json.hpp"

namespace VnV {
namespace DistUtils {

struct libInfo {
  std::string name = "<unknown>";
  long timestamp = 0;
  long size = 0;

  #ifndef SWIG
  nlohmann::json toJson() const {
    nlohmann::json j = nlohmann::json::object();
    j["name"] = name;
    j["timestamp"] = timestamp;
    j["size"] = size;
    return j;
  }

  void fromJson(const nlohmann::json& j) {
    name = j["name"].get<std::string>();
    timestamp = j["timestamp"].get<long>();
    size = j["size"].get<long>();
  }
  #endif 
};

struct libData {
  std::vector<libInfo> libs;
  libData() {}
};

}  // namespace DistUtils
}  // namespace VnV

#endif
