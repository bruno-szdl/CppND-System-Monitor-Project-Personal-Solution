#include <string>
#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization(std::string cpu_number) { 
    return (float)LinuxParser::ActiveJiffies(cpu_number) / LinuxParser::Jiffies(cpu_number) ;
}