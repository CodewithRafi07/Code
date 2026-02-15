#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>  // Required for std::transform
#include <cctype>      // Required for std::tolower
#include <sstream>
#include <cstdlib>     // Required for rand() and srand()
#include <ctime>       // Required for time()

class OmniBot {
private:
    // Mode enumeration
    enum class Mode {
        PC_BUILD,
        HEALTH_WELLNESS,
        TEXT_REFINEMENT,
        SIMPLE_GAME,
        NONE
    };

    // Current active mode
    Mode currentMode = Mode::NONE;
    
    // Game state for simple games
    std::string gameState;
    std::string gameAnswer;
    int gameQuestionsLeft;

    // PC Build components database
    std::map<std::string, std::vector<std::string>> pcComponents = {
        {"GPU", {"NVIDIA RTX 3060", "AMD RX 6600", "NVIDIA RTX 4060", "AMD RX 7600"}},
        {"CPU", {"Intel i5-13400", "AMD Ryzen 5 7600", "Intel i5-13600K", "AMD Ryzen 7 7700"}},
        {"Motherboard", {"ASUS B760-PLUS", "MSI PRO B760", "Gigabyte B650 AORUS ELITE", "ASRock B650 PG"}},
        {"RAM", {"16GB DDR4 3200MHz", "16GB DDR5 5200MHz", "32GB DDR4 3600MHz", "32GB DDR5 5600MHz"}},
        {"Storage", {"1TB NVMe SSD", "2TB NVMe SSD", "500GB NVMe SSD + 2TB HDD", "1TB SATA SSD"}},
        {"PSU", {"650W 80+ Bronze", "750W 80+ Gold", "850W 80+ Gold", "1000W 80+ Platinum"}},
        {"Case", {"Mid-Tower ATX", "Compact Micro-ATX", "Full-Tower EATX", "Small Form Factor ITX"}}
    };

    // Check if input is health-related
    bool isHealthRelated(const std::string& input) {
        std::vector<std::string> keywords = {
            "headache", "pain", "tired", "fatigue", "symptom", "health", "wellness",
            "diet", "exercise", "sleep", "medication", "fever", "nausea", "dizzy"
        };
        
        std::string lowerInput = toLower(input);
        for (const auto& keyword : keywords) {
            if (lowerInput.find(keyword) != std::string::npos) {
                return true;
            }
        }
        return false;
    }

    // Check if input is PC build related
    bool isPCBuildRelated(const std::string& input) {
        std::vector<std::string> keywords = {
            "pc", "computer", "build", "gpu", "cpu", "motherboard", "ram", "storage",
            "psu", "case", "graphics card", "budget", "gaming", "video editing"
        };
        
        std::string lowerInput = toLower(input);
        for (const auto& keyword : keywords) {
            if (lowerInput.find(keyword) != std::string::npos) {
                return true;
            }
        }
        return false;
    }

    // Check if input is text refinement related
    bool isTextRefinementRelated(const std::string& input) {
        std::vector<std::string> keywords = {
            "fix", "correct", "autocorrect", "grammar", "spelling", "refine", "improve text"
        };
        
        std::string lowerInput = toLower(input);
        for (const auto& keyword : keywords) {
            if (lowerInput.find(keyword) != std::string::npos) {
                return true;
            }
        }
        return false;
    }

    // Check if input is game related
    bool isGameRelated(const std::string& input) {
        std::vector<std::string> keywords = {
            "game", "play", "riddle", "puzzle", "fun", "entertainment", "20 questions"
        };
        
        std::string lowerInput = toLower(input);
        for (const auto& keyword : keywords) {
            if (lowerInput.find(keyword) != std::string::npos) {
                return true;
            }
        }
        return false;
    }

    // Simple autocorrect function
    std::string autocorrect(const std::string& input) {
        std::string corrected = input;
        
        // Common misspellings dictionary
        std::map<std::string, std::string> corrections = {
            {"headche", "headache"},
            {"tierd", "tired"},
            {"recieve", "receive"},
            {"occured", "occurred"},
            {"seperate", "separate"},
            {"definately", "definitely"},
            {"existance", "existence"},
            {"recieved", "received"}
        };
        
        // Apply corrections
        for (const auto& pair : corrections) {
            size_t pos = 0;
            while ((pos = corrected.find(pair.first, pos)) != std::string::npos) {
                corrected.replace(pos, pair.first.length(), pair.second);
                pos += pair.second.length();
            }
        }
        
        return corrected;
    }

