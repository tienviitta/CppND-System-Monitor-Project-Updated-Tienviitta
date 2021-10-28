#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <map>
#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// The /proc/stat string split is based on method:
// - https://stackoverflow.com/a/60782724/2153439

// The /proc/meminfo string split is based on method:
// - https://stackoverflow.com/a/60782724/2153439
// - https://stackoverflow.com/a/26530289/2153439

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization (OBSOLETE)
float LinuxParser::MemoryUtilization() {
  string line;
  string key;
  long value;
  std::map<std::string, long> meminfo;
  char delim = ' ';
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      size_t start = line.find_first_not_of(delim, 0);
      size_t end = line.find(delim, start);
      // Assume first string is key value
      key = line.substr(start, end - start);
      start = line.find_first_not_of(delim, end);
      end = line.find(delim, start);
      // Assume second string is value and convert to long
      std::istringstream(line.substr(start, end - start)) >> value;
      meminfo[key] = value;
    }
  }
  // System memory utilization computation is based on <TODO>
  long totalUsed = meminfo["MemTotal:"] - meminfo["MemFree:"];
  long memTotal = meminfo["MemTotal:"];
  return static_cast<float>(totalUsed) / static_cast<float>(memTotal);
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { return 0; }

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid [[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string line;
  char delim = ' ';
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    std::vector<std::string> cpustat;
    size_t start;
    size_t end = 0;
    while ((start = line.find_first_not_of(delim, end)) != std::string::npos) {
      end = line.find(delim, start);
      cpustat.push_back(line.substr(start, end - start));
    }
    return cpustat;
  }
  return {};
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { return 0; }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { return 0; }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid [[maybe_unused]]) { return 0; }

// Read and return the /proc/stat
std::map<std::string, std::vector<long>> LinuxParser::ParseProcStat() {
  string key;
  string line;
  long value;
  char delim = ' ';
  std::map<std::string, std::vector<long>> procStat;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::vector<long> values = {};
      std::istringstream linestream(line);
      // Assume first string is key value
      size_t start = line.find_first_not_of(delim, 0);
      size_t end = line.find(delim, start);
      key = line.substr(start, end - start);
      // Convert other strings to long
      while ((start = line.find_first_not_of(delim, end)) !=
             std::string::npos) {
        end = line.find(delim, start);
        std::istringstream(line.substr(start, end - start)) >> value;
        values.push_back(value);
      }
      procStat[key] = values;
    }
  }
  return procStat;
}

// Read and return the /proc/meminfo
std::map<std::string, long> LinuxParser::ParseProcMeminfo() {
  string line;
  string key;
  long value;
  std::map<std::string, long> procMeminfo;
  char delim = ' ';
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      size_t start = line.find_first_not_of(delim, 0);
      size_t end = line.find(delim, start);
      // Assume first string is key value
      key = line.substr(start, end - start);
      start = line.find_first_not_of(delim, end);
      end = line.find(delim, start);
      // Assume second string is value and convert to long
      std::istringstream(line.substr(start, end - start)) >> value;
      procMeminfo[key] = value;
    }
  }
  return procMeminfo;
}