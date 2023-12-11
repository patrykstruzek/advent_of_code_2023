#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

using namespace std;

static auto read_inputs(const string& filename) -> string
{
    ifstream file {filename};
    if (!file)
    {
        throw runtime_error("File reading failure!");
    }
    stringstream ss {};
    ss << file.rdbuf();
    return std::move(ss.str());
}

struct number
{
    using positions = pair<int, int>;
    int number {};
    positions pos {};
};

static auto part1(const string& inputs) -> int
{
    istringstream ss {inputs};
    vector<vector<int>> chars {};
    vector<vector<number>> numbers {};
    string line {};

    while (getline(ss, line))
    {
        chars.emplace_back();
        numbers.emplace_back();
        for (int i = 0; i < line.size(); ++i)
        {
            if (isdigit(line[i]))
            {
                number n {.pos.first = i};
                int digit = line[i] - '0';
                while (i + 1 < line.size() && isdigit(line[i + 1]))
                {
                    digit = digit * 10 + (line[i + 1] - '0');
                    ++i;
                }
                n.pos.second = i;
                n.number = digit;
                numbers.back().push_back(n);
            }
            else if (ispunct(line[i]) && line[i] != '.')
            {
                chars.back().push_back(i);
            }
        }
    }

    auto isadjacent = [&chars](const number& n, int i)
    {
        auto check_adjacent = [&n](int p) { return p >= n.pos.first - 1 && p <= n.pos.second + 1; };

        if (i == 0)
        {
            return std::any_of(chars[i].begin(), chars[i].end(), check_adjacent) ||
                   std::any_of(chars[i + 1].begin(), chars[i + 1].end(), check_adjacent);
        }
        else if (i == chars.size() - 1)
        {
            return std::any_of(chars[i].begin(), chars[i].end(), check_adjacent) ||
                   std::any_of(chars[i - 1].begin(), chars[i - 1].end(), check_adjacent);
        }
        else
        {
            return std::any_of(chars[i - 1].begin(), chars[i - 1].end(), check_adjacent) ||
                   std::any_of(chars[i].begin(), chars[i].end(), check_adjacent) ||
                   std::any_of(chars[i + 1].begin(), chars[i + 1].end(), check_adjacent);
        }
    };

    int result = 0;
    for (int i = 0; i < numbers.size(); i++)
    {
        for (const auto& n : numbers[i])
        {
            if (isadjacent(n, i))
            {
                result += n.number;
            }
        }
    }

    return result;
}

static auto run(const string& filename, string_view option)
{
    const auto inputs = read_inputs(filename);
    if (option == "-p1")
    {
        cout << "result: " << part1(inputs);
    }
    else
    {
    }
}

int main(int argc, char* argv[])
{
    if (argc > 3 || argc < 3)
    {
        cerr << "Invalid args!";
        return 1;
    }
    const auto option = string {argv[1]};
    const auto filename = string {argv[2]};

    try
    {
        run(filename, option);
    }
    catch (const exception& e)
    {
        cerr << e.what() << endl;
    }
}