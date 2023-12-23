#include <array>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

static auto read_inputs(const string& filename) -> vector<string> {
   ifstream file{filename};
   if (!file) {
      throw runtime_error("File reading failure!");
   }

   vector<string> inputs{};
   auto line = string{};
   while (getline(file, line)) {
      inputs.push_back(line);
   }
   return inputs;
}

// part 1
static auto get_cvalue1(string_view line) -> int {
   constexpr auto digits = "0123456789";

   const auto first = line.find_first_of(digits);
   const auto last = line.find_last_of(digits);

   return (line[first] - '0') * 10 + (line[last] - '0');
}

// part 2
static auto get_cvalue2(string_view line) -> int {
   int result{};

   constexpr auto digits = "0123456789";
   constexpr auto strdigits = array<string_view, 10>{"zero", "one", "two",   "three", "four",
                                                     "five", "six", "seven", "eight", "nine"};

   auto first = line.find_first_of(digits);

   size_t index{INTMAX_MAX};
   size_t number{};
   for (size_t i = 0; i < strdigits.size(); i++) {
      auto first2 = line.find(strdigits[i]);

      if (first2 < index && first2 != string::npos) {
         index = first2;
         number = i;
      }
   }

   if (index < first) {
      result += number * 10;
   } else {
      result += (line[first] - '0') * 10;
   }

   auto last = line.find_last_of(digits);

   index = 0;
   number = 0;
   for (std::size_t j = 0; j < strdigits.size(); j++) {
      auto last2 = line.rfind(strdigits[j]);

      if (index < last2 && last2 != string::npos) {
         index = last2;
         number = j;
      }
   }

   if (index > last) {
      result += number;
      return result;
   }

   result += line[last] - '0';
   return result;
}

static auto run(const string& filename, string_view option) {
   const auto inputs = read_inputs(filename);

   if (option == "-p1") {
      for (const auto& line : inputs) {
         cout << get_cvalue1(line);
      }
   } else if (option == "-p2") {
      for (const auto& line : inputs) {
         cout << get_cvalue2(line);
      }
   } else {
      throw runtime_error("Invalid arg!");
   }
}

int main(int argc, char* argv[]) {
   if (argc > 3 || argc < 3) {
      cerr << "Invalid amount of args!";
      return 1;
   }

   const auto filename = string{argv[2]};
   const auto option = string{argv[1]};

   try {
      run(filename, option);
   } catch (const exception& e) {
      cerr << e.what() << endl;
      return 1;
   }
}