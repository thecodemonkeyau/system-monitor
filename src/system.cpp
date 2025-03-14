#include "system.h"

#include <unistd.h>

#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;
/*You need to complete the mentioned TODOs in order to satisfy the rubric
criteria "The student will be able to extract and display basic data about the
system."

You need to properly format the uptime. Refer to the comments mentioned in
format. cpp for formatting the uptime.*/

/// Return the system's CPU
/// @return system's CPU
Processor& System::Cpu() { return cpu_; }

// Return a container composed of the system's processes
vector<Process>& System::Processes() { return processes_; }

/// Return the system's kernel identifier (string)
/// @return kernel identifier
std::string System::Kernel() { return LinuxParser::Kernel(); }

/// Return the system's memory utilization
/// @return memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

/// Return the operating system name
/// @return operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

/// Return the number of processes actively running on the system
/// @return number of running processes
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

/// Return the total number of processes on the system
/// @return total number of processes
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

/// Return the number of seconds since the system started running
/// @return number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }
