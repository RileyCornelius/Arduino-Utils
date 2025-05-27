// More information about PlatformIO Unit Testing:
// https://docs.platformio.org/en/latest/advanced/unit-testing/index.html

#include <unity.h>
#include <Arduino.h>
#include <format.h>
#include <log.h>

/*------------------------------------------------------------------------------
 * TESTS FOR format_to (fixed array)
 *----------------------------------------------------------------------------*/

void test_format_to_basic_string()
{
	char buffer[50];
	afmt::format_to(buffer, "Hello, {}!", "world");
	TEST_ASSERT_EQUAL_STRING_MESSAGE("Hello, world!", buffer, "Basic string formatting");
}

void test_format_to_integers()
{
	char buffer[50];

	// Basic integer
	afmt::format_to(buffer, "Number: {}", 42);
	TEST_ASSERT_EQUAL_STRING_MESSAGE("Number: 42", buffer, "Basic integer");

	// Negative integer
	afmt::format_to(buffer, "Negative: {}", -123);
	TEST_ASSERT_EQUAL_STRING_MESSAGE("Negative: -123", buffer, "Negative integer");

	// Zero
	afmt::format_to(buffer, "Zero: {}", 0);
	TEST_ASSERT_EQUAL_STRING_MESSAGE("Zero: 0", buffer, "Zero value");
}

void test_format_to_hex()
{
	char buffer[50];

	// Lowercase hex
	afmt::format_to(buffer, "Hex: {:x}", 255);
	TEST_ASSERT_EQUAL_STRING_MESSAGE("Hex: ff", buffer, "Lowercase hex");

	// Uppercase hex
	afmt::format_to(buffer, "Hex: {:X}", 255);
	TEST_ASSERT_EQUAL_STRING_MESSAGE("Hex: FF", buffer, "Uppercase hex");

	// Hex with prefix
	afmt::format_to(buffer, "Hex: {:#x}", 255);
	TEST_ASSERT_EQUAL_STRING_MESSAGE("Hex: 0xff", buffer, "Hex with prefix");
}

void test_format_to_octal_binary()
{
	char buffer[50];

	// Octal
	afmt::format_to(buffer, "Oct: {:o}", 64);
	TEST_ASSERT_EQUAL_STRING_MESSAGE("Oct: 100", buffer, "Octal formatting");

	// Binary
	afmt::format_to(buffer, "Bin: {:b}", 5);
	TEST_ASSERT_EQUAL_STRING_MESSAGE("Bin: 101", buffer, "Binary formatting");

	// Binary with prefix
	afmt::format_to(buffer, "Bin: {:#b}", 5);
	TEST_ASSERT_EQUAL_STRING_MESSAGE("Bin: 0b101", buffer, "Binary with prefix");
}

void test_format_to_floats()
{
	char buffer[50];

	// Basic float
	afmt::format_to(buffer, "Pi: {:.2f}", 3.14159);
	TEST_ASSERT_EQUAL_STRING_MESSAGE("Pi: 3.14", buffer, "Float with precision");

	// Default precision
	afmt::format_to(buffer, "Value: {}", 2.50464);
	TEST_ASSERT_EQUAL_STRING_MESSAGE("Value: 2.50", buffer, "Float default precision");

	// Zero precision
	afmt::format_to(buffer, "Int: {:.0f}", 3.7);
	TEST_ASSERT_EQUAL_STRING_MESSAGE("Int: 4", buffer, "Float zero precision");
}

void test_format_to_alignment()
{
	char buffer[50];

	// Right alignment (default for numbers)
	afmt::format_to(buffer, "Right: '{:5}'", 42);
	TEST_ASSERT_EQUAL_STRING_MESSAGE("Right: '   42'", buffer, "Right alignment");

	// Left alignment
	afmt::format_to(buffer, "Left: '{:<5}'", 42);
	TEST_ASSERT_EQUAL_STRING_MESSAGE("Left: '42   '", buffer, "Left alignment");

	// Center alignment
	afmt::format_to(buffer, "Center: '{:^5}'", 42);
	TEST_ASSERT_EQUAL_STRING_MESSAGE("Center: ' 42  '", buffer, "Center alignment");
}

