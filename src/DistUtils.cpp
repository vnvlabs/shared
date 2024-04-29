
/**
  @file DistUtils.cpp
**/

#include "shared/DistUtils.h"
#include "shared/Utilities.h"
#include "shared/exceptions.h"

#include <sys/file.h>
#include <unistd.h>
#include <sys/stat.h>

#include <iostream>
#include <sstream>
#include<filesystem>



using nlohmann::json;
namespace VnV {
namespace DistUtils {

libInfo getLibInfo(std::string filepath, unsigned long add) {
  libInfo info;
  info.name = getAbsolutePath(filepath);
  try {
    info.size = std::filesystem::file_size(filepath);
  } catch (...) {
    info.size = 0;
  }
  try {

    struct stat result;
    stat(filepath.c_str(), &result);
    info.timestamp = result.st_mtim.tv_sec;
    
    // Can use this once we go to C++20. Until then, the epoch time is not fixed to unix epoch time.
    //info.timestamp = std::filesystem::last_write_time(filepath).time_since_epoch().count();

  } catch (...) {
    info.timestamp = 0;
  }
  return info;
}

std::string getCurrentDirectory() { return std::filesystem::current_path().string(); }

std::string getAbsolutePath(std::string filename) {
  return std::filesystem::absolute(filename).string();
}

bool fileEquals(std::string f1, std::string f2) { return getAbsolutePath(f1).compare(getAbsolutePath(f2)) == 0; }

bool fileInDirectory(std::string file, std::string directory) {
  auto ap = getAbsolutePath(file);
  auto ad = getAbsolutePath(directory);
  return ap.rfind(ad, 0) == 0;
}

bool makedir(std::string filename, mode_t mode) { 
    return std::filesystem::create_directory(filename); 
}

bool mv(std::string oldFileName, std::string newFileName) {
  std::filesystem::rename(oldFileName.c_str(), newFileName.c_str());
  return true;
}


bool cp(std::string oldFileName, std::string newFilename) {
  // Gotta be a better way than this right?
  std::ifstream src(oldFileName, std::ios::binary);
  std::ofstream dst(newFilename, std::ios::binary);
  dst << src.rdbuf();
  return true;
}

std::string copy_file(std::string input, std::string fileStub) {
    
    namespace fs = std::filesystem;
    
    // Define the source path
    fs::path source_path(input);

    // Define the destination directory
    fs::path destination_directory = fileStub;

    // Create the destination directory if it doesn't exist
    if (!fs::exists(destination_directory.parent_path())) {
        fs::create_directories(destination_directory.parent_path());
    }

    // Check if the input path exists
    if (!fs::exists(source_path)) {
        throw "Error";
    }

    // Construct the destination path
    fs::path destination_path = destination_directory;

    try {
        // Check if the source is a directory
        if (fs::is_directory(source_path)) {
            // Copy directory recursively
            fs::copy(source_path, destination_path, fs::copy_options::recursive);
            return destination_path;
        } else if (fs::is_regular_file(source_path)) {
            // Copy file
            fs::copy_file(source_path, destination_path, fs::copy_options::overwrite_existing);
            return destination_path;

        } else {
           throw "ERror";
        }
    } catch (const fs::filesystem_error& e) {
        throw "Error";
    }
}


bool ln(std::string oldFileName, std::string newFilename, bool hard) {
  if (hard) {
    std::filesystem::create_hard_link(oldFileName.c_str(), newFilename.c_str());
  } else {
    std::filesystem::create_symlink(oldFileName.c_str(), newFilename.c_str());
  }
  return true;
}

std::string getTempFolder() {
  char const* folder = getenv("TMPDIR");
  if (!folder) folder = getenv("TMP");
  if (!folder) folder = getenv("TMP");
  if (!folder) folder = getenv("TEMP");
  if (!folder) folder = getenv("TEMPDIR");
  if (!folder) folder = "/tmp";
  return folder;
}

void makedirs(std::string path, bool make_last = false, mode_t mode = 0777) {
  std::vector<std::string> dirs;
  StringUtils::StringSplit(path, "/", dirs, true);

  std::string s = "";

  int end = (make_last) ? dirs.size() : dirs.size() - 1;
  for (int i = 0; i < end; i++) {
    s += dirs[i] + "/";
    try {
      makedir(s, mode);
    } catch (...) {
    }
  }
}

std::string getFileSeparator() {
    char separator = std::filesystem::path::preferred_separator;
    return std::string(1, separator); // Convert char to string
}

std::string join(std::vector<std::string> vec_i, mode_t i, bool makeDir, bool isFile) {
  if (vec_i.size() > 0) {
    std::string s = "";
    for (auto it = vec_i.begin(); it != vec_i.end(); ++it) {
      if (it->size() == 0)
        continue;
      else if (it->substr(0, 1).compare(getFileSeparator()) == 0) {
        s = *it;  // If it starts with a "/" then its a root path.
      } else {
        s = s + *it;  // It gets appended.
      }

      // If we are not at the end, then add a slash.
      if ((it + 1) != vec_i.end()) {
        s +=  getFileSeparator();
      }
    }

    if (makeDir) {
      makedirs(s, !isFile, i);
    }
    return s;
  }
  throw INJECTION_EXCEPTION_("Empty directory list passed to join");
}

std::string getTempFile(std::string code, std::string ext) {
  return join({getTempFolder(), "vnv_" + code + ext}, 077, false);
}



void permissions(std::string filename, bool read, bool write, bool execute) {
    try {
        // Get the current permissions of the file
        auto current_perms = std::filesystem::status(filename).permissions();

        // Update the permissions based on the input flags
        if (read) {
            current_perms |= std::filesystem::perms::owner_read;
        } else {
            current_perms &= ~std::filesystem::perms::owner_read;
        }

        if (write) {
            current_perms |= std::filesystem::perms::owner_write;
        } else {
            current_perms &= ~std::filesystem::perms::owner_write;
        }

        if (execute) {
            current_perms |= std::filesystem::perms::owner_exec;
        } else {
            current_perms &= ~std::filesystem::perms::owner_exec;
        }

        // Set the updated permissions for the file
        std::filesystem::permissions(filename, current_perms);
        
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Error setting permissions for " << filename << ": " << e.what() << std::endl;
    }
}

bool fileExists(std::string filename) {
  return std::filesystem::exists(filename);
}

std::string getEnvironmentVariable(std::string name, std::string def) {
  const char* val = std::getenv(name.c_str());
  if (val) {
    return val;
  }
  return def;
}

std::vector<std::string> listFilesInDirectory(std::string directory) {
    std::vector<std::string> files;

    try {
        // Iterate over each entry in the directory
        for (const auto& entry : std::filesystem::directory_iterator(directory)) {
            // Check if the entry is a regular file
            if (entry.is_regular_file()) {
                // Add the file path to the vector
                files.push_back(entry.path().filename().string());
            }
        }
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Error listing files in directory " << directory << ": " << e.what() << std::endl;
    }

    return files;
}

void initialize_lock(LockFile* lockfile) {}
void lock_file(LockFile* lockfile) { flock(lockfile->fd, LOCK_EX); }
void unlock_file(LockFile* lockfile) { flock(lockfile->fd, LOCK_UN); }
void close_file(LockFile* lockfile) {
  unlock_file(lockfile);
  close(lockfile->fd);
}


}  // namespace DistUtils
}  // namespace VnV
