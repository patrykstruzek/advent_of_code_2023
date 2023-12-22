#include <algorithm>
#include <array>
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

using Seed = long long;
struct Conversion;
using conversion_map = vector<Conversion>;

static auto read_inputs(const string& filename) -> string {
   ifstream file{filename};
   if (!file) {
      throw runtime_error("File reading failure!");
   }

   ostringstream ss{};
   ss << file.rdbuf();
   return ss.str();
}

struct Conversion {
   long long dest_rng{};
   long long src_rng{};
   long long rng_length{};
};

static auto parse_alamanac(const string& inputs) -> pair<vector<Seed>, vector<conversion_map>> {
   const auto get_seeds = [&inputs]() {
      constexpr string_view str_seed = "seeds: ";

      auto seeds = vector<long long>{};
      istringstream iss_input{inputs};
      auto line = string{};

      while (getline(iss_input, line)) {
         if (size_t pos = line.find(str_seed); pos != string::npos) {
            istringstream iss_line{line.substr(pos + str_seed.size())};
            long long seed{};

            while (iss_line >> seed) {
               seeds.push_back(seed);
            }
            break;
         }
      }

      return seeds;
   };

   const auto find_map = [&inputs](string_view map_name) {
      istringstream iss_input{inputs};
      auto line = string{};

      auto map = vector<Conversion>{};
      while (getline(iss_input, line)) {
         if (line.find(map_name) != string::npos) {
            while (getline(iss_input, line) && !line.empty()) {
               istringstream iss_line{line};
               Conversion c{};

               if (!(iss_line >> c.dest_rng >> c.src_rng >> c.rng_length)) {
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

   constexpr auto map_names = array<string_view, 7>{
       "seed-to-soil map",        "soil-to-fertilizer map",   "fertilizer-to-water map",
       "water-to-light map",      "light-to-temperature map", "temperature-to-humidity map",
       "humidity-to-location map"};

   auto maps = vector<conversion_map>{};
   for (const auto& name : map_names) {
      maps.push_back(find_map(name));
   }

   return pair{seeds, maps};
}

static auto get_location_numbers(vector<Seed>& seeds, const vector<conversion_map>& maps)
    -> vector<Seed> {
   vector<bool> seed_matched(seeds.size(), false);

   const auto is_in_range = [&seed_matched, &seeds](size_t i, const Conversion& conv) {
      return !seed_matched[i] && seeds[i] >= conv.src_rng
             && seeds[i] < (conv.src_rng + conv.rng_length);
   };

   const auto process_conversion = [&seed_matched, &seeds, &is_in_range](const Conversion& conv) {
      for (size_t i = 0; i < seeds.size(); ++i) {
         if (is_in_range(i, conv)) {
            seeds[i] = conv.dest_rng + (seeds[i] - conv.src_rng);
            seed_matched[i] = true;
         }
      }
   };

   for (const auto& map : maps) {
      std::for_each(map.begin(), map.end(), process_conversion);
      std::fill(seed_matched.begin(), seed_matched.end(), false);
   }

   return seeds;
}

static auto part1(const string& inputs) -> int {
   auto [seeds, maps] = parse_alamanac(inputs);

   const auto location_values = get_location_numbers(seeds, maps);

   auto smallest = *std::min_element(seeds.begin(), seeds.end());

   return smallest;
}

static auto part2(const string& inputs) -> int {
   auto [seeds, maps] = parse_alamanac(inputs);

   const auto get_new_seeds = [&seeds]() {
      auto new_seeds = vector<Seed>{};
      for (size_t i = 0; i < seeds.size(); i += 2) {
         for (int j = seeds[i]; j < seeds[i + 1]; j++) {
            new_seeds.push_back(j);
         }
      }
      return new_seeds;
   };

   auto new_seeds = get_new_seeds();

   const auto location_values = get_location_numbers(new_seeds, maps);

   auto smallest = *std::min_element(seeds.begin(), seeds.end());

   return smallest;
}

static auto run(const string& filename, string_view option) {
   const auto inputs = read_inputs(filename);

   if (option == "-p1") {
      cout << "result: " << part1(inputs);
   } else if (option == "-p2") {
      cout << "result: " << part2(inputs);
   } else {
      throw runtime_error("Invalid arg!");
   }
}

int main(int argc, char* argv[]) {
   if (argc > 3 || argc < 3) {
      cerr << "Invalid args!";
      return 1;
   }

   const auto option = string{argv[1]};
   const auto filename = string{argv[2]};

   try {
      run(filename, option);
   } catch (const exception& e) {
      cerr << e.what() << endl;
   }
}