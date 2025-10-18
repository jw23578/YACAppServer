#ifndef JW78CORE_THREADSAFEQUEUE_H
#define JW78CORE_THREADSAFEQUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>

template <class T>
class ThreadSafeQueue
{
public:
    ThreadSafeQueue(void)
        : dataQueue()
        , mutex()
        , condition()
    {}

    ~ThreadSafeQueue(void)
    {}

    void enqueue(T t)
    {
        std::lock_guard<std::mutex> lock(mutex);
        dataQueue.push(t);
        condition.notify_one();
    }

    T dequeue(void)
    {
        while (dataQueue.size() == 0)
        {
            std::unique_lock<std::mutex> lock(mutex);
            condition.wait_for(lock, std::chrono::seconds(10), [&]{return !dataQueue.empty();});
        }
        T val = dataQueue.front();
        dataQueue.pop();
        return val;
    }

private:
    std::queue<T> dataQueue;
    mutable std::mutex mutex;
    std::condition_variable condition;
};

#endif // JW78CORE_THREADSAFEQUEUE_H
