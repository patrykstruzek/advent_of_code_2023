#include <algorithm>
#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

using namespace std;

using seed = long long;
struct Conversion;
using conversion_map = vector<Conversion>;

static auto read_inputs(const string& filename) -> string
{
    ifstream file {filename};
    if (!file)
    {
        throw runtime_error("File reading failure!");
    }
    ostringstream ss {};
    ss << file.rdbuf();
    return ss.str();
}

struct Conversion
{
    long long dest_rng {};
    long long src_rng {};
    long long rng_length {};
};

static auto parse_alamanac(const string& inputs) -> pair<vector<seed>, vector<conversion_map>>
{
    const auto get_seeds = [&inputs]()
    {
        istringstream iss_input {inputs};
        auto line = string {};

        constexpr string_view str_seed = "seeds: ";
        auto seeds = vector<long long> {};
        while (getline(iss_input, line))
        {
            if (size_t pos = line.find(str_seed); pos != string::npos)
            {
                istringstream iss_line {line.substr(pos + str_seed.size())};
                long long seed {};
                while (iss_line >> seed)
                {
                    seeds.push_back(seed);
                }
                break;
            }
        }

        return seeds;
    };

    const auto find_map = [&inputs](const string& map_name)
    {
        istringstream iss_input {inputs};
        auto line = string {};

        auto map = vector<Conversion> {};
        while (getline(iss_input, line))
        {
            if (line.find(map_name) != string::npos)
            {
                while (getline(iss_input, line) && !line.empty())
                {
                    istringstream iss_line {line};
                    Conversion c {};

                    if (!(iss_line >> c.dest_rng >> c.src_rng >> c.rng_length))
                    {
                        throw runtime_error("Error parsing line: " + line);
                    }

                    map.push_back(c);
                }

                break;
            }
        }

        return map;
    };

    const auto seeds = get_seeds();
    const auto sts_map = find_map("seed-to-soil map");
    const auto stf_map = find_map("soil-to-fertilizer map");
    const auto stw_map = find_map("fertilizer-to-water map");
    const auto wtl_map = find_map("water-to-light map");
    const auto ltt_map = find_map("light-to-temperature map");
    const auto tth_map = find_map("temperature-to-humidity map");
    const auto htl_map = find_map("humidity-to-location map");
    vector<vector<Conversion>> maps {sts_map, stf_map, stw_map, wtl_map, ltt_map, tth_map, htl_map};

    return pair {seeds, maps};
}

static auto part1(const string& inputs) -> int
{
    auto [seeds, maps] = parse_alamanac(inputs);

    vector<bool> seedMatched(seeds.size(), false);

    for (const auto& map : maps)
    {
        for (const auto& conv : map)
        {
            for (size_t i = 0; i < seeds.size(); ++i)
            {
                if (!seedMatched[i] && seeds[i] >= conv.src_rng &&
                    seeds[i] < (conv.src_rng + conv.rng_length))
                {
                    seeds[i] = conv.dest_rng + (seeds[i] - conv.src_rng);
                    seedMatched[i] = true;
                }
            }
        }
        fill(seedMatched.begin(), seedMatched.end(), false);
    }

    auto smallest = *std::min_element(seeds.begin(), seeds.end());

    return smallest;
}

static auto part2(const string& inputs) -> int {}

static auto run(const string& filename, string_view option)
{
    const auto inputs = read_inputs(filename);
    if (option == "-p1")
    {
        cout << "result: " << part1(inputs);
    }
    else if (option == "-p2")
    {
        cout << "result: " << part2(inputs);
    }
    else
    {
        throw runtime_error("Invalid arg!");
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