void test_format_to_zero_padding()
{
	char buffer[50];

	// Zero padding for integers
	afmt::format_to(buffer, "ZeroPad: {:05}", 42);
	TEST_ASSERT_EQUAL_STRING_MESSAGE("ZeroPad: 00042", buffer, "Zero padding integer");

	afmt::format_to(buffer, "ZeroPadNeg: {:05}", -42);
	TEST_ASSERT_EQUAL_STRING_MESSAGE("ZeroPadNeg: -0042", buffer, "Zero padding negative integer");

	// Zero padding with plus sign
	afmt::format_to(buffer, "ZeroPadPlus: {:+05}", 42);
	TEST_ASSERT_EQUAL_STRING_MESSAGE("ZeroPadPlus: +0042", buffer, "Zero padding with plus sign");

	// Zero padding with hex
	afmt::format_to(buffer, "ZeroPadHex: {:05x}", 42);
	TEST_ASSERT_EQUAL_STRING_MESSAGE("ZeroPadHex: 0002a", buffer, "Zero padding hex");

	// Zero padding with binary
	afmt::format_to(buffer, "ZeroPadBin: {:08b}", 5);
	TEST_ASSERT_EQUAL_STRING_MESSAGE("ZeroPadBin: 00000101", buffer, "Zero padding binary");

	// Test with a string to ensure zero padding is ignored for non-numeric types
	afmt::format_to(buffer, "ZeroPadStr: {:05}", "hi");
	TEST_ASSERT_EQUAL_STRING_MESSAGE("ZeroPadStr:    hi", buffer,
									 "Zero padding string (should be space)");
}

void test_format_to_scientific_notation()
{
	char buffer[50];

	// Test lowercase scientific notation
	afmt::format_to(buffer, "Sci: {:.2e}", 12345.6789);
	TEST_ASSERT_EQUAL_STRING_MESSAGE("Sci: 1.23e+04", buffer, "Scientific notation lowercase");

	// Test uppercase scientific notation
	afmt::format_to(buffer, "Sci: {:.3E}", 98765.4321);
	TEST_ASSERT_EQUAL_STRING_MESSAGE("Sci: 9.877E+04", buffer, "Scientific notation uppercase");

	// Test small numbers in scientific notation
	afmt::format_to(buffer, "Small: {:.2e}", 0.00012345);
	TEST_ASSERT_EQUAL_STRING_MESSAGE("Small: 1.23e-04", buffer, "Scientific small number");

	// Test very small numbers
	afmt::format_to(buffer, "VerySmall: {:.1E}", 0.000000987);
	TEST_ASSERT_EQUAL_STRING_MESSAGE("VerySmall: 9.9E-07", buffer, "Scientific very small number");

	// Test zero in scientific notation - updated for 2-digit precision
	afmt::format_to(buffer, "Zero: {:.2e}", 0.0);
	TEST_ASSERT_EQUAL_STRING_MESSAGE("Zero: 0.00e+00", buffer, "Scientific zero");

	// Test negative numbers
	afmt::format_to(buffer, "Neg: {:.1e}", -1234.5);
	TEST_ASSERT_EQUAL_STRING_MESSAGE("Neg: -1.2e+03", buffer, "Scientific negative");

	// Test with different precisions
	afmt::format_to(buffer, "Precision: {:.0e}", 1234.5);
	TEST_ASSERT_EQUAL_STRING_MESSAGE("Precision: 1e+03", buffer, "Scientific zero precision");

	afmt::format_to(buffer, "Precision: {:.4e}", 1234.5);
	TEST_ASSERT_EQUAL_STRING_MESSAGE("Precision: 1.2345e+03", buffer, "Scientific high precision");

	// Test edge case: exactly 1.0 - updated for 2-digit precision
	afmt::format_to(buffer, "One: {:.2e}", 1.0);
	TEST_ASSERT_EQUAL_STRING_MESSAGE("One: 1.00e+00", buffer, "Scientific notation for 1.0");

	// Test numbers that normalize to 10.0 (should become 1.0e+01)
	afmt::format_to(buffer, "Ten: {:.1e}", 9.99999);
	TEST_ASSERT_EQUAL_STRING_MESSAGE("Ten: 1.0e+01", buffer, "Scientific rounding to 10");
}

