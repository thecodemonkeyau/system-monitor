#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <filesystem>
#include <sstream>
#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// ==============================================================================
// System wide data calclations
// ==============================================================================

/// @brief An example of how to read data from the filesystem
/// provided by Udacity
/// @return the operating system name
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

/// @brief An example of how to read data from the filesystem
/// provided by Udacity
/// @return the kernel version
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

/// @brief read PIDs frrom the /proc directory
/// Modified Udacity example to use std::filesystem
/// @return vector of integers representing the PIDs
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  // use std::filesystem to iterate over the directory
  for (const auto &entry :
       std::filesystem::directory_iterator(kProcDirectory)) {
    if (entry.is_directory()) {  // check if it is a directory
      string filename = entry.path().filename().string();
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = std::stoi(filename);  // convert to int using std lib
        pids.push_back(pid);
      }
    }
  }
  return pids;
}

/// @brief Read and return the system memory utilization
/// File format:
/// MemTotal:  <value in kB>  # total memory
/// MemFree:   <value in kB>  # free memory
/// @return memory utilization as a percentage (0.0 - 1.0)
float LinuxParser::MemoryUtilization() {
  string line;
  string key;
  string value;

  int total_memory = 0;
  int free_memory = 0;

  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::remove_if(line.begin(), line.end(), isspace);
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal") {
          total_memory = std::stoi(value);
        } else if (key == "MemFree") {
          free_memory = std::stoi(value);
        }
        if (total_memory != 0 && free_memory != 0) {  // exit early
          break;
        }
      }
    }
  }
  return (float)(total_memory - free_memory) / total_memory;
}

/// @brief Read and return the system uptime
/// @details File format:
/// The first value represents the total number of seconds the system has been
/// up. The second value is the sum of how much time each core has spent idle,
/// in seconds. Consequently, the second value may be greater than the overall
/// system uptime on systems with multiple cores.
/// @return uptime in seconds
long LinuxParser::UpTime() {
  string uptime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
  }
  return std::stol(uptime);
}

/// @brief Read and return the number of jiffies for the system
/// based on stack overflow amswer
/// @see
/// https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
/// @return total number of jiffies
long LinuxParser::Jiffies() { return ActiveJiffies() + IdleJiffies(); }

/// @brief Read and return the number of active jiffies for the system
/// based on stack overflow amswer
/// @see
/// https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
/// @return number of active jiffies
long LinuxParser::ActiveJiffies() {
  // Read and return the number of active jiffies for the system
  auto jiffies = CpuUtilization();
  return stol(jiffies[CPUStates::kUser_]) + stol(jiffies[CPUStates::kNice_]) +
         stol(jiffies[CPUStates::kSystem_]) + stol(jiffies[CPUStates::kIRQ_]) +
         stol(jiffies[CPUStates::kSoftIRQ_]) +
         stol(jiffies[CPUStates::kSteal_]);
}

/// @brief Read and return the number of idle jiffies for the system
/// @details based on stack overflow answer
/// @see
/// https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
/// @return number of idle jiffies
long LinuxParser::IdleJiffies() {
  // Read and return the number of idle jiffies for the system
  auto jiffies = CpuUtilization();
  return stol(jiffies[CPUStates::kIdle_]) + stol(jiffies[CPUStates::kIOwait_]);
}

/// @brief Read and return CPU utilization valused
/// @details File format: data is on a separate line in the form
/// `cpu <user> <nice> <system> <idle> <iowait> <irq> <softirq> <steal>
/// <guest> <guest_nice>`
/// @return vector of strings representing the CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string line;
  vector<string> cpu_utilization = {};
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {  // we only want to top row for simple impl
    std::getline(stream, line);
    std::istringstream linestream(line);
    // skip the first word "cpu*"
    string cpu;
    linestream >> cpu;

    while (linestream) {
      string value;
      linestream >> value;
      cpu_utilization.push_back(value);
    }
  }
  return cpu_utilization;
}

/// @brief Read and return the total number of processes
/// @details File format: data is on a separate line in the form
/// `processes <number>`
/// @return total number of processes
int LinuxParser::TotalProcesses() {
  std::string line, key, value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {  // we only want to top row for simple impl
    while (line.rfind("processes", 0 != 0)) {
      std::getline(stream, line);
      std::istringstream linestream(line);
      linestream >> key >> value;
      break;
    }
  }
  return std::stoi(value);  // doesnt handle empty string
}

/// @brief Read and return the number of running processes
/// @details File format: data is on a separate line in the form
/// `procs_running <number>`
/// @return number of running processes
int LinuxParser::RunningProcesses() {
  std::string line, key, value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {  // we only want to top row for simple impl
    while (line.rfind("procs_running") == std::string::npos) {
      std::getline(stream, line);
    }
    std::istringstream linestream(line);
    linestream >> key >> value;
  }
  return std::stoi(value);
}

// ==============================================================================
// Per process data calclations
// ==============================================================================

/// @brief  Read and return the command associated with a process
/// @param pid the process ID
/// @return the command as a string
string LinuxParser::Command(int pid) {
  std::string line, key, value;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) std::getline(stream, line);
  return line;
}

/// @brief Read and return the user ID associated with a process
/// @param pid the process ID
/// @return the user ID as a string
string LinuxParser::Uid(int pid) {
  std::string line, key, value;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (line.rfind("Uid:") == std::string::npos) std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> key >> value;
  }
  return value;
}

/// @brief Read and return the user associated with a process
/// @param pid the process ID
/// @return the users username associated with this process as a string
string LinuxParser::User(int pid) {
  string line;
  string uid = Uid(pid);
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      std::string user, x, id;
      linestream >> user >> x >> id;
      if (id == uid) {
        return user;
      }
    }
  }
  return string();
}

/// @brief Read and return the number of active jiffies for a PID
/// @param pid the process ID
/// @return the number of active jiffies for this process
long LinuxParser::ActiveJiffies(int pid) {
  string line, value;
  vector<string> fields;
  long utime = 0, stime = 0, cutime = 0, cstime = 0;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    if (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream) {
        linestream >> value;
        fields.push_back(value);
      }
    }
    utime = std::stol(fields.at(ProcessStates::kUtime));
    stime = std::stol(fields.at(ProcessStates::kStime));
    cutime = std::stol(fields.at(ProcessStates::kCutime));
    cstime = std::stol(fields.at(ProcessStates::kCstime));
  }
  return utime + stime + cutime + cstime;  ///< total time including children
}

/// @brief read and return the uptime of a process
/// @param pid the process ID
/// @return the uptime of the process in seconds
long LinuxParser::UpTime(int pid) {
  // read the start time of the process
  string line, value;
  long uptime = 0;
  vector<string> fields = {};
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    if (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream) {
        linestream >> value;
        fields.push_back(value);
      }
    }
    auto proc_start_time = std::stol(
        fields.at(ProcessStates::kStarttime));  ///< system ticks at proc start
    auto system_uptime = UpTime();              // secon since boot
    auto hertz = sysconf(_SC_CLK_TCK);          // 5ticks per second
    uptime = system_uptime -
             (static_cast<float>(proc_start_time) / hertz);  // seconds
  }
  return uptime;
}

///  @brief Read and return the memory used by a process
///  @param pid the process ID
///  @return the memory used by this process as a string in kB
string LinuxParser::Ram(int pid) {
  std::string line, key, value;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (line.rfind("VmSize:") == std::string::npos)
      std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> key >> value;
    value = std::to_string(std::stol(value));
  }
  return value;  // RAM usage in kB
}
