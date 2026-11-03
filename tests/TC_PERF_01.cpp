#include "../src/InputHandler.h"

#include <chrono>
#include <cstdint>
#include <iostream>

int main()
{
    constexpr int iterations = 100000;
    constexpr std::chrono::milliseconds responseLimit{100};

    std::uint64_t parseChecksum = 0;
    std::uint64_t validationChecksum = 0;
    std::uint64_t combinedChecksum = 0;

    const auto parseStart = std::chrono::steady_clock::now();
    for (int i = 0; i < iterations; ++i)
    {
        const InputCommand command = InputHandler::parseInput('w');
        parseChecksum += static_cast<std::uint64_t>(command);
    }
    const auto parseEnd = std::chrono::steady_clock::now();

    const auto validationStart = std::chrono::steady_clock::now();
    for (int i = 0; i < iterations; ++i)
    {
        const bool valid = InputHandler::isValidDirection(
            Direction::Right, Direction::Up);
        validationChecksum += valid ? 1U : 0U;
    }
    const auto validationEnd = std::chrono::steady_clock::now();

    // Combined path: parse each input and validate the resulting direction.
    // This remains a function-level path, not real keyboard-to-render latency.
    const auto combinedStart = std::chrono::steady_clock::now();
    for (int i = 0; i < iterations; ++i)
    {
        const InputCommand command = InputHandler::parseInput('w');
        const bool valid = (command == InputCommand::MoveUp) &&
            InputHandler::isValidDirection(Direction::Right, Direction::Up);
        combinedChecksum += valid ? 1U : 0U;
    }
    const auto combinedEnd = std::chrono::steady_clock::now();

    const auto parseTime =
        std::chrono::duration_cast<std::chrono::nanoseconds>(
            parseEnd - parseStart).count();
    const auto validationTime =
        std::chrono::duration_cast<std::chrono::nanoseconds>(
            validationEnd - validationStart).count();
    const auto combinedTime =
        std::chrono::duration_cast<std::chrono::nanoseconds>(
            combinedEnd - combinedStart).count();

    if (parseChecksum !=
            static_cast<std::uint64_t>(InputCommand::MoveUp) * iterations ||
        validationChecksum != iterations ||
        combinedChecksum != iterations)
    {
        std::cerr << "Benchmark correctness check failed.\n";
        return 1;
    }

    const double combinedAverageNs =
        static_cast<double>(combinedTime) / iterations;
    const double combinedAverageMs = combinedAverageNs / 1'000'000.0;

    std::cout << "Iterations per operation: " << iterations << '\n';
    std::cout << "Input parsing total: " << parseTime << " ns\n";
    std::cout << "Direction validation total: "
              << validationTime << " ns\n";
    std::cout << "Combined input processing total: "
              << combinedTime << " ns\n";

    std::cout << "Input parsing average: "
              << static_cast<double>(parseTime) / iterations
              << " ns per call\n";
    std::cout << "Direction validation average: "
              << static_cast<double>(validationTime) / iterations
              << " ns per call\n";
    std::cout << "Combined input processing average: "
              << combinedAverageNs << " ns per operation ("
              << combinedAverageMs << " ms)\n";

    std::cout << "Correctness checks: PASS\n";

    if (combinedAverageMs <= responseLimit.count())
    {
        std::cout << "100 ms processing threshold: PASS\n";
    }
    else
    {
        std::cerr << "100 ms processing threshold: FAIL\n";
        return 2;
    }

    std::cout << "Note: function-level benchmark; not end-to-end keyboard "
                 "or rendering latency.\n";
    return 0;
}
