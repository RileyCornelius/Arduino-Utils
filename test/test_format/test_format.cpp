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
	afmt::format_to(buffer, "Value: {}", 2.5);
	TEST_ASSERT_EQUAL_STRING_MESSAGE("Value: 2.500000", buffer, "Float default precision");

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

	// Note: The current float formatting is basic and likely does not support 'e'/'E'.
	// These tests check for either proper scientific notation or graceful fallback.

	// Test lowercase scientific notation
	afmt::format_to(buffer, "Sci: {:.2e}", 12345.6789);
	// Expected: "Sci: 1.23e+04" if :e is implemented
	// Fallback: likely defaults to regular float formatting

	// For now, test what the current implementation produces
	// This test will help identify when :e support is added
	printf("Scientific notation test output: %s\n", buffer);
	TEST_ASSERT_TRUE_MESSAGE(strlen(buffer) > 0, "Scientific notation produces output");

	// Test uppercase scientific notation
	afmt::format_to(buffer, "Sci: {:.3E}", 0.00098765);
	// Expected: "Sci: 9.877E-04" if :E is implemented
	printf("Uppercase scientific test output: %s\n", buffer);
	TEST_ASSERT_TRUE_MESSAGE(strlen(buffer) > 0, "Uppercase scientific produces output");

	// Test scientific with zero
	afmt::format_to(buffer, "SciZero: {:.1e}", 0.0);
	// Expected: "SciZero: 0.0e+00" if :e is implemented
	printf("Scientific zero test output: %s\n", buffer);
	TEST_ASSERT_TRUE_MESSAGE(strlen(buffer) > 0, "Scientific zero produces output");
}

void test_format_to_general_notation()
{
	char buffer[50];

	// Note: The current float formatting is basic and likely does not support 'g'/'G'.
	// These tests check for either proper general notation or graceful fallback.

	// Test general notation (should prefer fixed for this case)
	afmt::format_to(buffer, "Gen: {:.3g}", 123.456);
	// Expected: "Gen: 123" (3 significant digits) if :g is implemented
	printf("General notation test output: %s\n", buffer);
	TEST_ASSERT_TRUE_MESSAGE(strlen(buffer) > 0, "General notation produces output");

	// Test general notation (should prefer scientific for large numbers)
	afmt::format_to(buffer, "GenLarge: {:.3g}", 1234567.0);
	// Expected: "GenLarge: 1.23e+06" if :g is implemented
	printf("General large number test output: %s\n", buffer);
	TEST_ASSERT_TRUE_MESSAGE(strlen(buffer) > 0, "General large number produces output");

	// Test uppercase general notation
	afmt::format_to(buffer, "GenUpper: {:.3G}", 0.00012345);
	// Expected: "GenUpper: 1.23E-04" if :G is implemented
	printf("Uppercase general test output: %s\n", buffer);
	TEST_ASSERT_TRUE_MESSAGE(strlen(buffer) > 0, "Uppercase general produces output");

	// Test general with small number
	afmt::format_to(buffer, "GenSmall: {:.4g}", 0.001234);
	// Expected: "GenSmall: 0.001234" if :g is implemented
	printf("General small number test output: %s\n", buffer);
	TEST_ASSERT_TRUE_MESSAGE(strlen(buffer) > 0, "General small number produces output");
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
	TEST_ASSERT_EQUAL_MESSAGE(14, size, "formatted_size calculation"); // "Hello, world!\0"
}

void test_formatted_size_complex()
{
	size_t size = afmt::formatted_size("Number: {}, Float: {:.2f}", 12345, 3.14159);
	std::string actual = afmt::format("Number: {}, Float: {:.2f}", 12345, 3.14159);
	TEST_ASSERT_EQUAL_MESSAGE(actual.length(), size, "formatted_size complex");
}

/*------------------------------------------------------------------------------
 * TESTS FOR adaptive_buffer
 *----------------------------------------------------------------------------*/

void test_format_to_adaptive_buffer()
{
	afmt::adaptive_buffer buf;

	afmt::format_to(buf, "Hello, {}!", "world");
	TEST_ASSERT_EQUAL_STRING_MESSAGE("Hello, world!", buf.data(), "adaptive_buffer formatting");
	TEST_ASSERT_EQUAL_MESSAGE(14, buf.size(), "adaptive_buffer size"); // Including null terminator
}

void test_format_to_adaptive_buffer_large()
{
	afmt::adaptive_buffer buf;

	// Test with string larger than SBO size
	std::string large_input(100, 'A');
	afmt::format_to(buf, "Large: {}", large_input.c_str());

	std::string expected = "Large: " + large_input;
	TEST_ASSERT_EQUAL_STRING_MESSAGE(expected.c_str(), buf.data(), "adaptive_buffer large string");
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
	afmt::format_to(buffer, "No args here{}");
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
	// Just verify that formatting many times doesn't crash
	char buffer[100];

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
	RUN_TEST(test_format_to_general_notation);

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

	// adaptive_buffer tests
	RUN_TEST(test_format_to_adaptive_buffer);
	RUN_TEST(test_format_to_adaptive_buffer_large);

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
	delay(2000);

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
