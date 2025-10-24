#include <vector>
#include <thread>
#include <mutex>
#include <atomic>


struct RaceState {
    // movement and finish
    std::vector<int> distance;          // size r, per race distance
    std::vector<bool> finished;         // size r
    std::vector<std::pair<int, std::thread::id>> finish_order; // size r, order of finishing (distance, thread id)


    // completion tracking
    int finished_count = 0;          // number of participants finished
    int total_races = 0;              // total number of participants (threads)
    std::atomic<bool> all_finished{false};  // atomic flag to indicate all have finished

    // threadsafe dice queue
    ThreadSafeQueue<int> dice_queue;  // queue of dice rolls

    // For completion tracking
    std::mutex completion_mtx;         // mutex for synchronizing access to completion tracking data

    // For I/O synchronization
    std::mutex io_mtx;                 // mutex for synchronizing access to shared data

    std::mutex rank_mtx;              // mutex for synchronizing access to finish_order and finished_count
};