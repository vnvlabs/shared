
#include <iostream>
#include "shared/Provenance.h"
#include "shared/Utilities.h"

using namespace VnV;

ProvFile::ProvFile() {}

ProvFile::ProvFile(std::string filename, std::string reader, std::string text)
{
  this->filename = filename;
  this->reader = reader;
  this->text = text;

  try
  {
    info = DistUtils::getLibInfo(filename, 0);
  }
  catch (std::exception &e)
  {
  }
}

ProvFile::ProvFile(DistUtils::libInfo lb, std::string reader)
{
  this->filename = lb.name;
  this->reader = reader;
  this->text = "";
  this->info = lb;
}

json ProvFile::toJson() const
{
  json j = json::object();
  j["filename"] = filename;
  j["reader"] = reader;
  j["text"] = text;
  j["info"] = info.toJson();
  j["comm"] = comm;
  j["package"] = package;
  j["name"] = name;
  j["copy"] = copy;
  j["crc"] = crc;
  return j;
}

json VnVProv::toArray(const std::vector<std::shared_ptr<ProvFile>> &array) const
{
  json a = json::array();
  for (auto it : array)
  {
    a.push_back(it->toJson());
  }
  return a;
}

ProvFile::ProvFile(const json& j) {
  this->filename = j["filename"].get<std::string>();
  this->reader = j["reader"].get<std::string>();
  this->text = j["text"].get<std::string>();
  this->info.fromJson(j["info"]);
  this->comm = j["comm"].get<long>();
  this->package = j["package"].get<std::string>();
  this->name = j["name"].get<std::string>();
  this->copy = j.value("copy", false);
  this->crc = j.value("crc", (uint32_t) 0);
}

json ProvFile::getDataChildren() const {

    json o = json::object();
    o["text"] = name;
    o["icon"] = "feather icon-file";
    o["children"] = json::array();
    
    if (text.size() > 0 ) {
      json j = json::object();
      j["text"] = "Text: " + text;
      j["icon"] = "feather icon-minus";
      o["children"].push_back(j);
    }

    {
      json j = json::object();
      j["text"] = "Package: " + package;
      j["icon"] = "feather icon-minus";
      o["children"].push_back(j);
    }
    {
      json j = json::object();
      j["text"] = "Filename: " + filename;
      j["icon"] = "feather icon-minus";
      o["children"].push_back(j);
    }
     {
      json j = json::object();
      j["text"] = "CRC32: " + crc;
      j["icon"] = "feather icon-minus";
      o["children"].push_back(j);
    }
    return o;

}

void VnVProv::fromArray(std::vector<std::shared_ptr<ProvFile>>& array, const json& a) {
  for (auto& it : a) {
    array.push_back(std::make_shared<ProvFile>(it));
  }
}
VnVProv::VnVProv(const json& j) {

  currentWorkingDirectory = j["cwd"].get<std::string>();
  time_in_seconds_since_epoch = j["time"].get<long>();
  commandLine = j["cmd"].get<std::string>();
  inputFile.reset(new ProvFile(j["inp"]));
  executable.reset(new ProvFile(j["exe"]));

  fromArray(inputFiles, j["inps"]);
  fromArray(outputFiles, j["outs"]);
  fromArray(libraries, j["libs"]);
}

VnVProv::VnVProv(int argc, char **argv, std::string inputfileName, json &config)

{
  inputFile.reset(new ProvFile(inputfileName, "json", config.dump(4)));
  executable.reset(new ProvFile(argv[0], "binary"));
  currentWorkingDirectory = DistUtils::getCurrentDirectory();
  time_in_seconds_since_epoch = time(NULL);
  commandLine = VnV::ProvenanceUtils::cmdLineToString(argc, argv);

}

void VnVProv::setLibraries(const DistUtils::libData& lb) {
  for (auto it : lb.libs)
  {
    ProvFile pf(it, "binary");
    libraries.push_back(std::make_shared<ProvFile>(it, "binary"));
  }
}

json VnVProv::toJson() const
{
  json j = json::object();

  j["cwd"] = currentWorkingDirectory;
  j["time"] = time_in_seconds_since_epoch;
  j["cmd"] = commandLine;
  j["inp"] = inputFile->toJson();
  j["exe"] = executable->toJson();
  j["inps"] = toArray(inputFiles);
  j["outs"] = toArray(outputFiles);
  j["libs"] = toArray(libraries);
  return j;
}

VnVProv::VnVProv() {}

json VnVProv::getDataChildren() const
{
  json j = json::object();
  j["text"] = "provenance";
  j["icon"] = "feather icon-folder";

  json ch = json::array();

  json time_in_secs = json::object();
  time_in_secs["text"] = "Unix Start Time: " + std::to_string(time_in_seconds_since_epoch) + " seconds";
  time_in_secs["icon"] = "feather icon-watch";
  ch.push_back(time_in_secs);

  json wdir = json::object();
  wdir["text"] = "Working Directory: " + currentWorkingDirectory;
  wdir["icon"] = "feather icon-text";
  ch.push_back(wdir);

  json commandLineJ = json::object();
  commandLineJ["text"] = "Command Line: " + commandLine;
  commandLineJ["icon"] = "feather icon-text";
  ch.push_back(commandLineJ);

  if (inputFile != nullptr)
  {
    ch.push_back(inputFile->getDataChildren());
    ch.back()["text"] = "Input File: " + ch.back()["text"].template get<std::string>();
  }
  if (executable != nullptr)
  {
    ch.push_back(executable->getDataChildren());
    ch.back()["text"] = "Executable: " + ch.back()["text"].template get<std::string>();
  }

  std::vector<std::string> names = {"Application Input Files", "Application Output Files", "Shared Libraries"};

  for (int i = 0; i < names.size(); i++)
  {

    const std::vector<std::shared_ptr<ProvFile>> &data = (i == 0) ? inputFiles : (i == 1) ? outputFiles
                                                                                          : libraries;

    if (data.size() > 0)
    {
      json inputs = json::object();
      inputs["text"] = names[i];
      inputs["icon"] = "feather icon-list";

      int count = 0;
      json child = json::array();
      for (auto &it : inputFiles)
      {
        child.push_back(it->getDataChildren());
        child.back()["text"] = std::to_string(count++) + ": " + child.back()["text"].template get<std::string>();
      }
      inputs["children"] = child;
      ch.push_back(inputs);
    }
  }

  j["children"] = ch;
  return j;
}

void VnVProv::addInputFile(std::shared_ptr<ProvFile> pv) { inputFiles.push_back(pv); }

void VnVProv::addOutputFile(std::shared_ptr<ProvFile> pv) { outputFiles.push_back(pv); }
