#include <iostream>

int main()
{
#ifdef _WIN32
    std::cout << "PASS: Running on Windows." << std::endl;
#else
    std::cout << "FAIL: Unsupported platform." << std::endl;
    return 1;
#endif

    return 0;
}
