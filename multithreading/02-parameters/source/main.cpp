#include <iostream>
#include <chrono>
#include <thread>

void wait (std::string name, int count)
{
    using namespace std::literals::chrono_literals;
    std::cout << "Started Thread " << name << " (" << count << ")" << std::endl;
    for (int i=0; i<count; ++i) std::this_thread::sleep_for(1ms);
    std::cout << "Finished Thread " << name << std::endl;
}

int main (int argc, char** argv)
{
    std::thread a(wait, "A",  50);
    std::thread b(wait, "B", 100);
    std::thread c(wait, "C",  20);

    // Wait for threads to complete.
    a.join();
    b.join();
    c.join();

    return 0;
}
