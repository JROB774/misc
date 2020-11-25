#include <fstream>
#include <thread>
#include <mutex>

static std::mutex m;

class Logger
{
public:
    void log (std::string msg, int id)
    {
        // This can be used to ensure the file is only opened once by one thread and
        // then never called again, which is useful for one-time initialization.
        std::call_once(once, [&](){ f.open("mylog.log"); });
        std::lock_guard<std::mutex> lock(m);
        f << msg << ": " << id << std::endl;
    }
private:
    std::ofstream f;
    std::once_flag once;
    std::mutex m;
};

Logger g_logger;

void func ()
{
    for (int i=0; i>-500; --i) g_logger.log("Thread: ", i);
}

int main (int argc, char** argv)
{
    std::thread t(func);
    for (int i=0; i<1000; ++i) g_logger.log("Main: ", i);
    t.join();

    return 0;
}