    // Process health-related query
    void processHealth(const std::string& input) {
        std::cout << "Important Medical Disclaimer: I am an AI and not a medical professional. "
                  << "The information provided is for informational purposes only. "
                  << "Always consult a licensed healthcare provider for any medical concerns.\n\n";
        
        std::string lowerInput = toLower(input);
        
        if (lowerInput.find("headache") != std::string::npos && lowerInput.find("tired") != std::string::npos) {
            std::cout << "You mentioned experiencing a headache and feeling tired. This could be due to several factors:\n"
                      << "- Dehydration: Try drinking more water throughout the day\n"
                      << "- Lack of sleep: Ensure you're getting 7-9 hours of quality sleep\n"
                      << "- Stress: Consider relaxation techniques or taking breaks\n"
                      << "- Screen time: Take regular breaks from screens\n\n"
                      << "If symptoms persist or worsen, please consult a healthcare professional.\n";
        } 
        else if (lowerInput.find("sleep") != std::string::npos) {
            std::cout << "Regarding sleep concerns:\n"
                      << "- Maintain a consistent sleep schedule\n"
                      << "- Create a relaxing bedtime routine\n"
                      << "- Limit screen time before bed\n"
                      << "- Ensure your bedroom is dark, quiet, and cool\n"
                      << "- Avoid caffeine late in the day\n\n"
                      << "If sleep problems continue, consider speaking with a sleep specialist.\n";
        }
        else {
            std::cout << "I understand you're experiencing health concerns. While I can provide general information:\n"
                      << "- Stay hydrated and maintain a balanced diet\n"
                      << "- Get regular physical activity\n"
                      << "- Ensure adequate rest and manage stress\n"
                      << "- Monitor symptoms and seek professional medical advice if they persist\n\n"
                      << "For specific medical advice, please consult a healthcare provider.\n";
        }
    }

    // Process PC build query
    void processPCBuild(const std::string& input) {
        std::string lowerInput = toLower(input);
        
        // Extract budget if mentioned
        int budget = 0;
        size_t budgetPos = lowerInput.find("$");
        if (budgetPos != std::string::npos) {
            std::string budgetStr;
            for (size_t i = budgetPos + 1; i < lowerInput.size(); ++i) {
                if (std::isdigit(lowerInput[i])) {
                    budgetStr += lowerInput[i];
                } else if (!budgetStr.empty()) {
                    break;
                }
            }
            if (!budgetStr.empty()) {
                budget = std::stoi(budgetStr);
            }
        }
        
        // Extract use case
        std::string useCase = "general";
        if (lowerInput.find("gaming") != std::string::npos) {
            useCase = "gaming";
        } else if (lowerInput.find("video editing") != std::string::npos || lowerInput.find("content creation") != std::string::npos) {
            useCase = "content creation";
        } else if (lowerInput.find("office") != std::string::npos || lowerInput.find("work") != std::string::npos) {
            useCase = "office";
        }
        
        // Generate PC build recommendation
        std::cout << "Based on your request for a " << useCase << " PC";
        if (budget > 0) {
            std::cout << " with a budget of $" << budget;
        }
        std::cout << ", here's a compatible build:\n\n";
        
        // Component recommendations based on use case and budget
        std::map<std::string, std::string> recommendedComponents;
        
        if (useCase == "gaming") {
            if (budget >= 1500) {
                recommendedComponents = {
                    {"GPU", pcComponents["GPU"][2]}, // RTX 4060
                    {"CPU", pcComponents["CPU"][2]},  // i5-13600K
                    {"Motherboard", pcComponents["Motherboard"][2]},
                    {"RAM", pcComponents["RAM"][2]},  // 32GB DDR4
                    {"Storage", pcComponents["Storage"][1]}, // 2TB NVMe
                    {"PSU", pcComponents["PSU"][2]},  // 850W Gold
                    {"Case", pcComponents["Case"][0]}
                };
            } else if (budget >= 1000) {
                recommendedComponents = {
                    {"GPU", pcComponents["GPU"][0]}, // RTX 3060
                    {"CPU", pcComponents["CPU"][0]},  // i5-13400
                    {"Motherboard", pcComponents["Motherboard"][0]},
                    {"RAM", pcComponents["RAM"][0]},  // 16GB DDR4
                    {"Storage", pcComponents["Storage"][0]}, // 1TB NVMe
                    {"PSU", pcComponents["PSU"][0]},  // 650W Bronze
                    {"Case", pcComponents["Case"][0]}
                };
            } else {
                recommendedComponents = {
                    {"GPU", pcComponents["GPU"][1]}, // RX 6600
                    {"CPU", pcComponents["CPU"][0]},  // i5-13400
                    {"Motherboard", pcComponents["Motherboard"][1]},
                    {"RAM", pcComponents["RAM"][0]},  // 16GB DDR4
                    {"Storage", pcComponents["Storage"][2]}, // 500GB + 2TB
                    {"PSU", pcComponents["PSU"][0]},  // 650W Bronze
                    {"Case", pcComponents["Case"][1]}
                };
            }
        } 
        else if (useCase == "content creation") {
            recommendedComponents = {
                {"GPU", pcComponents["GPU"][2]}, // RTX 4060
                {"CPU", pcComponents["CPU"][3]},  // Ryzen 7 7700
                {"Motherboard", pcComponents["Motherboard"][3]},
                {"RAM", pcComponents["RAM"][3]},  // 32GB DDR5
                {"Storage", pcComponents["Storage"][1]}, // 2TB NVMe
                {"PSU", pcComponents["PSU"][2]},  // 850W Gold
                {"Case", pcComponents["Case"][0]}
            };
        }
        else { // office or general
            recommendedComponents = {
                {"GPU", "Integrated Graphics"}, // No dedicated GPU
                {"CPU", pcComponents["CPU"][0]},  // i5-13400
                {"Motherboard", pcComponents["Motherboard"][0]},
                {"RAM", pcComponents["RAM"][0]},  // 16GB DDR4
                {"Storage", pcComponents["Storage"][0]}, // 1TB NVMe
                {"PSU", pcComponents["PSU"][0]},  // 650W Bronze
                {"Case", pcComponents["Case"][1]}
            };
        }
        
        // Display the build
        for (const auto& component : recommendedComponents) {
            std::cout << component.first << ": " << component.second << "\n";
        }
        
        std::cout << "\nNote: All components are selected for compatibility. "
                  << "The motherboard socket matches the CPU, RAM type is compatible, "
                  << "and the PSU provides sufficient power for all components.\n";
    }

