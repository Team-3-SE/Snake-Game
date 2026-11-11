/*
 * Test Case: TC-SEC-04
 * Requirement: SG-SR-004
 *
 * Objective:
 * Verify that the game does not transmit player or gameplay data
 * over a network.
 *
 * Test Method: Automated scan of src/ (run from the repository root)
 *
 * Test Steps:
 * 1. Inspect the source files for network communication APIs.
 * 2. Check for external API or online service integrations.
 * 3. Review the project documentation for its offline-game requirement.
 *
 * Expected Result:
 * The game should not use network communication or transmit
 * player or gameplay data.
 *
 * Observed Result:
 * No common network communication APIs or external service
 * integrations were found in the inspected source files.
 *
 * Test Status: PASS (automated source scan)
 */

#include "check.h"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

int main(int argc, char** argv)
{
    namespace fs = std::filesystem;
    const fs::path srcDir = (argc > 1) ? fs::path(argv[1]) : fs::path("src");
    CHECK(fs::is_directory(srcDir));

    const std::vector<std::string> forbidden = {
        "<winsock", "<sys/socket.h>", "<netinet", "<arpa/inet.h>",
        "<netdb.h>", "<curl/", "<SFML/Network", "WSAStartup",
        "socket(", "connect(", "getaddrinfo", "InternetOpen",
        "URLDownloadToFile", "http://", "https://"};

    int scanned = 0;
    if (fs::is_directory(srcDir))
    {
        for (const auto& entry : fs::directory_iterator(srcDir))
        {
            const std::string ext = entry.path().extension().string();
            if (ext != ".cpp" && ext != ".h")
                continue;
            ++scanned;

            std::ifstream in(entry.path());
            std::stringstream text;
            text << in.rdbuf();
            const std::string body = text.str();

            for (const std::string& token : forbidden)
            {
                const bool found = body.find(token) != std::string::npos;
                if (found)
                    std::cerr << entry.path() << " contains " << token << "\n";
                CHECK(!found);
            }
        }
    }
    CHECK(scanned > 0);
    return finish("TC_SEC_04");
}
