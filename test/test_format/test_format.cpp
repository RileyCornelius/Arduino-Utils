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

void test_format_to_signs()
{
	char buffer[50];

	// Plus sign
	afmt::format_to(buffer, "Plus: {:+}", 42);
	TEST_ASSERT_EQUAL_STRING_MESSAGE("Plus: +42", buffer, "Plus sign");

	// Space for positive
	afmt::format_to(buffer, "Space: {: }", 42);
	TEST_ASSERT_EQUAL_STRING_MESSAGE("Space:  42", buffer, "Space for positive");

	// Negative (default)
	afmt::format_to(buffer, "Neg: {}", -42);
	TEST_ASSERT_EQUAL_STRING_MESSAGE("Neg: -42", buffer, "Negative sign");
}

void test_format_to_boolean()
{
	char buffer[50];

	// Boolean true
	afmt::format_to(buffer, "Bool: {}", true);
	TEST_ASSERT_EQUAL_STRING_MESSAGE("Bool: true", buffer, "Boolean true");

	// Boolean false
	afmt::format_to(buffer, "Bool: {}", false);
	TEST_ASSERT_EQUAL_STRING_MESSAGE("Bool: false", buffer, "Boolean false");
}

void test_format_to_char()
{
	char buffer[50];

	afmt::format_to(buffer, "Char: '{}'", 'A');
	TEST_ASSERT_EQUAL_STRING_MESSAGE("Char: 'A'", buffer, "Character formatting");
}

void test_format_to_pointer()
{
	char buffer[50];
	int value = 42;

	afmt::format_to(buffer, "Ptr: {}", &value);
	// Just check that it starts with "Ptr: 0x" and doesn't crash
	TEST_ASSERT_TRUE_MESSAGE(strncmp(buffer, "Ptr: 0x", 8) == 0, "Pointer formatting");

	// Null pointer
	afmt::format_to(buffer, "Null: {}", (void*)nullptr);
	TEST_ASSERT_EQUAL_STRING_MESSAGE("Null: nullptr", buffer, "Null pointer");
}

void test_format_to_multiple_args()
{
	char buffer[100];

	afmt::format_to(buffer, "Name: {}, Age: {}, Score: {:.1f}", "Alice", 25, 95.7);
	TEST_ASSERT_EQUAL_STRING_MESSAGE("Name: Alice, Age: 25, Score: 95.7", buffer,
									 "Multiple arguments");
}

void test_format_to_escaped_braces()
{
	char buffer[50];

	afmt::format_to(buffer, "Escaped: {{}} and {}", 42);
	TEST_ASSERT_EQUAL_STRING_MESSAGE("Escaped: {} and 42", buffer, "Escaped braces");
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
		afmt::format("User: {}, ID: {:04}, Active: {}, Score: {:.1f}%", "Bob", 123, true, 87.6);
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
	TEST_ASSERT_EQUAL_MESSAGE(actual.length() + 1, size,
							  "formatted_size complex"); // +1 for null terminator
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
	RUN_TEST(test_format_to_signs);
	RUN_TEST(test_format_to_boolean);
	RUN_TEST(test_format_to_char);
	RUN_TEST(test_format_to_pointer);
	RUN_TEST(test_format_to_multiple_args);
	RUN_TEST(test_format_to_escaped_braces);

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
	delay(5000);

	UNITY_BEGIN();
	tests();
	UNITY_END();
}

void loop()
{
	digitalWrite(13, HIGH);
	delay(100);
	digitalWrite(13, LOW);
	delay(500);
}
