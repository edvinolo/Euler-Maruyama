#ifndef FILE_IO_HPP
#define FILE_IO_HPP

#include <fstream>
#include <vector>

void write_line(const std::vector<double> &, std::ofstream &);

void write_line_bn(std::vector<double>, std::ofstream &);

void write_group(const std::vector<double> &, const int, const int, std::ofstream &);

#endif //FILE_IO_HPP