void test_format_to_scientific_edge_cases()
{
	char buffer[50];

	// Test infinity
	double inf = 1.0 / 0.0;
	afmt::format_to(buffer, "Inf: {:.2e}", inf); // specs.upper is false for 'e'
	TEST_ASSERT_EQUAL_STRING_MESSAGE("Inf: inf", buffer, "Scientific infinity lowercase");

	afmt::format_to(buffer, "Inf: {:.2E}", inf); // specs.upper is true for 'E' (exponent only)
	TEST_ASSERT_EQUAL_STRING_MESSAGE("Inf: INF", buffer, "Scientific infinity uppercase E");

	// Test negative infinity
	double neg_inf = -1.0 / 0.0;
	afmt::format_to(buffer, "NegInf: {:.2e}", neg_inf);
	TEST_ASSERT_EQUAL_STRING_MESSAGE("NegInf: -inf", buffer, "Scientific negative infinity");

	// Test NaN
	double nan_val = 0.0 / 0.0;
	afmt::format_to(buffer, "NaN: {:.2e}", nan_val);
	TEST_ASSERT_EQUAL_STRING_MESSAGE("NaN: nan", buffer, "Scientific NaN");

	// Test very large exponents
	afmt::format_to(buffer, "Large: {:.1e}", 1.0e20); // Use 1.0e20 to be explicit
	TEST_ASSERT_EQUAL_STRING_MESSAGE("Large: 1.0e+20", buffer, "Scientific large exponent");

	// Test very small exponents
	afmt::format_to(buffer, "Small: {:.1e}", 1.0e-20); // Use 1.0e-20
	TEST_ASSERT_EQUAL_STRING_MESSAGE("Small: 1.0e-20", buffer, "Scientific small exponent");
}

/*------------------------------------------------------------------------------
 * TESTS FOR format_to_n (with size limit)
 *----------------------------------------------------------------------------*/

void test_format_to_n_basic()
{
	char buffer[20];

	afmt::format_to_n(buffer, sizeof(buffer), "Hello, {}!", "world");
	TEST_ASSERT_EQUAL_STRING_MESSAGE("Hello, world!", buffer, "format_to_n basic");
}

void test_format_to_n_truncation()
{
	char buffer[10];

	afmt::format_to_n(buffer, sizeof(buffer), "This is a very long string");
	// Should be truncated to fit in 10 chars (including null terminator)
	TEST_ASSERT_TRUE_MESSAGE(strlen(buffer) < 10, "format_to_n truncation");
}

void test_format_to_n_zero_size()
{
	char buffer[10] = "unchanged";

	afmt::format_to_n(buffer, 0, "Hello, {}!", "world");
	TEST_ASSERT_EQUAL_STRING_MESSAGE("unchanged", buffer, "format_to_n zero size");
}

/*------------------------------------------------------------------------------
 * TESTS FOR format (std::string)
 *----------------------------------------------------------------------------*/

void test_format_basic_string()
{
	std::string result = afmt::format("Hello, {}!", "world");
	TEST_ASSERT_EQUAL_STRING_MESSAGE("Hello, world!", result.c_str(), "format basic string");
}

void test_format_integers()
{
	std::string result = afmt::format("Number: {}, Hex: {:x}", 42, 255);
	TEST_ASSERT_EQUAL_STRING_MESSAGE("Number: 42, Hex: ff", result.c_str(), "format integers");
}

void test_format_floats()
{
	std::string result = afmt::format("Pi: {:.3f}", 3.14159);
	TEST_ASSERT_EQUAL_STRING_MESSAGE("Pi: 3.142", result.c_str(), "format floats");
}

void test_format_complex()
{
	std::string result =
		afmt::format("User: {}, ID: {:0>4}, Active: {}, Score: {:.1f}%", "Bob", 123, true, 87.6);
	TEST_ASSERT_EQUAL_STRING_MESSAGE("User: Bob, ID: 0123, Active: true, Score: 87.6%",
									 result.c_str(), "format complex");
}

/*------------------------------------------------------------------------------
 * TESTS FOR formatted_size
 *----------------------------------------------------------------------------*/

void test_formatted_size()
{
	size_t size = afmt::formatted_size("Hello, {}!", "world");
	TEST_ASSERT_EQUAL_MESSAGE(13, size, "formatted_size calculation"); // "Hello, world!"
}

void test_formatted_size_complex()
{
	size_t size = afmt::formatted_size("Number: {}, Float: {:.2f}", 12345, 3.14159);
	std::string actual = afmt::format("Number: {}, Float: {:.2f}", 12345, 3.14159);
	TEST_ASSERT_EQUAL_MESSAGE(actual.length(), size, "formatted_size complex");
}

