#ifndef TESTER_QUOTE_H
#define TESTER_QUOTE_H

#include "Tester.h"
#include <vector>
#include <tuple>

class TesterQuote : public Tester {
public:
    TesterQuote() : last_test_result(false) {
		test_cases = {
			{"ls -l | cat -e", " ", "\"", {"ls", "-l", "|", "cat", "-e"}},
			{"hello \"world\" !", " ", "\"'", {"hello", "\"world\"", "!"}},
			{"hello\"world\"!", " ", "\"'", {"hello\"world\"!"}},
			{"\"hello 'world' !\"", " ", "\"", {"\"hello 'world' !\""}},
			{"hello 'world' !", " ", "\"'", {"hello", "'world'", "!"}},
			{"hello 'wo\"rld' !", " ", "\"", {"hello", "'wo\"rld' !"}},
			{"ls -l | cat -e", " ", "\"'", {"ls", "-l", "|", "cat", "-e"}},
			{"hello \"world\" !", " ", "\"'", {"hello", "\"world\"", "!"}},
			{"hello\"world\"!", " ", "\"'", {"hello\"world\"!"}},
			{"\"hello 'world' !\"", " ", "\"'", {"\"hello 'world' !\""}},
			{"hello 'world' !", " ", "\"'", {"hello", "'world'", "!"}},
			{"ls -l | cat -e", " ", "\"'", {"ls", "-l", "|", "cat", "-e"}},
			{"hello \"world\" !", " ", "\"'", {"hello", "\"world\"", "!"}},
			{"hello\"world\"!", " ", "\"'", {"hello\"world\"!"}},
			{"\"hello 'world' !\"", " ", "\"'", {"\"hello 'world' !\""}},
			{"hello 'world' !", " ", "\"'", {"hello", "'world'", "!"}},
			{"hello 'wo\"rld' !", " ", "\"'", {"hello", "'wo\"rld'", "!"}},
			{"ls -l | cat -e", " ", "\"'", {"ls", "-l", "|", "cat", "-e"}},
			{"hello \"world\" !", " ", "\"'", {"hello", "\"world\"", "!"}},
			{"hello\"world\"!", " ", "\"'", {"hello\"world\"!"}},
			{"\"hello 'world' !\"", " ", "\"'", {"\"hello 'world' !\""}},
			{"hello 'world' !", " ", "\"'", {"hello", "'world'", "!"}},
			{"hello 'wo\"rld' !", " ", "\"'", {"hello", "'wo\"rld'", "!"}},
			{"ls -l | cat -e", " ", "\"'", {"ls", "-l", "|", "cat", "-e"}},
			{"hello \"world\" !", " ", "\"'", {"hello", "\"world\"", "!"}},
			{"hello\"world\"!", " ", "\"'", {"hello\"world\"!"}},
		};
	}

    std::string getTestName() override;
    int getTests() override;

protected:
    bool runTest(long unsigned int index) override;

private:
    bool last_test_result;
    std::vector<std::tuple<std::string, std::string, std::string, std::vector<std::string>>> test_cases;

    std::vector<std::string> convert_char_array_to_vector(char** array);
    bool checkResults(const std::vector<std::string>& res, const std::vector<std::string>& expected);
};

#endif
