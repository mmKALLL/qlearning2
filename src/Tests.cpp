#include "Tests.hpp"

int runTests() {
    int testsPassed = 0;
    
    if (runNetworkTest() != 0) {
        std::cout << "Network test failed! Terminating tests." << std::endl;
        return -1;
    } else {
        testsPassed += 1;
        std::cout << "Network test passed." << std::endl;
    }
    
    if (runPhysicsTest() != 0) {
        std::cout << "Physics test failed! Terminating tests." << std::endl;
        return -2;
    } else {
        testsPassed += 1;
        std::cout << "Physics test passed." << std::endl;
    }
    
    std::cout << std::endl << "All tests were OK." << std::endl << testsPassed << " tests were passed." << std::endl;
    return 0;
}

int runNetworkTest() {
    Node testNode();
    return 0;
}

int runPhysicsTest() {
    return 0;
}
