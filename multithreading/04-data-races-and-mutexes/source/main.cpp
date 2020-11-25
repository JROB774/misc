#include <iostream>
#include <thread>
#include <mutex>

static std::mutex m; // Used by locking execution to avoid race conditions (in this case printing to std output).

void shared_print (std::string msg, int id)
{
    // This is preferred over straight locking and unlocking as this uses RAII. This is beneficial
    // because if an exception is thrown the stack will unwind ahd the mutex will be unlocked.
    std::lock_guard<std::mutex> guard(m);
    // m.lock();
    std::cout << msg << ": " << id << std::endl;
    // m.unlock();
}

void func ()
{
    for (int i=0; i>-500; --i) shared_print("Thread: ", i);
}

int main (int argc, char** argv)
{
    std::thread t(func);
    for (int i=0; i<1000; ++i) shared_print("Main: ", i);
    t.join();

    return 0;
}