/*------------------------------------------------------------------------------
 * EDGE CASES AND ERROR HANDLING
 *----------------------------------------------------------------------------*/

void test_empty_format_string()
{
	char buffer[10];
	afmt::format_to(buffer, "");
	TEST_ASSERT_EQUAL_STRING_MESSAGE("", buffer, "Empty format string");
}

void test_no_arguments()
{
	char buffer[20];
	afmt::format_to(buffer, "No args here");
	TEST_ASSERT_EQUAL_STRING_MESSAGE("No args here", buffer, "No arguments");
}

void test_unmatched_braces()
{
	char buffer[50];

	// Unmatched closing brace (should output as-is)
	afmt::format_to(buffer, "Unmatched }");
	TEST_ASSERT_EQUAL_STRING_MESSAGE("Unmatched }", buffer, "Unmatched closing brace");
}

#if AFMT_HAS_ARDUINO
/*------------------------------------------------------------------------------
 * TESTS FOR Arduino String functions
 *----------------------------------------------------------------------------*/

void test_aformat()
{
	String result = afmt::aformat("Hello, {}!", "Arduino");
	TEST_ASSERT_EQUAL_STRING_MESSAGE("Hello, Arduino!", result.c_str(), "aformat basic");
}

void test_aformat_complex()
{
	String result = afmt::aformat("Pin: {}, Value: {}, Voltage: {:.2f}V", 13, 1023, 3.3);
	TEST_ASSERT_EQUAL_STRING_MESSAGE("Pin: 13, Value: 1023, Voltage: 3.30V", result.c_str(),
									 "aformat complex");
}
#endif

/*------------------------------------------------------------------------------
 * PERFORMANCE TESTS (basic validation)
 *----------------------------------------------------------------------------*/

void test_performance_basic()
{
	char buffer[100];

	// Simple performance test: format a string multiple times
	for(int i = 0; i < 100; ++i)
	{
		afmt::format_to(buffer, "Iteration: {}, Value: {:.2f}", i, i * 1.5);
	}

	// Check the last iteration
	TEST_ASSERT_EQUAL_STRING_MESSAGE("Iteration: 99, Value: 148.50", buffer, "Performance test");
}

/*------------------------------------------------------------------------------
 * SETUP AND TEST RUNNER
 *----------------------------------------------------------------------------*/

// set stuff up here
void setUp(void)
{
	// Any setup needed before each test
}

// clean stuff up here
void tearDown(void)
{
	// Any cleanup needed after each test
}

// tests here
void tests()
{
	// format_to tests
	RUN_TEST(test_format_to_basic_string);
	RUN_TEST(test_format_to_integers);
	RUN_TEST(test_format_to_hex);
	RUN_TEST(test_format_to_octal_binary);
	RUN_TEST(test_format_to_floats);
	RUN_TEST(test_format_to_alignment);
	RUN_TEST(test_format_to_zero_padding);
	RUN_TEST(test_format_to_scientific_notation);
	RUN_TEST(test_format_to_scientific_edge_cases);

	// format_to_n tests
	RUN_TEST(test_format_to_n_basic);
	RUN_TEST(test_format_to_n_truncation);
	RUN_TEST(test_format_to_n_zero_size);

	// format tests
	RUN_TEST(test_format_basic_string);
	RUN_TEST(test_format_integers);
	RUN_TEST(test_format_floats);
	RUN_TEST(test_format_complex);

	// formatted_size tests
	RUN_TEST(test_formatted_size);
	RUN_TEST(test_formatted_size_complex);

	// Edge cases
	RUN_TEST(test_empty_format_string);
	RUN_TEST(test_no_arguments);
	RUN_TEST(test_unmatched_braces);

#if AFMT_HAS_ARDUINO
	// Arduino-specific tests
	RUN_TEST(test_aformat);
	RUN_TEST(test_aformat_complex);
#endif

	// Performance
	RUN_TEST(test_performance_basic);
}

void setup()
{
	// NOTE!!! Wait for >2 secs
	// if board doesn't support software reset via Serial.DTR/RTS
	delay(5000);

	UNITY_BEGIN();
	tests();
	UNITY_END();

	pinMode(13, OUTPUT); // Set pin 13 as output for LED
}

void loop()
{
	digitalWrite(13, HIGH);
	delay(100);
	digitalWrite(13, LOW);
	delay(500);
}
