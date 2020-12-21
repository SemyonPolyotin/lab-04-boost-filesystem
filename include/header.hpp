// Copyright 2020 Your Name <your_email>

#ifndef INCLUDE_HEADER_HPP_
#define INCLUDE_HEADER_HPP_

#include <boost/filesystem.hpp>
#include <iostream>
#include <map>
#include <string>
#include <vector>

namespace fs = boost::filesystem;

const char underscores = '_';
const char point = '.';
const char oldString[] = ".old";
const char txtString[] = ".txt";

struct Date {
  unsigned int year;
  unsigned int month;
  unsigned int day;
};

struct FinancialFile {
  std::string name;
  std::string broker;
  std::string type;
  size_t account_number;
  Date date;
  std::string extension;
};

class Analyzer {
 public:
  explicit Analyzer(const fs::path& pathToFtp);
  void Start();
  void PrintAllFiles(std::ostream& out);
  void PrintAccountInfo(std::ostream& out);
  const fs::path& GetPathToFtp() const;

 private:
  fs::path pathToFtp;
  std::vector<FinancialFile> files;
  std::vector<std::string> directories;
  std::map<size_t, std::vector<FinancialFile>> accounts;

  void startAnalise(const fs::path& path);
  FinancialFile ParseFilename(std::string filename);
  void AnalyzeDir(const fs::path& path);
  void AnalyzeFile(const fs::path& path);
};

bool operator<(const FinancialFile& lhs, const FinancialFile& rhs);
std::ostream& operator<<(std::ostream& out, const Date& date);

#endif // INCLUDE_HEADER_HPP_
