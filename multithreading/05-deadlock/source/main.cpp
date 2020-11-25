#include <iostream>
#include <thread>
#include <mutex>

static std::mutex m0; // Used by locking execution to avoid race conditions (in this case printing to std output).
static std::mutex m1;

void shared_print (std::string msg, int id)
{
    // If we were using more than one mutex then there is potential for threads being deadlocked.
    // So it's important to always lock mutexes in the same order. Furthermore, it is bes to use
    // std::lock to lock mulitple mutexes at the same time to avoid these kinds of issues.
    std::lock(m0,m1);
    // Telling the lock_guard to adopt the locks means we've already locked the mutex it just has to be unlocked on destruction.
    std::lock_guard<std::mutex> g0(m0, std::adopt_lock);
    std::lock_guard<std::mutex> g1(m1, std::adopt_lock);
    std::cout << msg << ": " << id << std::endl;
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
