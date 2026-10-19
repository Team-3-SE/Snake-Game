#include "../src/InputHandler.h"

#include <chrono>
#include <cstdint>
#include <iostream>

int main()
{
    constexpr int iterations = 100000;

    // Prevent the benchmark results from being entirely unused.
    std::uint64_t parseChecksum = 0;
    std::uint64_t validationChecksum = 0;

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

    const auto parseTime =
        std::chrono::duration_cast<std::chrono::nanoseconds>(
            parseEnd - parseStart).count();

    const auto validationTime =
        std::chrono::duration_cast<std::chrono::nanoseconds>(
            validationEnd - validationStart).count();

    // Check results outside the timed sections.
    if (parseChecksum !=
            static_cast<std::uint64_t>(InputCommand::MoveUp) * iterations ||
        validationChecksum != iterations)
    {
        std::cerr << "Benchmark correctness check failed.\n";
        return 1;
    }

    std::cout << "Iterations per operation: " << iterations << '\n';
    std::cout << "Input parsing total: " << parseTime << " ns\n";
    std::cout << "Direction validation total: "
              << validationTime << " ns\n";

    std::cout << "Input parsing average: "
              << static_cast<double>(parseTime) / iterations
              << " ns per call\n";

    std::cout << "Direction validation average: "
              << static_cast<double>(validationTime) / iterations
              << " ns per call\n";

    std::cout << "Correctness checks: PASS\n";
    std::cout << "Note: function-level benchmark; not end-to-end latency.\n";

    return 0;
}
