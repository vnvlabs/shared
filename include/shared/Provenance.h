#ifndef PROVANENCE_HEADER
#define PROVANENCE_HEADER
#include <iostream>
#include <string>
#include <vector>

#include "DistUtils.h"
#include "nlohmann/json.hpp"

using nlohmann::json;

namespace VnV {

class ProvFile {
 public:
  std::string filename = "<unset>";
  std::string reader = "";
  std::string text = "";
  std::string package = "root";
  std::string name = "<none>";
  bool copy = false;
  long comm = -1;
  uint32_t crc = 0;

  VnV::DistUtils::libInfo info;

  ProvFile();
  ProvFile(const json& json);
  ProvFile(std::string filename, std::string reader, std::string text = "");
  ProvFile(VnV::DistUtils::libInfo lb, std::string reader);

  json toJson() const;

  json getDataChildren()const; 

  std::string removeCommonPrefix(const std::string& path1, const std::string& path2) {
    std::string result;
    
    size_t minLength = std::min(path1.size(), path2.size());
    size_t i = 0;
    
    // Find the common prefix
    while (i < minLength && path1[i] == path2[i]) {
        i++;
    }

    // Extract the rest of path2 after the common prefix
    result = path2.substr(i);

    return result;
}

  void set_relative_to(std::string fileStub, std::string reportDirectory) {
      auto a = removeCommonPrefix(fileStub, filename );
      filename = reportDirectory + "/" + a;
  }

  uint32_t getCurrentCRC32() {
    return VnV::HashUtils::crc(filename);
  }

};

class VnVProv {
  json toArray(const std::vector<std::shared_ptr<ProvFile>>& array) const;
  void fromArray(std::vector<std::shared_ptr<ProvFile>>& array, const json& a);

 public:
  std::string currentWorkingDirectory;
  long time_in_seconds_since_epoch;
  std::string commandLine;
  std::shared_ptr<ProvFile> inputFile;
  std::shared_ptr<ProvFile> executable;
  std::vector<std::shared_ptr<ProvFile>> inputFiles;
  std::vector<std::shared_ptr<ProvFile>> outputFiles;
  std::vector<std::shared_ptr<ProvFile>> libraries;

  VnVProv(int argc, char** argv, std::string inputfileName, json& config);

  json toJson() const;

  VnVProv();
  VnVProv(const json& j);
  
  void setLibraries(const DistUtils::libData& lb);

  void addInputFile(std::shared_ptr<ProvFile> pv);

  void addOutputFile(std::shared_ptr<ProvFile> pv);

  std::shared_ptr<ProvFile> get(std::size_t index, int input) {
    return (input == 0) ? inputFiles[index] : (input == 1 ? outputFiles[index] : libraries[index]);
  }

  std::size_t size(int input) {
    return input == 0 ? inputFiles.size() : (input == 1 ? outputFiles.size() : libraries.size());
  }

 json getDataChildren()const; 

  virtual ~VnVProv() {}
};

}  // namespace VnV

#endif