// Copyright 2020 Your Name <your_email>

#include <header.hpp>

#include <algorithm>
#include <iomanip>
#include <tuple>

Analyzer::Analyzer(const fs::path& path) {
    if (path.empty()) {
        pathToFtp = fs::path{".."};
    } else {
        pathToFtp = path;
    }
}

void Analyzer::Start() { startAnalise(pathToFtp); }

void Analyzer::startAnalise(const fs::path& path) {
    for (const auto& iter : fs::directory_iterator{path}) {
        if (fs::is_regular_file(iter)) {
            AnalyzeFile(iter);
        } else if (fs::is_directory(iter)) {
            AnalyzeDir(iter);
        }
    }
    if (!directories.empty()) {
        directories.pop_back();
    }
}

FinancialFile Analyzer::ParseFilename(std::string filename) {
    FinancialFile file;

    file.name = filename;

    file.type = filename.substr(0, filename.find(underscores));
    filename = filename.substr(filename.find(underscores) + 1);

    file.account_number =
            std::stoi(filename.substr(0, filename.find(underscores)));
    filename = filename.substr(filename.find(underscores) + 1);

    file.date.year = std::stoi(filename.substr(0, 4));
    file.date.month = std::stoi(filename.substr(4, 2));
    file.date.day = std::stoi(filename.substr(6, 2));
    filename = filename.substr(8);

    if (filename[0] != point || filename.substr(0, 4) == oldString) {
        throw std::exception();
    } else if (filename.substr(1).find(point) != std::string::npos) {
        throw std::exception();
    }
    if (filename != txtString) {
        throw std::exception();
    }
    file.extension = filename;
    return file;
}

void Analyzer::AnalyzeFile(const fs::path& path) {
    try {
        FinancialFile file = ParseFilename(path.filename().string());
        file.broker = directories.back();
        files.emplace_back(file);
        accounts[file.account_number].emplace_back(file);
    } catch (std::exception& ex) {
    }
}

void Analyzer::AnalyzeDir(const fs::path& path) {
    directories.push_back(path.stem().string());
    startAnalise(path);
}

bool operator<(const FinancialFile& lhs, const FinancialFile& rhs) {
    return std::make_tuple(lhs.date.year, lhs.date.month, lhs.date.day) <
           std::make_tuple(rhs.date.year, rhs.date.month, rhs.date.day);
}

std::ostream& operator<<(std::ostream& out, const Date& date) {
    out << date.year << "-" << date.month << "-" << date.day;
    return out;
}

void Analyzer::PrintAllFiles(std::ostream& out) {
    for (const auto& file : files) out << file.broker << " " << file.name << '\n';
}

void Analyzer::PrintAccountInfo(std::ostream& out) {
    for (const auto& acc : accounts) {
        out << "broker: " << acc.second[0].broker << " ";
        out << " account: ";
        out << std::setw(8) << std::setfill('0') << acc.first << " ";
        out << " files: " << acc.second.size() << " ";
        out << " lastdate: "
            << std::max_element(acc.second.begin(), acc.second.end())->date << " \n";
    }
}

const fs::path& Analyzer::GetPathToFtp() const { return pathToFtp; }
