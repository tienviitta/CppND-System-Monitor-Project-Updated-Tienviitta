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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { return 0.0; }

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  float uptime, idle;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime >> idle;
  }
  // return static_cast<long>(uptime);
  return static_cast<long>(uptime + idle);
}

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
      cpustat.emplace_back(line.substr(start, end - start));
    }
    return cpustat;
  }
  return {};
}

// TODO: Read and return CPU utilization
float LinuxParser::CpuUtilization(int pid) {
  string line;
  long value;
  char delim = ' ';
  std::vector<long> cpustat;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    size_t start;
    size_t end = 0;
    while ((start = line.find_first_not_of(delim, end)) != std::string::npos) {
      end = line.find(delim, start);
      std::istringstream(line.substr(start, end - start)) >> value;
      cpustat.emplace_back(value);
    }
  }
  long herz = sysconf(_SC_CLK_TCK);
  long upTime = LinuxParser::UpTime();
  long totalTime = cpustat[13] + cpustat[14] + cpustat[15] + cpustat[16];
  long startTime = cpustat[21];
  long seconds = upTime - (startTime / herz);
  float cpu =
      static_cast<float>((totalTime / herz)) / static_cast<float>(seconds);
  return cpu;
}

// TODO: Read and return the total number of processes (OBSOLETE)
int LinuxParser::TotalProcesses() { return 0; }

// TODO: Read and return the number of running processes (OBSOLETE)
int LinuxParser::RunningProcesses() { return 0; }

// TODO: Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string cmd, type, path;
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> cmd >> type >> path;
  }
  return cmd;
}

// TODO: Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize:") {
          long value_int = std::stol(value) / 1000;
          return to_string(value_int);
          // return value;
        }
      }
    }
  }
  return "0";
}

// TODO: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid:") {
          return value;
        }
      }
    }
  }
  return value;
}

// TODO: Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string uid = LinuxParser::Uid(pid);
  string line;
  char delim = ':';
  std::map<std::string, std::vector<long>> procStat;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::vector<std::string> values = {};
      std::istringstream linestream(line);
      size_t start;
      size_t end = 0;
      while ((start = line.find_first_not_of(delim, end)) !=
             std::string::npos) {
        end = line.find(delim, start);
        values.emplace_back(line.substr(start, end - start));
      }
      if (values[2] == uid) {
        return values[0];
      }
    }
  }
  return "";
}

// TODO: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  std::vector<long> cpustat = LinuxParser::ParseProcPidStat(pid);
  return cpustat[21];
}

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
        values.emplace_back(value);
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

// Read and return the /proc/[pid]/stat
std::vector<long> LinuxParser::ParseProcPidStat(int pid) {
  string line;
  long value;
  char delim = ' ';
  std::vector<long> cpustat;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    size_t start;
    size_t end = 0;
    while ((start = line.find_first_not_of(delim, end)) != std::string::npos) {
      end = line.find(delim, start);
      std::istringstream(line.substr(start, end - start)) >> value;
      cpustat.emplace_back(value);
    }
  }
  return cpustat;
}
