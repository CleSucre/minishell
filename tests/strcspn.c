#include <stdio.h>
#include <string.h>

#include <stddef.h>

#include <stddef.h>

size_t	ft_strcspn_recursive(const char *s1, const char *s2, size_t pos)
{
    if (!s1[pos])
        return (pos);
    const char *spanp = s2;
    while (*spanp)
	{
        if (*spanp == s1[pos])
            return (pos);
        spanp++;
    }
    return (ft_strcspn_recursive(s1, s2, pos + 1));
}

size_t	ft_strcspn(const char *s1, const char *s2) {
    return ft_strcspn_recursive(s1, s2, 0);
}

void run_test(const char *str, const char *reject)
{
    size_t len = strcspn(str, reject);
    size_t len2 = ft_strcspn(str, reject);
    if (len != len2)
        printf("Error: str='%s', reject='%s', expected %zu, got %zu\n", str, reject, len, len2);
}

int main(void)
{
    // Série de tests
    run_test("hello", "l");
    run_test("hello", "x");
    run_test("hello, world", ", ");
    run_test("1234567890", "5");
    run_test("abcdef", "xyz");
    run_test("abcdef", "def");
    run_test("hello world", " ");
    run_test("foo bar baz", "baz");
    run_test("foo bar baz", "bar");
    run_test("test string", "g");

    run_test("", "a");
    run_test("abc", "");
    run_test("", "");
    run_test("123456", "123");
    run_test("123456", "456");
    run_test("abc", "c");
    run_test("abc", "a");
    run_test("a", "a");
    run_test("a", "b");
    run_test("abcd", "xyz");

    run_test("abcd", "xyzd");
    run_test("test string", "s");
    run_test("example", "pl");
    run_test("space", " ");
    run_test("newline\n", "\n");
    run_test("tab\tcharacter", "\t");
    run_test("1234567890", "0");
    run_test("alpha beta gamma", "beta");
    run_test("abcdef", "fghij");
    run_test("longer string for testing", "test");

    run_test("another test case", "case");
    run_test("yet another one", " ");
    run_test("punctuation!", "!");
    run_test("more tests", "t");
    run_test("hello again", "again");
    run_test("abcdef", "abcd");
    run_test("short", "longer");
    run_test("numbers123", "123");
    run_test("characters", "cters");
    run_test("abcdefg", "g");

    run_test("abcdefg", "gfedcba");
    run_test("0123456789", "9876543210");
    run_test("so10me string", "string");
    run_test("final test", "final");
    run_test("edge cases", "cases");
    run_test("function", "function");

	printf("All tests completed.\n");
    return (0);
}
