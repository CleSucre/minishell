#ifndef TESTER_H
#define TESTER_H

#include <iostream>
#include <string>

extern "C" {
    #include "../../include/minishell.h"
}

class Tester {
public:
    virtual std::string getTestName() = 0;

    virtual int getTests() = 0;

    virtual int runTests() {
        int successCount = 0;

		std::cout << CYAN << "Running " << getTestName() << " tests..." << RESET << std::endl;

        for (int i = 0; i < getTests(); i++) {
            if (runTest(i)) {
                successCount++;
            }
		}

		std::cout << CYAN << "Finished " << getTestName() << " tests, " << successCount << " / " << getTests() << " passed." << RESET << std::endl;
        return successCount;
    }

private:
    virtual bool runTest(long unsigned int index) = 0;
};

#endif
