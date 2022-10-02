#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include "miniunit.h"
#include "smintf.h"

int _test_one();
int _test_two();
int _test_three();

int main(int argc, char* argv[]) {

	mu_run(_test_one);
	mu_run(_test_two);
	mu_run(_test_three);

	return EXIT_SUCCESS;
}

int _test_one() {
	mu_start();

	char* expected = "1";
	char* test = smintf("%d", 1);
	mu_check_strings_eq(expected, test);
	free(test);

	expected = "A";
	test = smintf("A");
	mu_check_strings_eq(expected, test);
	free(test);
		
	expected = "";
	test = smintf("");
	mu_check_strings_eq(expected, test);
	free(test);
	
	expected = "19 27";
    test = smintf("%d %d", 19, 27);
	mu_check_strings_eq(expected, test);
	free(test);

	expected = "-14 -13";
	test = smintf("%d %d", -14,-13);
	mu_check_strings_eq(expected, test);
	free(test);

	expected = "2147483647 -2147483648";
	test = smintf("%d %d", INT_MAX, INT_MIN);
	mu_check_strings_eq(expected, test);
	free(test);


	mu_end();
}
int _test_two() {
	mu_start();

	char* expected1 = "0b111 0b111";
	char* test1 = smintf("%b %b", 7, 7);
	mu_check_strings_eq(expected1, test1);
	free(test1);

	expected1 = "-0b111 -0b111";
	test1 = smintf("%b %b", -7, -7);
	mu_check_strings_eq(expected1, test1);
	free(test1);

	expected1 = "0x41a 0x4d8";
	test1 = smintf("%x %x", 1050, 1240);
	mu_check_strings_eq(expected1, test1);
	free(test1);

	expected1 = "-0x41a -0x4d8";
	test1 = smintf("%x %x", -1050, -1240);
	mu_check_strings_eq(expected1, test1);
	free(test1);

	expected1 = "Hello";
	test1 = smintf("Hello");
	mu_check_strings_eq(expected1, test1);
	free(test1);

	expected1 = "%%";
	test1 = smintf("%%%");
	mu_check_strings_eq(expected1, test1);
	free(test1);

	expected1 = "0x7fffffff -0x80000000";
	test1 = smintf("%x %x", INT_MAX, INT_MIN);
	mu_check_strings_eq(expected1, test1);
	free(test1);

	mu_end();
}

int _test_three() {
	mu_start();

	char* expected2 = "$10.20 -$24.90";
	char* test2 = smintf("%$ %$", 1020, -2490);
	mu_check_strings_eq(expected2, test2);
	free(test2);

	expected2 = "$21474836.47 -$21474836.48";
	test2 = smintf("%$ %$", INT_MAX, INT_MIN);
	mu_check_strings_eq(expected2, test2);
	free(test2);

	expected2 = "Excess parameters are ignored: 75";
	test2 = smintf("Excess parameters are ignored: %d", 75, 75);
	mu_check_strings_eq(expected2, test2);
	free(test2);

	expected2 = "Hello I am Alan";
	test2 = smintf("Hello I am %s", "Alan");
	mu_check_strings_eq(expected2, test2);
	free(test2);

	expected2 = "Your chance is %30";
	test2 = smintf("Your chance is %30");
	mu_check_strings_eq(expected2, test2);
	free(test2);

	mu_end();
}
