#include <iostream>
#include <deque>
#include <thread>
#include <mutex>
#include <chrono>

static std::deque<int> q;
static std::mutex m;

static std::condition_variable cond;

void function0 ()
{
    int count = 10;
    while (count > 0)
    {
        std::unique_lock<std::mutex> lock(m);
        q.push_front(count);
        lock.unlock();
        cond.notify_one(); // Notify one waiting thread if there is one. (We can also notify_all).
        std::this_thread::sleep_for(std::chrono::seconds(1));
        count--;
    }
}

void function1 ()
{
    int data = 0;
    while (data != 1)
    {
        std::unique_lock<std::mutex> lock(m);
        // We pass in lock so that the condition can unlock it and then lock it again once we wake up. This
        // is so that other threads can use the lock whilst we are sleeping (because we aren't using it.)
        cond.wait(lock, [](){ return !q.empty(); }); // Waits until notified by the other thread but will not actually carry on execution
        data = q.back();                             // until q is not empty because there is no point otherwisw. This is suprious wake.
        q.pop_back();
        lock.unlock();
        std::cout << "t1 got a value from t0: " << data << std::endl;
    }
}

int main (int argc, char** argv)
{
    std::thread t0(function0);
    std::thread t1(function1);

    // Important to always either join or detach threads.
    t0.join();
    t1.join();

    return 0;
}
