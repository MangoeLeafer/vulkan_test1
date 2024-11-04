#include "src/app.hpp"

#include <iostream>

using namespace App;

int main() {
    Application app;
    bool keepRunning = true;

    try {
        app.init();

        while (keepRunning) {
            app.run(keepRunning);
        }
        
        app.cleanUp();

    } catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}