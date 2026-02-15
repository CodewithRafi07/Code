#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <cctype>
#include <sstream>
#include <ctime>

using namespace std;

// 🔹 Global helper for lowercase conversion (accessible from main)
string toLower(const string& str) {
    string result = str;
    transform(result.begin(), result.end(), result.begin(),
              [](unsigned char c){ return tolower(c); });
    return result;
}

class OmniBot {
private:
    enum class Mode {
        PC_BUILD,
        HEALTH_WELLNESS,
        TEXT_REFINEMENT,
        SIMPLE_GAME,
        NONE
    };

    Mode currentMode = Mode::NONE;
    string gameState;
    string gameAnswer;
    int gameQuestionsLeft;

    map<string, vector<string>> pcComponents = {
        {"GPU", {"NVIDIA RTX 3060", "AMD RX 6600", "NVIDIA RTX 4060", "AMD RX 7600"}},
        {"CPU", {"Intel i5-13400", "AMD Ryzen 5 7600", "Intel i5-13600K", "AMD Ryzen 7 7700"}},
        {"Motherboard", {"ASUS B760-PLUS", "MSI PRO B760", "Gigabyte B650 AORUS ELITE", "ASRock B650 PG"}},
        {"RAM", {"16GB DDR4 3200MHz", "16GB DDR5 5200MHz", "32GB DDR4 3600MHz", "32GB DDR5 5600MHz"}},
        {"Storage", {"1TB NVMe SSD", "2TB NVMe SSD", "500GB NVMe SSD + 2TB HDD", "1TB SATA SSD"}},
        {"PSU", {"650W 80+ Bronze", "750W 80+ Gold", "850W 80+ Gold", "1000W 80+ Platinum"}},
        {"Case", {"Mid-Tower ATX", "Compact Micro-ATX", "Full-Tower EATX", "Small Form Factor ITX"}}
    };

    bool isHealthRelated(const string& input) {
        vector<string> keywords = {"headache", "pain", "tired", "fatigue", "symptom", "health", "wellness",
                                   "diet", "exercise", "sleep", "medication", "fever", "nausea", "dizzy"};
        string lowerInput = toLower(input);
        for (const auto& keyword : keywords)
            if (lowerInput.find(keyword) != string::npos) return true;
        return false;
    }

    bool isPCBuildRelated(const string& input) {
        vector<string> keywords = {"pc", "computer", "build", "gpu", "cpu", "motherboard", "ram", "storage",
                                   "psu", "case", "graphics card", "budget", "gaming", "video editing"};
        string lowerInput = toLower(input);
        for (const auto& keyword : keywords)
            if (lowerInput.find(keyword) != string::npos) return true;
        return false;
    }

    bool isTextRefinementRelated(const string& input) {
        vector<string> keywords = {"fix", "correct", "autocorrect", "grammar", "spelling", "refine", "improve text"};
        string lowerInput = toLower(input);
        for (const auto& keyword : keywords)
            if (lowerInput.find(keyword) != string::npos) return true;
        return false;
    }

    bool isGameRelated(const string& input) {
        vector<string> keywords = {"game", "play", "riddle", "puzzle", "fun", "entertainment", "20 questions"};
        string lowerInput = toLower(input);
        for (const auto& keyword : keywords)
            if (lowerInput.find(keyword) != string::npos) return true;
        return false;
    }

    string autocorrect(const string& input) {
        string corrected = input;
        map<string, string> corrections = {
            {"headche", "headache"}, {"tierd", "tired"}, {"recieve", "receive"},
            {"occured", "occurred"}, {"seperate", "separate"}, {"definately", "definitely"},
            {"existance", "existence"}, {"recieved", "received"}
        };

        for (const auto& pair : corrections) {
            size_t pos = 0;
            while ((pos = corrected.find(pair.first, pos)) != string::npos) {
                corrected.replace(pos, pair.first.length(), pair.second);
                pos += pair.second.length();
            }
        }
        return corrected;
    }

    void processHealth(const string& input) {
        cout << "⚕️ Important Medical Disclaimer: I'm an AI, not a medical professional.\n\n";
        string lowerInput = toLower(input);

        if (lowerInput.find("headache") != string::npos && lowerInput.find("tired") != string::npos) {
            cout << "You mentioned headache and tiredness. Possible reasons:\n"
                 << "- Dehydration\n- Lack of sleep\n- Stress\n- Too much screen time\n";
        } else if (lowerInput.find("sleep") != string::npos) {
            cout << "Sleep tips:\n- Keep consistent schedule\n- Avoid caffeine late\n- Reduce screen use before bed\n";
        } else {
            cout << "General wellness tips:\n- Stay hydrated\n- Eat balanced diet\n- Get regular exercise\n";
        }
    }

    void processPCBuild(const string& input) {
        string lowerInput = toLower(input);
        cout << "🖥️ PC Build Recommendation:\n";
        cout << "CPU: " << pcComponents["CPU"][0] << "\nGPU: " << pcComponents["GPU"][0]
             << "\nRAM: " << pcComponents["RAM"][0] << "\nStorage: " << pcComponents["Storage"][0] << "\n";
    }

    void processTextRefinement(const string& input) {
        string corrected = autocorrect(input);
        if (corrected != input) {
            cout << "Original: " << input << "\nCorrected: " << corrected << "\n";
        } else {
            cout << "No spelling errors found.\n";
        }
    }

    void processGame(const string& input) {
        string lowerInput = toLower(input);
        if (lowerInput.find("riddle") != string::npos) {
            vector<string> riddles = {
                "What has keys but can't open locks? (Answer: Piano)",
                "The more you take, the more you leave behind. (Answer: Footsteps)"
            };
            cout << riddles[rand() % riddles.size()] << "\n";
        } else {
            cout << "Let's play a simple guessing game next time!\n";
        }
    }

    void askForClarification() {
        cout << "I'm not sure what you meant. I can help with:\n"
             << "- PC building 🖥️\n- Health info ⚕️\n- Grammar check ✍️\n- Simple games 🎮\n";
    }

public:
    OmniBot() {
        srand(time(nullptr));
    }

    void processInput(const string& input) {
        if (input.empty()) return;
        if (toLower(input) == "exit" || toLower(input) == "quit") {
            cout << "Goodbye! 👋\n";
            return;
        }

        Mode detectedMode = Mode::NONE;
        if (isHealthRelated(input)) detectedMode = Mode::HEALTH_WELLNESS;
        else if (isPCBuildRelated(input)) detectedMode = Mode::PC_BUILD;
        else if (isTextRefinementRelated(input)) detectedMode = Mode::TEXT_REFINEMENT;
        else if (isGameRelated(input)) detectedMode = Mode::SIMPLE_GAME;

        currentMode = detectedMode;

        switch (currentMode) {
            case Mode::HEALTH_WELLNESS: processHealth(input); break;
            case Mode::PC_BUILD: processPCBuild(input); break;
            case Mode::TEXT_REFINEMENT: processTextRefinement(input); break;
            case Mode::SIMPLE_GAME: processGame(input); break;
            default: askForClarification(); break;
        }
    }
};

int main() {
    OmniBot bot;
    string input;

    cout << "Hello! I'm Omni-Bot 🤖\n"
         << "I can help with PC builds, health info, grammar check, and simple games.\n"
         << "Type 'exit' to quit.\n";

    while (true) {
        cout << "\n> ";
        getline(cin, input);

        if (toLower(input) == "exit" || toLower(input) == "quit")
            break;

        bot.processInput(input);
    }

    return 0;
}
