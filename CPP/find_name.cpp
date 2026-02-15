#include <iostream>
#include <string>
#include <curl/curl.h>
#include "json.hpp" // Download from https://github.com/nlohmann/json

using json = nlohmann::json;

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    output->append((char*)contents, size * nmemb);
    return size * nmemb;
}

void predictGender(const std::string& name) {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    std::string url = "https://api.genderize.io?name=" + name;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (res == CURLE_OK) {
            json result = json::parse(readBuffer);
            if (!result["gender"].is_null()) {
                std::string gender = result["gender"];
                double probability = result["probability"];
                std::cout << "The name '" << name << "' is likely " << gender
                          << " (confidence: " << probability * 100 << "%)" << std::endl;
            } else {
                std::cout << "The name '" << name << "' could not be classified." << std::endl;
            }
        } else {
            std::cerr << "Error fetching data from API." << std::endl;
        }
    }
}

int main() {
    std::string name;
    std::cout << "Enter a name: ";
    std::cin >> name;
    predictGender(name);
    return 0;
}
