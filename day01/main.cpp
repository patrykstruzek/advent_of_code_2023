#include <iostream>
#include <fstream>
#include <string>
#include <vector>

auto read_inputs(std::string filename) -> std::vector<std::string>
{
    std::ifstream file {filename};
    if (!file)
    {
        throw std::runtime_error("Something goes wrong with reading the file!");
    }

    std::vector<std::string> inputs {};
    auto line = std::string {};
    while (std::getline(file, line))
    {
        inputs.push_back(line);
    }
    return inputs;
}

auto seek_for_calibration_value(std::string line) -> int
{
    std::vector<char> values {};
    for (auto c : line)
    {
        if (std::isdigit(c))
        {
            values.push_back(c);
        }
    }
    auto value = std::string{} +  values.front() + values.back();
    return std::stoi(value);
}

int main(int argc, char *argv[])
{
    if(argc > 2 || argc < 2)
    {
        std::cerr << "Invalid amount of args! You can only pass one file.";
        return 1;
    }
    std::string filename = argv[1];

    try
    {
        int result = 0;
        auto inputs = read_inputs(filename);
        for (auto line : inputs)
        {
            result += seek_for_calibration_value(line);
        }
        std::cout << "result: " << result;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}