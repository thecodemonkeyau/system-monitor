#include "process.h"

#include <unistd.h>

#include <cctype>
#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

/// @brief constructor accpeting process's ID
/// @param pid this process's ID
Process::Process(int pid) {
  pid_ = pid;
  cpu_utlization_ = 0;
  hertz_ = sysconf(_SC_CLK_TCK);

  // need to calculate this at constructions to get sorting to work
  float active_jiffies =
      LinuxParser::ActiveJiffies(pid_);     // ticks sunce system start
  long uptime = LinuxParser::UpTime(pid_);  // seconds
  if (uptime > 0) {
    cpu_utlization_ = (active_jiffies / hertz_) / static_cast<float>(uptime);
  }

  // seems like we ar really reading 0 0 0 0 for active_jiffies?
  // 0 seems a valid for uptime (rounding down for small values)
  // nan means active_jiffies is 0 and uptime may also be 0
  // inf means uptime is 0
  if (std::isnan(cpu_utlization_) || std::isinf(cpu_utlization_)) {
    cpu_utlization_ = 0.0;  // return 0.0 if we can't calculate it
  }
  string mem = LinuxParser::Ram(pid_);
  if (IsNumeric(mem)) {
    ram_usage_ = std::stol(mem) / 1024;  // convert to MB
  } else {
    ram_usage_ = 0;
  }
}

/// @brief check if the string is actually numeric
/// @param s the string to check
/// @return true if the string is composed entirely of numbers
bool Process::IsNumeric(const std::string& s) {
  return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
}

/// @brief get this process's ID
/// @brief Return this process's ID
int Process::Pid() { return pid_; }

/// @brief calculate this process's CPU utilization
/// @todo this seems wrong. Numbers seem overly stable
/// @return this process's CPU utilization as a percentage (0.0 - 1.0)
float Process::CpuUtilization() { return cpu_utlization_; }

/// @brief query the command that generated this process
/// @return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_); }

/// @brief:calculate this process's memory utilization
/// @return this process's memory utilization
long Process::Ram() { return ram_usage_; }

/// @brief Query the user (name) that generated this process
/// @return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid_); }

/// @brief calculate the age of this process (in seconds)
/// @return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

/// @brief Overload the "less than" comparison operator for Process objects
/// @return true if this process has less CPU utilization than the input process
bool Process::operator<(Process const& a) const {
  return this->cpu_utlization_ < a.cpu_utlization_;
}

/// @brief Overload the "greater than" comparison operator for Process objects
/// @return true if this process has more CPU utilization than the input process
bool Process::operator>(Process const& a) const {
  return this->cpu_utlization_ > a.cpu_utlization_;
}