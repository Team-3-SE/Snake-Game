
#include "../src/InputHandler.h"

#include <chrono>
#include <cassert>
#include <iostream>

int main()
{
    constexpr int iterations = 100000;

    // Measure input parsing.
    const auto parseStart = std::chrono::steady_clock::now();

    for (int i = 0; i < iterations; ++i)
    {
        const InputCommand command = InputHandler::parseInput('w');
        assert(command == InputCommand::MoveUp);
    }

    const auto parseEnd = std::chrono::steady_clock::now();

    // Measure direction validation.
    const auto validationStart = std::chrono::steady_clock::now();

    for (int i = 0; i < iterations; ++i)
    {
        const bool valid = InputHandler::isValidDirection(
            Direction::Right, Direction::Up);
        assert(valid);
    }

    const auto validationEnd = std::chrono::steady_clock::now();

    const auto parseTime =
        std::chrono::duration_cast<std::chrono::microseconds>(
            parseEnd - parseStart).count();

    const auto validationTime =
        std::chrono::duration_cast<std::chrono::microseconds>(
            validationEnd - validationStart).count();

    std::cout << "Iterations: " << iterations << '\n';
    std::cout << "Input parsing total: " << parseTime << " us\n";
    std::cout << "Direction validation total: "
              << validationTime << " us\n";

    std::cout << "Input parsing average: "
              << static_cast<double>(parseTime) / iterations
              << " us per call\n";

    std::cout << "Direction validation average: "
              << static_cast<double>(validationTime) / iterations
              << " us per call\n";

    // These timings are diagnostic; the SRS requirement concerns
    // response to actual user input in the integrated application.
    return 0;
}
