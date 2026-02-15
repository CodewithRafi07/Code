#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <string>

/**
 * @file loading_animations.cpp
 * @brief A collection of console-based loading animations using C++.
 * * This program demonstrates how to create visual feedback in a terminal
 * using standard output (\r carriage return) and thread sleeping.
 */

/**
 * Clears the current line in the console and moves the cursor to the beginning.
 */
void resetLine() {
    std::cout << "\r" << std::flush;
}

/**
 * Displays a spinning character animation.
 * @param duration_ms Total time to run the animation in milliseconds.
 */
void spinnerAnimation(int duration_ms) {
    const std::vector<char> frames = {'|', '/', '-', '\\'};
    int elapsed = 0;
    int i = 0;

    std::cout << "Loading... ";
    while (elapsed < duration_ms) {
        std::cout << frames[i % frames.size()] << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        resetLine();
        std::cout << "Loading... ";
        i++;
        elapsed += 100;
    }
    std::cout << "Done!          " << std::endl;
}

/**
 * Displays a growing progress bar.
 * @param steps Number of segments in the bar.
 */
void progressBarAnimation(int steps) {
    std::cout << "Downloading: [";
    for (int i = 0; i <= steps; ++i) {
        float percentage = (float)i / steps * 100;
        
        resetLine();
        std::cout << "Downloading: [";
        
        // Draw filled part
        for (int j = 0; j < i; ++j) std::cout << "=";
        // Draw empty part
        for (int j = i; j < steps; ++j) std::cout << " ";
        
        std::cout << "] " << (int)percentage << "%" << std::flush;
        
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }
    std::cout << "\nDownload Complete!" << std::endl;
}

/**
 * Displays a bouncing dot animation.
 * @param duration_ms Total time to run.
 */
void bouncingDotAnimation(int duration_ms) {
    const int width = 20;
    int pos = 0;
    int direction = 1;
    int elapsed = 0;

    while (elapsed < duration_ms) {
        resetLine();
        std::cout << "Searching: <";
        
        for (int i = 0; i < width; ++i) {
            if (i == pos) std::cout << "O";
            else std::cout << ".";
        }
        
        std::cout << ">" << std::flush;

        pos += direction;
        if (pos == width - 1 || pos == 0) direction *= -1;

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        elapsed += 50;
    }
    std::cout << "\nFound!" << std::endl;
}

int main() {
    std::cout << "--- C++ Console Loading Animations ---" << std::endl << std::endl;

    // 1. Spinner
    spinnerAnimation(3000);
    std::cout << std::endl;

    // 2. Progress Bar
    progressBarAnimation(30);
    std::cout << std::endl;

    // 3. Bouncing Dot
    bouncingDotAnimation(4000);

    std::cout << "\nAll animations finished. Press Enter to exit.";
    std::cin.get();

    return 0;
}