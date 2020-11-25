#include <iostream>
#include <thread>
#include <mutex>

static std::mutex m;

void shared_print (std::string msg, int id)
{
    // Unique locks provide more control over the locking and unlocking mechanisms whilst still
    // providing the safety of unlocking a given mutex at destruction (in case of exception).

    std::unique_lock<std::mutex> lock(m, std::defer_lock); // Unique lock allows us to defer the locking of the mutex.
    // ...
    lock.lock();
    std::cout << msg << ": " << id << std::endl;
    lock.unlock(); // We can also explicitly unlock the mutex for fine-grained control.
    // ...
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
