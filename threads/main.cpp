#include <mutex>
#include <iostream>
#include <thread>
#include <atomic>
#include <vector>

class Counter
{
public:
    explicit Counter(int initial = 0) : value_(initial) {}

    int get() const
    {
        std::lock_guard<std::mutex> lock(mtx_);
        return value_;
    }

    void increment()
    {
        std::lock_guard<std::mutex> lock(mtx_);
        ++value_;
    }

private:
    mutable std::mutex mtx_;
    int value_;
};

class Worker
{
public:
    Worker(Counter &counter) : counter_(counter), stop_flag(false) {}

    void run()
    {
        t_ = std::thread([this]
                         { this->task(); });
    }

    void stop()
    {
        stop_flag.store(true);
        if (t_.joinable())
        {
            t_.join();
        }
    }

    virtual ~Worker() = default;

protected:
    Counter &counter_;
    std::atomic<bool> stop_flag;

    virtual void task() = 0;

private:
    std::thread t_;
};

class SenderWorker : public Worker
{
public:
    using Worker::Worker;

protected:
    void task() override
    {
        while (!stop_flag.load())
        {
            counter_.increment();
            std::this_thread::sleep_for(std::chrono::microseconds(500));
        }
    }
};

class ReceiverWorker : public Worker
{
public:
    using Worker::Worker;

protected:
    void task() override
    {
        while (!stop_flag.load())
        {
            std::cout << "Counter: " << counter_.get() << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }
};

int main()
{
    Counter counter;

    std::vector<std::unique_ptr<Worker>> workers;

    workers.push_back(std::make_unique<SenderWorker>(counter));
    workers.push_back(std::make_unique<ReceiverWorker>(counter));

    for (auto &w : workers)
        w->run();

    std::this_thread::sleep_for(std::chrono::seconds(3));

    for (auto &w : workers)
        w->stop();

    std::cout << "Final counter: " << counter.get() << std::endl;
}