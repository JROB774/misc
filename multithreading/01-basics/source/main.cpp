#include <iostream>
#include <chrono>
#include <thread>

static std::atomic_bool g_finished = false;

void do_work ()
{
    using namespace std::literals::chrono_literals;
    std::cout << "Started Thread ID" << std::this_thread::get_id() << std::endl;
    while (!g_finished)
    {
        std::cout << "Working..." << std::endl;
        std::this_thread::sleep_for(1s);
    }
}

int main (int argc, char** argv)
{
    std::cout << "Started Thread ID" << std::this_thread::get_id() << std::endl;
    std::thread worker(do_work);
    std::cin.get();
    g_finished = true;
    worker.join(); // Wait for thread to complete.
    std::cout << "Finished!" << std::endl;
    return 0;
}
