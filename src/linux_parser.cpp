#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

// DONE: An example of how to read data from the filesystem
std::string LinuxParser::OperatingSystem() {
  std::string line;
  std::string key;
  std::string value;
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
std::string LinuxParser::Kernel() {
  std::string os;
  std::string version;
  std::string kernel;
  std::string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
std::vector<int> LinuxParser::Pids() {
  std::vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      std::string filename(file->d_name);
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
float LinuxParser::MemoryUtilization() {
  float mem_total;
  float mem_free;
  std::string line;
  std::string key;
  std::string value;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while(std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal:") { 
          mem_total = stof(value); 
        }
        else if (key == "MemFree:") {
          mem_free = stof(value);
        }
      } 
    }
  }
  return (mem_total - mem_free)/mem_total; 
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  std::string line;
  std::string up_time;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> up_time;
  }
  return stol(up_time);
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies(std::string cpu_number) {
  return ActiveJiffies(cpu_number) + IdleJiffies(cpu_number);
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
  std::string value;
  long utime;
  long stime;
  long cutime;
  long cstime;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    int iter = 1;
    while (filestream >> value) { 
      if( iter == 14){
        utime = std::stol(value);
      } 
      else if(iter == 15){
        stime = std::stol(value);
      }
      else if(iter == 16){
        cutime = std::stol(value);
      }
      else if(iter == 17){
        cstime = std::stol(value);
        break;
      }
      iter++;
    }
  }
  return utime + stime + cutime + cstime;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies(std::string cpu_number) {
  std::vector<std::string> values = CpuUtilization(cpu_number);
  return stol(values[CPUStates::kUser_]) + stol(values[CPUStates::kNice_]) + 
      stol(values[CPUStates::kSystem_]) + stol(values[CPUStates::kIRQ_]) +
      stol(values[CPUStates::kSoftIRQ_]) + stol(values[CPUStates::kSteal_]) +
      stol(values[CPUStates::kGuest_]) + stol(values[CPUStates::kGuestNice_]);
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies(std::string cpu_number) {
  std::vector<std::string> values = CpuUtilization(cpu_number);
  return std::stol(values[CPUStates::kIdle_]) + std::stol(values[CPUStates::kIOwait_]);
}

// TODO: Read and return CPU utilization
std::vector<std::string> LinuxParser::CpuUtilization(std::string cpu_number) {
  std::vector<std::string> cpu;
  std::string line;
  std::string key;
  std::string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key) {
        if (key == cpu_number){
          while (linestream >> key){
            cpu.emplace_back(key);
          } 
          return cpu;
        }
      }
    }
  }
  return cpu;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  std::string line;
  std::string key;
  std::string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while(std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") { 
          return stoi(value); 
        }
      } 
    }
  }
  return 0;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  std::string line;
  std::string key;
  std::string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while(std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") { 
          return stoi(value); 
        }
      } 
    }
  }
  return 0;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
std::string LinuxParser::Command(int pid) {
  std::string line;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()){
    std::getline(filestream, line);
  }
  return line;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
std::string LinuxParser::Ram(int pid) {
  std::string line;
  std::string key;
  std::string value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while(std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize:") { 
          return std::to_string(stoi(value)/1024); 
        }
      } 
    }
  }
  return 0;
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
std::string LinuxParser::Uid(int pid) {
  std::string line;
  std::string key;
  std::string value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while(std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid:") { 
          return value; 
        }
      } 
    }
  }
  return std::string();
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
std::string LinuxParser::User(int pid) {
  std::string line;
  std::string key;
  std::string x;
  std::string value;
  std::string user;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while(std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> value >> x >> key) {
        if (key == Uid(pid)) {
          user = value; 
        }
      } 
    }
  }
  return user;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  std::string line;
  std::string value;
  std::string starttime;
    std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
    if (filestream.is_open()) {
      int iter = 1;
      while (filestream >> value) { 
        if(iter == 22){
          starttime = value;
          break;
        }
        iter++;
      }
    }
    long upTime = atol(starttime.c_str());
    return upTime / sysconf(_SC_CLK_TCK);
}