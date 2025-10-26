/**
 * @file src/main.cpp
 * 
 * @brief Main entry point for the race 2000 simulation program.
 * 
 * @author Samii Shabuse <sus24@drexel.edu>
 * @date October 25, 2025
 * 
 * @section Overview
 * 
 * This file contains the main function that initializes the race simulation. It processes command-line arguments to determine the number of racers,
 * spawns racer threads and a game master thread, and manages their execution. After all racers have finished, it prints the final rankings of the race.
 */
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "race.h"
#include "util.h"


/**
 * @brief Main function to run the race simulation.
 * 
 * @param argc Argument count.
 * @param argv Argument vector.
 * 
 * @return int Exit status.
 * 
 * @details This function initializes the race simulation, spawns threads for each racer and a game master, and manages their execution.
 *          It processes command-line arguments to determine the number of racers, ensuring valid input. After all racers have finished,
 *          it prints the final rankings.
 */
int main (int argc, char** argv) {
    // Check for exactly one argument
    if (argc < 2 or argc > 2) {
        std::cerr << "usage: " << argv[0] << " <number>\n";
        return -1;
    }

    int r;
    // Check to make sure the argument is a number greater than 0
    try {
        r = std::stoi(argv[1]);
        if (r <= 0) {
            std::cerr << "Error: " << argv[1] << " is not a positive number.\n";
            return -1;
        }
    } catch (std::invalid_argument&) {
        std::cerr << "Error: " << argv[1] << " is not a valid number.\n";
        return -1;
    } catch (std::out_of_range&) {
        std::cerr << "Error: " << argv[1] << " is out of range.\n";
        return -1;
    }

    // Starting race intro
    std::printf("Starting race with %d threads.\n", r);

    RaceState S(r);

    // Spawn racers
    std::vector<std::thread> racers;
    racers.reserve(r);
    for (int i = 0; i < r; ++i) {
        racers.emplace_back([&S, i]{ racer(S, i); });
    }

    // Spawn game master
    std::thread gm([&S]{ game_master(S); });

    // Join racers first (so all finish), then GM
    for (auto& t : racers) t.join();
    S.all_finished.store(true);
    S.dice.notify_all();
    gm.join();

    // Print final rankings (finish_order stores thread ids in finishing order)
    for (size_t i = 0; i < S.finish_order.size(); ++i) {
        std::thread::id tid = S.finish_order[i];
        std::cout << (i + 1) << " : " << tid_to_str(tid) << "\n";
    }

    return 0;

}