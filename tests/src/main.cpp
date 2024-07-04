#include "TesterQuote.h"
#include <cstring>

using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::pair;
using std::get;
using std::tuple;

int main() {
	int	totalTests = 0;
	int	successCount = 0;

	TesterQuote quoteTester;

	totalTests += quoteTester.getTests();

	successCount += quoteTester.runTests();

	if (successCount == totalTests) {
		cout << "\n" << GREEN << "All " << YELLOW << successCount << GREEN << " tests passed!" << RESET << endl;
	} else {
		cout << "\n" << RED << totalTests - successCount << " tests failed!" << RESET << endl;
	}
	return 0;
}