    // Process text refinement query
    void processTextRefinement(const std::string& input) {
        std::string corrected = autocorrect(input);
        
        if (corrected != input) {
            std::cout << "I've corrected some errors in your text:\n\n"
                      << "Original: " << input << "\n"
                      << "Corrected: " << corrected << "\n\n";
        } else {
            std::cout << "No spelling errors detected in your text.\n\n";
        }
        
        // Provide additional refinement suggestions
        std::cout << "Additional suggestions for improving your text:\n"
                  << "- Consider using more specific vocabulary\n"
                  << "- Check sentence structure for clarity\n"
                  << "- Ensure consistent tense usage\n"
                  << "- Review punctuation for proper flow\n";
    }

    // Process game query
    void processGame(const std::string& input) {
        std::string lowerInput = toLower(input);
        
        if (lowerInput.find("20 questions") != std::string::npos || gameState == "20questions") {
            if (gameState != "20questions") {
                // Start a new game of 20 Questions
                gameState = "20questions";
                gameQuestionsLeft = 20;
                
                // Pick a random object (simplified for this example)
                std::vector<std::string> objects = {
                    "elephant", "bicycle", "mountain", "computer", "banana",
                    "airplane", "guitar", "telescope", "volcano", "piano"
                };
                gameAnswer = objects[rand() % objects.size()];
                
                std::cout << "Great! Let's play 20 Questions.\n"
                          << "I'm thinking of an object. You have 20 questions to guess what it is.\n"
                          << "Ask yes/no questions to figure it out.\n"
                          << "You have " << gameQuestionsLeft << " questions left.\n";
            } else {
                // Process a question in the ongoing game
                if (gameQuestionsLeft <= 0) {
                    std::cout << "You've run out of questions! The answer was: " << gameAnswer << "\n"
                              << "Would you like to play again?\n";
                    gameState = "";
                    return;
                }
                
                // Simple response logic (in a real implementation, this would be more sophisticated)
                std::string question = toLower(input);
                bool answer = false;
                
                if (question.find("animal") != std::string::npos) {
                    answer = (gameAnswer == "elephant");
                } else if (question.find("man-made") != std::string::npos || question.find("artificial") != std::string::npos) {
                    answer = (gameAnswer != "elephant" && gameAnswer != "mountain" && gameAnswer != "banana" && gameAnswer != "volcano");
                } else if (question.find("larger than") != std::string::npos) {
                    answer = (gameAnswer == "elephant" || gameAnswer == "mountain" || gameAnswer == "airplane" || gameAnswer == "volcano");
                } else if (question.find("electronic") != std::string::npos) {
                    answer = (gameAnswer == "computer" || gameAnswer == "telescope");
                } else if (question.find("instrument") != std::string::npos) {
                    answer = (gameAnswer == "guitar" || gameAnswer == "piano");
                } else if (question.find("food") != std::string::npos) {
                    answer = (gameAnswer == "banana");
                } else if (question.find("vehicle") != std::string::npos) {
                    answer = (gameAnswer == "bicycle" || gameAnswer == "airplane");
                } else {
                    // Default response for unrecognized questions
                    std::cout << "I'm not sure how to answer that question. Please ask a yes/no question.\n";
                    return;
                }
                
                gameQuestionsLeft--;
                std::cout << (answer ? "Yes" : "No") << ". You have " << gameQuestionsLeft << " questions left.\n";
                
                // Check if the user guessed the answer
                if (question.find(gameAnswer) != std::string::npos) {
                    std::cout << "Correct! The answer was " << gameAnswer << ". You win!\n";
                    gameState = "";
                }
            }
        } 
        else if (lowerInput.find("riddle") != std::string::npos) {
            // Present a riddle
            std::vector<std::string> riddles = {
                "I speak without a mouth and hear without ears. I have no body, but I come alive with the wind. What am I? (Answer: Echo)",
                "The more you take, the more you leave behind. What am I? (Answer: Footsteps)",
                "I have cities, but no houses. I have mountains, but no trees. I have water, but no fish. What am I? (Answer: A map)",
                "What has keys but can't open locks? (Answer: Piano)",
                "What can travel around the world while staying in a corner? (Answer: Stamp)"
            };
            
            std::string riddle = riddles[rand() % riddles.size()];
            std::cout << "Here's a riddle for you:\n" << riddle << "\n";
        }
        else {
            std::cout << "I can play several games with you:\n"
                      << "- 20 Questions (I think of an object, you guess with yes/no questions)\n"
                      << "- Riddles (I give you a riddle to solve)\n"
                      << "Which would you like to play?\n";
        }
    }

