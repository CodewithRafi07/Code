//Weight for weight. sum of weights
// https://www.codewars.com/kata/55c6126177c9441a

#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cctype>
#include <iostream>

class WeightSort {
public:
    static std::string orderWeight(const std::string &strng) {
        std::vector<std::string> numbers;
        std::stringstream ss(strng);
        std::string temp;

        while (ss >> temp) {
            numbers.push_back(temp);
        }

        auto weight = [](const std::string &num) {
            int sum = 0;
            for (char c : num) sum += c - '0';
            return sum;
        };

        std::sort(numbers.begin(), numbers.end(), [&](const std::string &a, const std::string &b) {
            int wa = weight(a), wb = weight(b);
            return (wa == wb) ? a < b : wa < wb;
        });

        std::string result;
        for (size_t i = 0; i < numbers.size(); ++i) {
            result += numbers[i];
            if (i != numbers.size() - 1) result += " ";
        }

        return result;
    }
};

int main() {
    std::string input = "56 65 74 100 99 68 86 180 90";
    std::string output = WeightSort::orderWeight(input);

    std::cout << "Sorted by weight: " << output << std::endl;
    return 0;
}
