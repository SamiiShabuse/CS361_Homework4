/**
 * @file src/race.cpp
 * 
 * @brief Implementation of race simulation functions. 
 * 
 * @author Samii Shabuse <sus24@drexel.edu>
 * @date October 25, 2025
 * 
 * @section Overview
 * 
 * This file contains the implementation of the game master and racer thread functions.
 */
#include <iostream>
#include "race.h"
#include "util.h"

/**
 * @brief Convert thread ID to string.
 */
static thread_local RNG_mine tls_rng;


/**
 * @brief Thread-safe print function.
 * 
 * @param S Reference to the RaceState for synchronization.
 * @param msg Message to print.
 * 
 * @return void
 * 
 * @details This function locks the I/O mutex in the RaceState to ensure that messages from different threads do not interleave.
 *          It then prints the provided message to the standard output, making it thread-safe.
 */
static void safe_print(RaceState& S, const std::string& msg) {
  std::lock_guard<std::mutex> g(S.io_mtx);
  std::cout << msg << "\n";
}


/**
 * @brief Game master thread function.
 * 
 * @param S Reference to the RaceState.
 * 
 * @return void
 * 
 * @details This function continuously produces dice rolls and pushes them into the shared dice queue until all racers have finished.
 *          It generates a random number of dice rolls (between 0 and 5) each tick, with each die being a 6-sided die.
 *          After pushing the dice rolls, it sleeps for a random duration between 0 and 2 seconds before the next tick.
 *          Once all racers have finished, it notifies all waiting threads to ensure they can exit cleanly.
 */
void game_master(RaceState& S) {
  // Produce dice until all racers finish.
  while (!S.all_finished.load(std::memory_order_relaxed)) {
    int k = tls_rng.uniform_int(0, 5);        // number of dice to roll this tick
    for (int i = 0; i < k; ++i) {
      int d = tls_rng.uniform_int(1, 6);      // a single 6-sided die
      S.dice.push(d);
    }
    tls_rng.sleep_between_0_and_2_seconds();
  }
  // Ensure any sleepers wake up
  S.dice.notify_all();
}


/**
 * @brief Racer thread function.
 * 
 * @param S Reference to the RaceState.
 * @param index Index of the racer.
 * 
 * @return void
 * 
 * @details This function represents a racer in the race. Each racer thread continuously attempts to pop dice rolls from the shared dice queue.
 *          Upon receiving a dice roll, the racer moves forward by that amount. If the racer reaches or exceeds the finish distance, it marks itself as finished,
 *          updates the finish order, and announces its completion. The racer sleeps for a random duration between 0 and 2 seconds after each move.
 *          The racer thread exits once it has finished the race.
 */
void racer(RaceState& S, int index) {
  safe_print(S, "Thread " + tid_to_str(std::this_thread::get_id()) + " has left the gate.");

  while (true) {
    if (S.finished[index]) break;
    int roll = 0;
    try {
      roll = S.dice.pop_blocking([&]{ return S.all_finished.load(std::memory_order_relaxed); });
    } catch (...) {
      // If stopped and queue empty, check if we already finished; else loop (GM might still push).
      if (S.all_finished.load()) break;
      else continue;
    }

    // Move forward
    S.distance[index] += roll;
    safe_print(S, "Thread " + tid_to_str(std::this_thread::get_id()) +
            " moved forward " + std::to_string(roll) + " spaces.");

    if (S.distance[index] >= S.finish_distance) {
      // Mark finish once
      bool announce = false;
      {
        std::lock_guard<std::mutex> g(S.rank_mtx);
        if (!S.finished[index]) {
          S.finished[index] = true;
          ++S.finished_count;
          // record the finishing thread id in finish order
          S.finish_order.push_back(std::this_thread::get_id());
          announce = true;
          if (S.finished_count == S.total_racers) {
            S.all_finished.store(true);
            S.dice.notify_all();
          }
        }
      }
      if (announce) {
        safe_print(S, "Thread " + tid_to_str(std::this_thread::get_id()) +
                " has crossed the finish line.");
      }
      break;
    }

    tls_rng.sleep_between_0_and_2_seconds();
  }
}
