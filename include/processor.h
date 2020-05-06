#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <fstream>
#include <regex>
#include <string>

class Processor {
 public:
  const std::string kProcDirectory{"/proc/"};
  const std::string kStatFilename{"/stat"};
  float Utilization(std::string cpu_number);  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
};

#endif