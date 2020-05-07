#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
std::vector<Process>& System::Processes() {
    processes_.clear();
    std::vector<int> pids = LinuxParser::Pids();
    
    for (int pid : pids){
        Process process(pid);
        processes_.emplace_back(process);
    }

    std::sort(processes_.begin(), processes_.end(), [](Process const& a, Process const& b) {return a < b;});
    return processes_;
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() const { return LinuxParser::Kernel(); }

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// TODO: Return the operating system name
std::string System::OperatingSystem() const { return LinuxParser::OperatingSystem(); }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }