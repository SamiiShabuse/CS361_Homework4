/**
 * @file src/thread_safe_queue.h
 * 
 * @brief Definition of ThreadSafeQueue class.
 * 
 * @author Samii Shabuse <sus24@drexel.edu>
 * @date October 25, 2025
 * 
 * @section Overview
 *
 * This file contains the definition of a thread-safe queue class that supports blocking pop operations. 
 * It uses mutexes and condition variables to ensure safe concurrent access. 
 */
#pragma once
#include <condition_variable>
#include <mutex>
#include <queue>
#include <optional>
#include <functional>
#include <stdexcept>

/**
 * @brief A thread-safe queue implementation.
 * 
 * @tparam T Type of elements stored in the queue.
 * 
 * @details This class provides a thread-safe queue with blocking pop functionality.
 */
template <typename T>
class ThreadSafeQueue {

    private:
        std::queue<T> q; // underlying queue
        std::mutex m;  // mutex for synchronizing access
        std::condition_variable cv; // condition variable for blocking pops
        bool closed = false; // when closed, wake all waiters and no more pushes
    
    public:
        ThreadSafeQueue() = default; // default constructor
        ~ThreadSafeQueue() = default; // default destructor

        /**
         * @brief Push an item into the queue.
         * 
         * @param item The item to be pushed.
         * 
         * @return void
         */
        void push(T item) {
            std::unique_lock<std::mutex> lk(m);
            if (closed) return;
            q.push(std::move(item));
            cv.notify_one();
        }

        /**
         * @brief Blocking pop that returns false if the queue is closed and empty.
         * 
         * @param out Reference to store the popped item.
         * 
         * @return true if an item was popped, false if the queue is closed and empty.
         */
        bool pop_blocking(T &out) {
            std::unique_lock<std::mutex> lk(m);
            cv.wait(lk, [this]{ return !q.empty() || closed; });
            if (q.empty()) return false;
            out = std::move(q.front());
            q.pop();
            return true;
        }

        /**
         * @brief Blocking pop that uses a predicate to determine when to stop waiting.
         * 
         * @param stop Predicate function that returns true to stop waiting.
         * 
         * @return The popped item.
         */
        template <typename Pred>
        T pop_blocking(const Pred &stop) {
            std::unique_lock<std::mutex> lk(m);
            cv.wait(lk, [this, &stop]{ return !q.empty() || stop(); });
            if (q.empty()) throw std::runtime_error("stopped");
            T v = std::move(q.front());
            q.pop();
            return v;
        }

        /**
         * @brief Notify all waiting threads and close the queue.
         * 
         * @param void
         * 
         * @return void
         * 
         */
        void notify_all() {
            std::unique_lock<std::mutex> lk(m);
            closed = true;
            cv.notify_all();
        }

        /**
         * @brief Check if the queue is empty.
         * 
         * @param void
         * 
         * @return true if the queue is empty, false otherwise.
         */
        bool empty() {
            std::unique_lock<std::mutex> lk(m);
            return q.empty();
        }
};

