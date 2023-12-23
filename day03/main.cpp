#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

using namespace std;

static auto read_inputs(const string& filename) -> string {
   ifstream file{filename};
   if (!file) {
      throw runtime_error("File reading failure!");
   }

   stringstream ss{};
   ss << file.rdbuf();

   return ss.str();
}

struct number {
   using positions = pair<int, int>;
   int number{};
   positions pos{};
};

using chars_map = vector<vector<int>>;
using numbers_map = vector<vector<number>>;

pair<chars_map, numbers_map> parse_schematic(const string& inputs, function<bool(char)> checker) {
   istringstream ss{inputs};
   chars_map chars{};
   numbers_map numbers{};
   string line{};

   auto is_next_digit = [&line](int i) { return i + 1 < line.size() && isdigit(line[i + 1]); };

   while (getline(ss, line)) {
      chars.emplace_back();
      numbers.emplace_back();

      for (int i = 0; i < line.size(); ++i) {
         if (isdigit(line[i])) {
            number n{.pos.first = i};
            int digit = line[i] - '0';

            while (is_next_digit(i)) {
               digit = digit * 10 + (line[i + 1] - '0');
               ++i;
            }

            n.pos.second = i;
            n.number = digit;

            numbers.back().push_back(n);
         } else if (checker(line[i])) {
            chars.back().push_back(i);
         }
      }
   }

   return pair{chars, numbers};
}

static auto part1(const string& inputs) -> int {
   const auto [chars, numbers]
       = parse_schematic(inputs, [](char c) { return ispunct(c) && c != '.'; });

   auto isadjacent = [&chars](const number& n, int i) {
      auto check_adjacent = [&n](int p) { return p >= n.pos.first - 1 && p <= n.pos.second + 1; };

      if (i == 0) {
         return std::any_of(chars[i].begin(), chars[i].end(), check_adjacent)
                || std::any_of(chars[i + 1].begin(), chars[i + 1].end(), check_adjacent);
      } else if (i == chars.size() - 1) {
         return std::any_of(chars[i].begin(), chars[i].end(), check_adjacent)
                || std::any_of(chars[i - 1].begin(), chars[i - 1].end(), check_adjacent);
      } else {
         return std::any_of(chars[i - 1].begin(), chars[i - 1].end(), check_adjacent)
                || std::any_of(chars[i].begin(), chars[i].end(), check_adjacent)
                || std::any_of(chars[i + 1].begin(), chars[i + 1].end(), check_adjacent);
      }
   };

   int result = 0;
   for (int i = 0; i < numbers.size(); i++) {
      for (const auto& n : numbers[i]) {
         if (isadjacent(n, i)) {
            result += n.number;
         }
      }
   }

   return result;
}

static auto part2(const string& inputs) -> int {
   //    const auto schematic = parse_schematic(inputs, [](char c) { return c == '*'; });
   //    const chars_map chars{schematic.first};
   //    const numbers_map numbers{schematic.second};

   //    auto isadjacent = [&chars](const number& n, int i) {
   //       auto check_adjacent = [&n](int p) { return p >= n.pos.first - 1 && p <= n.pos.second +
   //       1; };

   //       if (i == 0) {
   //          return std::any_of(chars[i].begin(), chars[i].end(), check_adjacent)
   //                 || std::any_of(chars[i + 1].begin(), chars[i + 1].end(), check_adjacent);
   //       } else if (i == chars.size() - 1) {
   //          return std::any_of(chars[i].begin(), chars[i].end(), check_adjacent)
   //                 || std::any_of(chars[i - 1].begin(), chars[i - 1].end(), check_adjacent);
   //       } else {
   //          return std::any_of(chars[i - 1].begin(), chars[i - 1].end(), check_adjacent)
   //                 || std::any_of(chars[i].begin(), chars[i].end(), check_adjacent)
   //                 || std::any_of(chars[i + 1].begin(), chars[i + 1].end(), check_adjacent);
   //       }
   //    };

   //    int result = 0;
   //    for (int i = 0; i < numbers.size(); i++) {
   //       for (const auto& n : numbers[i]) {
   //          if (isadjacent(n, i)) {
   //             result += n.number;
   //          }
   //       }
   //    }

   //    return result;
   return 0;
}

static auto run(const string& filename, string_view option) {
   const auto inputs = read_inputs(filename);

   if (option == "-p1") {
      cout << "result: " << part1(inputs);
   } else if (option == "-p1") {
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