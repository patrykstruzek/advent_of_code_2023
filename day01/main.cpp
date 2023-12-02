#include <vcruntime.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

static auto read_inputs(const std::string& filename) -> std::vector<std::string>
{
    std::ifstream file {filename};
    if (!file)
    {
        throw std::runtime_error("File reading failure!");
    }

    std::vector<std::string> inputs {};
    auto line = std::string {};
    while (std::getline(file, line))
    {
        inputs.push_back(line);
    }
    return std::move(inputs);
}

// part 1
static auto seek_for_cvalue_1(const std::string& line) -> int
{
    std::vector<char> values {};
    for (const auto& c : line)
    {
        if (std::isdigit(c))
        {
            values.push_back(c);
        }
    }
    auto value = std::string {} + values.front() + values.back();
    return std::stoi(std::move(value));
}

// part 2
static auto seek_for_cvalue_2(const std::string& line) -> int
{
    const std::unordered_map<std::string, char> numbers = {
        {"one", 1}, {"two", 2},   {"three", 3}, {"four", 4}, {"five", 5},
        {"six", 6}, {"seven", 7}, {"eight", 8}, {"nine", 9},
    };

    auto result = std::pair<int, int> {};

    size_t lborder_distance {};
    size_t rborder_distance {};
    for (const auto& [alpha, digit] : numbers)
    {
        size_t pos = line.find(alpha);
        if (pos != std::string::npos && pos < lborder_distance)
        {
            lborder_distance = pos;
            result.first = digit;
        }

        pos = line.rfind(alpha);
        if (pos != std::string::npos && pos > lborder_distance)
        {
            rborder_distance = pos;
            result.second = digit;
        }
    }

    return 10 * result.first + result.second;
}

static auto run(const std::string& filename, std::string_view option) -> bool
{
    int result {};
    const auto inputs = read_inputs(filename);
    if (option == "-p1")
    {
        for (const auto& line : inputs)
        {
            result += seek_for_cvalue_1(line);
        }
    }
    else if (option == "-p2")
    {
        for (const auto& line : inputs)
        {
            result += seek_for_cvalue_2(line);
        }
    }
    else
    {
        std::cerr << "Invalid arg!";
        return false;
    }
    std::cout << "result: " << result;
    return true;
}

int main(int argc, char* argv[])
{
    if (argc > 3 || argc < 3)
    {
        std::cerr << "Invalid amount of args! You can only pass one arg and one file.";
        return 1;
    }
    const auto filename = std::string{argv[2]};
    const auto option = std::string{argv[1]};

    try
    {
        return run(filename, option);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}