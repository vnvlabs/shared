#ifndef PROVANENCE_HEADER
#define PROVANENCE_HEADER
#include <iostream>
#include <string>
#include <vector>

#include "DistUtils.h"

# ifndef SWIG
#include "nlohmann/json.hpp"
using nlohmann::json;
# endif

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
  long crc = 0;

  VnV::DistUtils::libInfo info;

  ProvFile();
  
  ProvFile(std::string filename, std::string reader, std::string text = "", bool crc = false);  
  ProvFile(VnV::DistUtils::libInfo lb, std::string reader,  bool crc = false);

  # ifndef SWIG
  ProvFile(const json& json);
  json toJson() const;
  json getDataChildren()const; 
  # endif 

  long getCurrentCRC32(std::string file) {
    return VnV::HashUtils::crc32(file);
  }

};

class VnVProv {
  #ifndef SWIG
  json toArray(const std::vector<std::shared_ptr<ProvFile>>& array) const;
  void fromArray(std::vector<std::shared_ptr<ProvFile>>& array, const json& a);
  #endif

 public:
  std::string currentWorkingDirectory;
  long time_in_seconds_since_epoch;
  std::string commandLine;
  std::shared_ptr<ProvFile> inputFile;
  std::shared_ptr<ProvFile> executable;
  std::vector<std::shared_ptr<ProvFile>> inputFiles;
  std::vector<std::shared_ptr<ProvFile>> outputFiles;
  std::vector<std::shared_ptr<ProvFile>> libraries;

  #ifndef SWIG
  VnVProv(int argc, char** argv, std::string inputfileName, json& config);
  VnVProv(const json& j);
  json toJson() const;
  json getDataChildren()const; 
  #endif

  VnVProv();

  void setLibraries(const DistUtils::libData& lb);

  void addInputFile(std::shared_ptr<ProvFile> pv);

  void addOutputFile(std::shared_ptr<ProvFile> pv);

  std::shared_ptr<ProvFile> get(std::size_t index, int input) {
    return (input == 0) ? inputFiles[index] : (input == 1 ? outputFiles[index] : libraries[index]);
  }

  std::size_t size(int input) {
    return input == 0 ? inputFiles.size() : (input == 1 ? outputFiles.size() : libraries.size());
  }


  virtual ~VnVProv() {}
};

}  // namespace VnV

#endif