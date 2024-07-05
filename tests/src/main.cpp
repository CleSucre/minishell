#include "TesterQuote.h"
#include "TesterStrSplit.h"

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
	TesterStrSplit strSplitTester;

	totalTests += quoteTester.getTests();
	totalTests += strSplitTester.getTests();

	successCount += quoteTester.runTests();
	successCount += strSplitTester.runTests();

	if (successCount == totalTests) {
		cout << "\n" << GREEN << "All " << YELLOW << successCount << GREEN << " tests passed!" << RESET << endl;
	} else {
		cout << "\n" << RED << totalTests - successCount << " tests failed!" << RESET << endl;
	}
	return 0;
}
