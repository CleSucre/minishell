#include "TesterStrSplit.h"

using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::tuple;
using std::get;

std::string TesterStrSplit::getTestName() {
    return "ft_strsplit";
}

int TesterStrSplit::getTests() {
    return static_cast<int>(test_cases.size());
}

bool TesterStrSplit::runTest(long unsigned int index) {
    if (index >= test_cases.size())
        return false;

    const auto& [str, charset, expected] = test_cases[index];
	char** raw_result = ft_strsplit(str.c_str(), charset.c_str());
	vector<string> res = convert_char_array_to_vector(raw_result);
	last_test_result = checkResults(res, expected);

    cout << (last_test_result ? GREEN "OK | " : RED "KO | ");
    cout << "str: |" << str << "| -> ";
	for (const auto& item : res) {
		cout << "[" << item << "]";
	}
    if (!last_test_result) {
		cout << " EXPECTED : ";
		for (const auto& exp : expected) {
			cout << "[" << exp << "]";
		}
    }
    cout << RESET << endl;
    return last_test_result;
}

std::vector<std::string> TesterStrSplit::convert_char_array_to_vector(char** array) {
	std::vector<std::string> result;
	if (array) {
		for (int i = 0; array[i] != nullptr; ++i) {
			result.push_back(std::string(array[i]));
			free(array[i]);
		}
		free(array);
	}
	return result;
}

bool TesterStrSplit::checkResults(const std::vector<std::string>& res, const std::vector<std::string>& expected) {
    if (res.size() != expected.size())
        return false;

    for (size_t i = 0; i < res.size(); ++i) {
        if (res[i] != expected[i])
            return false;
    }

    return true;
}
