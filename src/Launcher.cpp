#include <string>

int main(int argc, char* argv[]) {
    if (argc < 1) { // First argument always program call
        std::cerr << "Error in Launcher; wrong amount of args." << std::endl;
    } else {
        for (int i = 1; i < argc; i++) {
            if (argv[i] == "-t" || argv[i] == "--test") {
                Tests::runTests();
            }
            if (argv[i] == "-h" || argv[i] == "--help") {
                std::cout << "This is the help string." << std::endl
                << "Execute with -t or --test to run the tests. Execute without parameters to start up the GUI."
            }
        }
    }
    
    // Do things if user runs the program normally, like...
    // Controller mainController();
        
    return 0;
}