#include <random>
#include <thread>

struct RNG {
    // Mersenne Twister random number generator
    std::mt19937 gen;

    // Constructor: seed with random device
    RNG() : gen(std::random_device{}()) {}
    
    // Generate uniform integer in [low, high]
    int uniform_int(int low, int high) {
        std::uniform_int_distribution<> dist(low, high);
        return dist(gen);
    }

    // Create a sleep 
    void sleep_for_random_duration(int min_ms, int max_ms) {
        int duration = uniform_int(min_ms, max_ms);
        std::this_thread::sleep_for(std::chrono::milliseconds(duration));
    }
    
}