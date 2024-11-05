#include "file_io.hpp"

void write_line(const std::vector<double>& line , std::ofstream& fstream)
{
    for (size_t i = 0; i < line.size()-1; i++)
    {
        fstream << line[i] << " ";
    }

    fstream << line[line.size()-1] << "\n";

}

void write_line_bn(std::vector<double> line, std::ofstream& fstream)
{
    for (size_t i = 0; i < line.size(); i++)
    {
        fstream.write(reinterpret_cast<char*>( &line[i]),sizeof(double));
    }
}

void write_group(const std::vector<double> & groups, const int group_size, const int N_groups, std::ofstream& fstream)
{
    for (size_t i = 0; i < N_groups; i++)
    {
        for (size_t j = 0; j < group_size-1; j++)
        {
            fstream << groups[i*group_size + j] << " ";
        }

        fstream << groups[(i+1)*group_size-1] << "\n";
    }
}