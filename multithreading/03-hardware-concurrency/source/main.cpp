#include <iostream>
#include <thread>

int main (int argc, char** argv)
{
    // This value can be useful for creating an optimal number of threads and avoiding oversubscription.
    std::cout << "Hardware can support " << std::thread::hardware_concurrency() << " threads." << std::endl;
    return 0;
}
