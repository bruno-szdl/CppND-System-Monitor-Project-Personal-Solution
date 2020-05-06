#include <string>
#include "processor.h"

using std::string;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization(string cpu_number) {
  string line, cpu;
  float user, nice, system, idle, iowait, irq, softirq, steal;
  std::ifstream filestream(kProcDirectory + kStatFilename);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal) {
        if (cpu == cpu_number) {
          float _idle = idle + iowait;
          float non_idle = user + nice + system + irq + softirq + steal;
          float total = _idle + non_idle;
          return (total - _idle)/total;
        }
      }
    }
  }
}