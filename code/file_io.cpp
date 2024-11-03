#include "file_io.hpp"

void write_line(const std::vector<double>& line , std::ofstream& fstream)
{
    for (size_t i = 0; i < line.size()-1; i++)
    {
        fstream << line[i] << " ";
    }

    fstream << line[line.size()-1] << "\n";

}