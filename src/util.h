/**
 * @file src/util.h
 * 
 * @brief Utility functions and structures for the racing simulation.
 * 
 * @author Samii Shabuse <sus24@drexel.edu>
 * @date October 25, 2025
 * 
 * @section Overview
 * 
 * This file contains utility functions and structures for the racing simulation.
 */

#include <random>
#include <thread>
#include <sstream>

/**
 * @brief Simple random number generator utility.
 * 
 * @details Provides methods for generating uniform integers and sleeping for random durations.
 */
struct RNG_mine {
    // Mersenne Twister random number generator
    std::mt19937 gen;

    // Constructor: seed with random device
    RNG_mine() : gen(std::random_device{}()) {}

    /**
     * @brief Generate a uniform integer between low and high (inclusive).
     * 
     * @param low Lower bound (inclusive).
     * @param high Upper bound (inclusive).
     * 
     * @return Random integer between low and high.
     */
    int uniform_int(int low, int high) {
        std::uniform_int_distribution<> dist(low, high);
        return dist(gen);
    }

    /**
     * @brief Sleep for a random duration between 0 and 2 seconds.
     * 
     * @param void
     * 
     * @return void
     */
    void sleep_between_0_and_2_seconds(void) {
        int duration = uniform_int(0, 2000); // milliseconds
        std::this_thread::sleep_for(std::chrono::milliseconds(duration));
    }

};

/**
 * @brief Convert a thread ID to a string.
 * 
 * @param tid Thread ID.
 * 
 * @return String representation of the thread ID.
 * 
 * @details Uses a stringstream to convert the thread ID to string.
 */
inline std::string tid_to_str(std::thread::id tid) {
  std::ostringstream oss; oss << tid; return oss.str();
}