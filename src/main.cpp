#include <iostream>

int main (int argc, char** argv) {
    // Check for exactly one argument
    if (argc < 2 or argc > 2) {
        std::cerr << "usage: " << argv[0] << " <number>\n";
        return -1;
    }

    // Check to make sure the argument is a number greater than 0
    try {
        int number = std::stoi(argv[1]);
        if (number <= 0) {
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
    std::printf("Starting race with %s participants!\n", argv[1]);

    // TODO: insert function I will be calling here later

    return 0;

}