#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>

// TODO make this a better implementation later

template <typename T>
class ThreadSafeQueue {
private:
    
    // Underlying queue
    std::queue<T> queue;

    // mutex for thread synchronization
    std::mutex mutex;

    // Condition variable for signaling
    std::condition_variable cv;

public:
    // Pushes an element to the queue
    void push(T item)
    {

        // Acquire lock
        std::unique_lock<std::mutex> lock(mutex);

        // Add item
        queue.push(item);

        // Notify one thread that is waiting
        cv.notify_one();
    }

    // Pops an element off the queue
    T pop()
    {
        // acquire lock
        std::unique_lock<std::mutex> lock(mutex);

        // wait until queue is not empty
        cv.wait(lock, [this]() { return !queue.empty(); });

        // retrieve item
        T item = queue.front();
        queue.pop();

        // return item
        return item;
    }
};