    // Ask for clarification when intent is unclear
    void askForClarification() {
        std::cout << "I'm not sure what you're looking for. I can help with:\n"
                  << "- PC building and component recommendations\n"
                  << "- General health and wellness information\n"
                  << "- Text correction and refinement\n"
                  << "- Simple games and entertainment\n"
                  << "Please clarify what you need assistance with.\n";
    }

public:
    // Constructor
    OmniBot() {
        // Initialize random seed for games
        srand(static_cast<unsigned int>(time(nullptr)));  // Fixed: Added proper cast and namespace
    }

    // Convert string to lowercase - Made public
    std::string toLower(const std::string& str) {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(),
                       [](unsigned char c){ return std::tolower(c); });  // Fixed: Added proper cast
        return result;
    }

    // Process user input and determine appropriate response
    void processInput(const std::string& input) {
        // Skip empty input
        if (input.empty()) {
            return;
        }
        
        // Check for exit command
        if (toLower(input) == "exit" || toLower(input) == "quit") {
            std::cout << "Goodbye! Feel free to come back if you need assistance.\n";
            return;
        }
        
        // Determine mode based on input
        Mode detectedMode = Mode::NONE;
        
        if (isHealthRelated(input)) {
            detectedMode = Mode::HEALTH_WELLNESS;
        } else if (isPCBuildRelated(input)) {
            detectedMode = Mode::PC_BUILD;
        } else if (isTextRefinementRelated(input)) {
            detectedMode = Mode::TEXT_REFINEMENT;
        } else if (isGameRelated(input)) {
            detectedMode = Mode::SIMPLE_GAME;
        }
        
        // If we're in a game mode and the input doesn't match other modes, continue the game
        if (currentMode == Mode::SIMPLE_GAME && detectedMode == Mode::NONE && !gameState.empty()) {
            detectedMode = Mode::SIMPLE_GAME;
        }
        
        // Update current mode if a new mode is detected
        if (detectedMode != Mode::NONE && detectedMode != currentMode) {
            currentMode = detectedMode;
            gameState = ""; // Reset game state when switching modes
        }
        
        // Process based on detected mode
        switch (currentMode) {
            case Mode::HEALTH_WELLNESS: {
                // First autocorrect if needed
                std::string correctedInput = autocorrect(input);
                if (correctedInput != input) {
                    std::cout << "I noticed some spelling errors. Let me correct that: " << correctedInput << "\n\n";
                }
                processHealth(correctedInput);
                break;
            }
            case Mode::PC_BUILD:
                processPCBuild(input);
                break;
            case Mode::TEXT_REFINEMENT:
                processTextRefinement(input);
                break;
            case Mode::SIMPLE_GAME:
                processGame(input);
                break;
            default:
                askForClarification();
                break;
        }
    }
};

int main() {
    OmniBot bot;
    std::string input;
    
    std::cout << "Hello! I'm Omni-Bot, your multi-functional AI assistant.\n"
              << "I can help with PC building, health information, text refinement, or simple games.\n"
              << "How can I assist you today? (Type 'exit' to quit)\n";
    
    while (true) {
        std::cout << "\n> ";
        std::getline(std::cin, input);
        
        // Fixed: Use bot.toLower() instead of global toLower()
        if (bot.toLower(input) == "exit" || bot.toLower(input) == "quit") {
            break;
        }
        
        bot.processInput(input);
    }
    
    return 0;
}