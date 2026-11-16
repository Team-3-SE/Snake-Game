// Minimal dependency-free check helper (standard library only).
#ifndef TEST_CHECK_H
#define TEST_CHECK_H

#include <iostream>

namespace tf
{
    inline int& failures() { static int n = 0; return n; }
    inline int& checks() { static int n = 0; return n; }
}

#define CHECK(cond)                                                      \
    do {                                                                 \
        ++tf::checks();                                                  \
        if (!(cond)) {                                                   \
            ++tf::failures();                                            \
            if (tf::failures() <= 20)                                    \
                std::cerr << __FILE__ << ":" << __LINE__                 \
                          << " CHECK failed: " #cond "\n";               \
        }                                                                \
    } while (0)

// Returns the process exit code.
inline int finish(const char* name)
{
    std::cout << name << ": " << tf::checks() - tf::failures() << "/"
              << tf::checks() << " checks passed\n";
    return tf::failures() == 0 ? 0 : 1;
}

#endif
