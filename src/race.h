/**
 * @file src/race.h
 * 
 * @brief Definition of RaceState structure and race functions.
 * 
 * @author Samii Shabuse <sus24@drexel.edu>
 * @date October 25, 2025
 * 
 * @section Overview
 * 
 * This file contains the definition of the RaceState structure and declarations of the game master and racer thread functions.
 */

#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include "thread_safe_queue.h"


/**
 * @brief Structure to hold the state of the race. 
 * 
 * @details Includes parameters, racer states, synchronization primitives, and a thread-safe dice queue.
 * 
 */
struct RaceState {
    // parameters
    const int total_racers;     // total number of racers
    const int finish_distance = 20; // distance to finish line

    // movement and finish per racer
    std::vector<int> distance;          // size r, per racer distance
    std::vector<bool> finished;         // size r
    std::vector<int> finish_order;      // order of finishing: store racer indices in finish order


    // completion tracking
    int finished_count = 0;          // number of participants finished
    int total_races = 0;              // total number of participants (threads)
    std::atomic<bool> all_finished{false};  // atomic flag to indicate all have finished

    // threadsafe dice queue
    ThreadSafeQueue<int> dice;  // queue of dice rolls

    // For completion tracking
    std::mutex completion_mtx;         // mutex for synchronizing access to completion tracking data

    // For I/O synchronization
    std::mutex io_mtx;                 // mutex for synchronizing access to shared data

    std::mutex rank_mtx;              // mutex for synchronizing access to finish_order and finished_count

    explicit RaceState(int racer)
    : total_racers(racer), distance(racer, 0), finished(racer, false) {}
};

/**
 * @brief Game master thread function.
 * 
 * @param state Reference to the RaceState.
 * 
 * @return void
 */
void game_master(RaceState& state);

/**
 * @brief Racer thread function.
 * 
 * @param state Reference to the RaceState.
 * @param racer_id Index of the racer.
 * 
 * @return void
 */
void racer(RaceState& state, int racer_id);