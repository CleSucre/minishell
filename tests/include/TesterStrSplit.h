#ifndef TESTER_STRSPLIT_H
#define TESTER_STRSPLIT_H

#include "Tester.h"
#include <vector>
#include <tuple>

class TesterStrSplit : public Tester {
public:
    TesterStrSplit() : last_test_result(false) {
		test_cases = {
			{"Hello World How you doing?", "World", {"Hello ", " How you doing?"}},
		};
	}

    std::string getTestName() override;
    int getTests() override;

protected:
    bool runTest(long unsigned int index) override;

private:
    bool last_test_result;
    std::vector<std::tuple<std::string, std::string, std::vector<std::string>>> test_cases;

    std::vector<std::string> convert_char_array_to_vector(char** array);
    bool checkResults(const std::vector<std::string>& res, const std::vector<std::string>& expected);
};

#endif
