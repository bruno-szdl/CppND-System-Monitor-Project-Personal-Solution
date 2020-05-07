#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

Process::Process(int pid) : pid_(pid){
  cpu = LinuxParser::ActiveJiffies(pid);
}

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
  double seconds;
    seconds = LinuxParser::UpTime() - ((float)cpu / sysconf(_SC_CLK_TCK));
    return ((float)cpu / sysconf(_SC_CLK_TCK) / seconds );
}

// TODO: Return the command that generated this process
std::string Process::Command() { return LinuxParser::Command(Pid()); }

// TODO: Return this process's memory utilization
std::string Process::Ram() { return LinuxParser::Ram(Pid()); }

// TODO: Return the user (name) that generated this process
std::string Process::User() { return LinuxParser::User(Pid()); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(Pid()); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { return this->cpu > a.cpu; }