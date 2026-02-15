#include <array>
#include <string>
#include <sstream>
#include <iomanip>
#include <map>
#include <algorithm>
#include <cctype>

using namespace std;

string driver(const array<string, 5> &data)
{
    string forename = data[0];
    string middlename = data[1];
    string surname = data[2];
    string dob = data[3];
    string gender = data[4];

    // Step 1: Surname (uppercase, first 5 characters, padded with '9')
    string surname_code = surname;
    transform(surname_code.begin(), surname_code.end(), surname_code.begin(), ::toupper);
    surname_code = surname_code.substr(0, 5);
    while (surname_code.size() < 5) surname_code += '9';

    // Step 2: Parse date of birth (format can be like "13-Dec-1981" or "02-September-1981")
    stringstream ss(dob);
    string day_str, month_str, year_str;
    getline(ss, day_str, '-');
    getline(ss, month_str, '-');
    getline(ss, year_str, '-');

    // Normalize month string to uppercase
    transform(month_str.begin(), month_str.end(), month_str.begin(), ::toupper);

    // Month name map (supports both short and full forms)
    map<string, string> month_map = {
        {"JAN", "01"}, {"FEB", "02"}, {"MAR", "03"}, {"APR", "04"},
        {"MAY", "05"}, {"JUN", "06"}, {"JUL", "07"}, {"AUG", "08"},
        {"SEP", "09"}, {"SEPTEMBER", "09"},
        {"OCT", "10"}, {"NOV", "11"}, {"DEC", "12"}, {"DECEMBER", "12"},
        {"JANUARY", "01"}, {"FEBRUARY", "02"}, {"MARCH", "03"}, {"APRIL", "04"},
        {"JUNE", "06"}, {"JULY", "07"}, {"AUGUST", "08"}, {"NOVEMBER", "11"}, {"OCTOBER", "10"}, {"DECEMBER", "12"}
    };

    string month_num = month_map[month_str.substr(0, 3)];
    if (gender == "F") {
        int mn = stoi(month_num) + 50;
        month_num = (mn < 10 ? "0" : "") + to_string(mn);
    }

    // Step 3: Extract year digits
    char decade_digit = year_str[2];
    char year_digit = year_str[3];

    // Step 4: Day (pad to 2 digits)
    stringstream day_ss;
    day_ss << setw(2) << setfill('0') << day_str;
    string day = day_ss.str();

    // Step 5: First and middle initials
    char first_initial = toupper(forename[0]);
    char middle_initial = middlename.empty() ? '9' : toupper(middlename[0]);

    // Final license number
    string license = "";
    license += surname_code;
    license += decade_digit;
    license += month_num;
    license += day;
    license += year_digit;
    license += first_initial;
    license += middle_initial;
    license += '9';  // Arbitrary digit
    license += "AA"; // Check digits

    return license;
}
