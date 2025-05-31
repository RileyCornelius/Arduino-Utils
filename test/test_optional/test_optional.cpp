#include <unity.h>
#include <Arduino.h>
#include <Optional.h>

/*------------------------------------------------------------------------------
 * TESTS FOR Optional Construction
 *----------------------------------------------------------------------------*/

void test_optional_default_constructor()
{
    Optional<int> opt;
    TEST_ASSERT_FALSE_MESSAGE(opt.hasValue(), "Default constructed optional should be empty");
    TEST_ASSERT_FALSE_MESSAGE((bool)opt, "Default constructed optional should be false");
}

void test_optional_nullopt_constructor()
{
    Optional<int> opt(nullopt);
    TEST_ASSERT_FALSE_MESSAGE(opt.hasValue(), "Nullopt constructed optional should be empty");
    TEST_ASSERT_FALSE_MESSAGE((bool)opt, "Nullopt constructed optional should be false");
}

void test_optional_value_constructor()
{
    Optional<int> opt(42);
    TEST_ASSERT_TRUE_MESSAGE(opt.hasValue(), "Value constructed optional should have value");
    TEST_ASSERT_TRUE_MESSAGE((bool)opt, "Value constructed optional should be true");
    TEST_ASSERT_EQUAL_MESSAGE(42, opt.value(), "Value should match constructor argument");
}

void test_optional_copy_constructor()
{
    Optional<int> opt1(123);
    Optional<int> opt2(opt1);
    
    TEST_ASSERT_TRUE_MESSAGE(opt2.hasValue(), "Copied optional should have value");
    TEST_ASSERT_EQUAL_MESSAGE(123, opt2.value(), "Copied value should match original");
    
    // Test copying empty optional
    Optional<int> empty1;
    Optional<int> empty2(empty1);
    TEST_ASSERT_FALSE_MESSAGE(empty2.hasValue(), "Copied empty optional should be empty");
}

void test_optional_string_constructor()
{
    Optional<String> opt("Hello");
    TEST_ASSERT_TRUE_MESSAGE(opt.hasValue(), "String optional should have value");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("Hello", opt.value().c_str(), "String value should match");
}

/*------------------------------------------------------------------------------
 * TESTS FOR Optional Assignment
 *----------------------------------------------------------------------------*/

void test_optional_nullopt_assignment()
{
    Optional<int> opt(42);
    opt = nullopt;
    TEST_ASSERT_FALSE_MESSAGE(opt.hasValue(), "Optional should be empty after nullopt assignment");
}

void test_optional_value_assignment()
{
    Optional<int> opt;
    opt = 99;
    TEST_ASSERT_TRUE_MESSAGE(opt.hasValue(), "Optional should have value after assignment");
    TEST_ASSERT_EQUAL_MESSAGE(99, opt.value(), "Assigned value should match");
    
    // Test reassignment
    opt = 200;
    TEST_ASSERT_EQUAL_MESSAGE(200, opt.value(), "Reassigned value should match");
}

void test_optional_copy_assignment()
{
    Optional<int> opt1(456);
    Optional<int> opt2;
    
    opt2 = opt1;
    TEST_ASSERT_TRUE_MESSAGE(opt2.hasValue(), "Assigned optional should have value");
    TEST_ASSERT_EQUAL_MESSAGE(456, opt2.value(), "Assigned value should match");
    
    // Test self-assignment
    opt1 = opt1;
    TEST_ASSERT_TRUE_MESSAGE(opt1.hasValue(), "Self-assigned optional should retain value");
    TEST_ASSERT_EQUAL_MESSAGE(456, opt1.value(), "Self-assigned value should be unchanged");
    
    // Test assigning empty to non-empty
    Optional<int> empty;
    opt1 = empty;
    TEST_ASSERT_FALSE_MESSAGE(opt1.hasValue(), "Optional should be empty after empty assignment");
}

/*------------------------------------------------------------------------------
 * TESTS FOR Optional Methods
 *----------------------------------------------------------------------------*/

void test_optional_hasValue()
{
    Optional<int> empty;
    Optional<int> filled(42);
    
    TEST_ASSERT_FALSE_MESSAGE(empty.hasValue(), "Empty optional hasValue should be false");
    TEST_ASSERT_TRUE_MESSAGE(filled.hasValue(), "Filled optional hasValue should be true");
}

void test_optional_value()
{
    Optional<int> opt(789);
    TEST_ASSERT_EQUAL_MESSAGE(789, opt.value(), "value() should return stored value");
    
    const Optional<int> constOpt(321);
    TEST_ASSERT_EQUAL_MESSAGE(321, constOpt.value(), "const value() should return stored value");
}

void test_optional_valueOr()
{
    Optional<int> empty;
    Optional<int> filled(100);
    
    TEST_ASSERT_EQUAL_MESSAGE(999, empty.valueOr(999), "valueOr should return default for empty");
    TEST_ASSERT_EQUAL_MESSAGE(100, filled.valueOr(999), "valueOr should return value for filled");
}

void test_optional_valueOr_string()
{
    Optional<String> empty;
    Optional<String> filled("World");
    
    String defaultStr = empty.valueOr("Default");
    String filledStr = filled.valueOr("Default");
    
    TEST_ASSERT_EQUAL_STRING_MESSAGE("Default", defaultStr.c_str(), "valueOr should return default string");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("World", filledStr.c_str(), "valueOr should return stored string");
}

/*------------------------------------------------------------------------------
 * TESTS FOR Optional Operators
 *----------------------------------------------------------------------------*/

void test_optional_bool_operator()
{
    Optional<int> empty;
    Optional<int> filled(42);
    
    TEST_ASSERT_FALSE_MESSAGE((bool)empty, "Empty optional should convert to false");
    TEST_ASSERT_TRUE_MESSAGE((bool)filled, "Filled optional should convert to true");
    
    // Test in if statements
    if (empty) {
        TEST_FAIL_MESSAGE("Empty optional should not enter if statement");
    }
    
    if (!filled) {
        TEST_FAIL_MESSAGE("Filled optional should enter if statement");
    }
}

void test_optional_dereference_operator()
{
    Optional<int> opt(555);
    TEST_ASSERT_EQUAL_MESSAGE(555, *opt, "Dereference should return stored value");
    
    const Optional<int> constOpt(777);
    TEST_ASSERT_EQUAL_MESSAGE(777, *constOpt, "Const dereference should return stored value");
    
    // Test modification through dereference
    *opt = 888;
    TEST_ASSERT_EQUAL_MESSAGE(888, *opt, "Modified value should be updated");
}

void test_optional_arrow_operator()
{
    Optional<String> opt("Test");
    TEST_ASSERT_EQUAL_MESSAGE(4, opt->length(), "Arrow operator should access methods");
    
    const Optional<String> constOpt("Hello");
    TEST_ASSERT_EQUAL_MESSAGE(5, constOpt->length(), "Const arrow operator should access methods");
}

void test_optional_equality_operator()
{
    Optional<int> empty1;
    Optional<int> empty2;
    Optional<int> filled1(42);
    Optional<int> filled2(42);
    Optional<int> filled3(99);
    
    TEST_ASSERT_TRUE_MESSAGE(empty1 == empty2, "Two empty optionals should be equal");
    TEST_ASSERT_TRUE_MESSAGE(filled1 == filled2, "Two optionals with same value should be equal");
    TEST_ASSERT_FALSE_MESSAGE(empty1 == filled1, "Empty and filled optionals should not be equal");
    TEST_ASSERT_FALSE_MESSAGE(filled1 == filled3, "Optionals with different values should not be equal");
}

void test_optional_inequality_operator()
{
    Optional<int> empty;
    Optional<int> filled1(42);
    Optional<int> filled2(99);
    
    TEST_ASSERT_TRUE_MESSAGE(empty != filled1, "Empty and filled should be unequal");
    TEST_ASSERT_TRUE_MESSAGE(filled1 != filled2, "Different values should be unequal");
    TEST_ASSERT_FALSE_MESSAGE(filled1 != filled1, "Same optional should not be unequal to itself");
}

/*------------------------------------------------------------------------------
 * TESTS FOR Optional with Complex Types
 *----------------------------------------------------------------------------*/

struct TestStruct {
    int value;
    String name;
    
    TestStruct(int v, const String& n) : value(v), name(n) {}
    
    bool operator==(const TestStruct& other) const {
        return value == other.value && name == other.name;
    }
};

void test_optional_complex_type()
{
    Optional<TestStruct> opt(TestStruct(42, "Test"));
    
    TEST_ASSERT_TRUE_MESSAGE(opt.hasValue(), "Complex type optional should have value");
    TEST_ASSERT_EQUAL_MESSAGE(42, opt->value, "Complex type member access should work");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("Test", opt->name.c_str(), "Complex type string member should work");
}

void test_optional_complex_assignment()
{
    Optional<TestStruct> opt;
    opt = TestStruct(100, "Assigned");
    
    TEST_ASSERT_TRUE_MESSAGE(opt.hasValue(), "Complex type assignment should work");
    TEST_ASSERT_EQUAL_MESSAGE(100, opt.value().value, "Assigned complex type value should match");
}

/*------------------------------------------------------------------------------
 * TESTS FOR Optional Edge Cases
 *----------------------------------------------------------------------------*/

void test_optional_multiple_assignments()
{
    Optional<int> opt;
    
    // Empty -> Value
    opt = 1;
    TEST_ASSERT_EQUAL_MESSAGE(1, opt.value(), "First assignment should work");
    
    // Value -> Value
    opt = 2;
    TEST_ASSERT_EQUAL_MESSAGE(2, opt.value(), "Second assignment should work");
    
    // Value -> Empty
    opt = nullopt;
    TEST_ASSERT_FALSE_MESSAGE(opt.hasValue(), "Assignment to nullopt should work");
    
    // Empty -> Value again
    opt = 3;
    TEST_ASSERT_EQUAL_MESSAGE(3, opt.value(), "Assignment after nullopt should work");
}

void test_optional_chain_operations()
{
    Optional<String> opt("Hello");
    
    // Chain multiple operations
    if (opt && opt->length() > 0) {
        String result = opt.valueOr("") + " World";
        TEST_ASSERT_EQUAL_STRING_MESSAGE("Hello World", result.c_str(), "Chained operations should work");
    } else {
        TEST_FAIL_MESSAGE("Chain operation condition should be true");
    }
}

/*------------------------------------------------------------------------------
 * TESTS FOR Optional Function Returns
 *----------------------------------------------------------------------------*/

Optional<int> getOptionalInt(bool hasValue) {
    if (hasValue) {
        return 42;
    }
    return nullopt;
}

Optional<String> getOptionalString(bool hasValue) {
    if (hasValue) {
        return String("Success");
    }
    return nullopt;
}

void test_optional_function_returns()
{
    Optional<int> result1 = getOptionalInt(true);
    Optional<int> result2 = getOptionalInt(false);
    
    TEST_ASSERT_TRUE_MESSAGE(result1.hasValue(), "Function should return filled optional");
    TEST_ASSERT_EQUAL_MESSAGE(42, result1.value(), "Function return value should match");
    TEST_ASSERT_FALSE_MESSAGE(result2.hasValue(), "Function should return empty optional");
    
    Optional<String> strResult1 = getOptionalString(true);
    Optional<String> strResult2 = getOptionalString(false);
    
    TEST_ASSERT_TRUE_MESSAGE(strResult1.hasValue(), "String function should return filled optional");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("Success", strResult1.value().c_str(), "String function return should match");
    TEST_ASSERT_FALSE_MESSAGE(strResult2.hasValue(), "String function should return empty optional");
}

/*------------------------------------------------------------------------------
 * SETUP AND TEST RUNNER
 *----------------------------------------------------------------------------*/

void setUp(void) {
    // Any setup needed before each test
}

void tearDown(void) {
    // Any cleanup needed after each test
}

void tests() {
    // Construction tests
    RUN_TEST(test_optional_default_constructor);
    RUN_TEST(test_optional_nullopt_constructor);
    RUN_TEST(test_optional_value_constructor);
    RUN_TEST(test_optional_copy_constructor);
    RUN_TEST(test_optional_string_constructor);
    
    // Assignment tests
    RUN_TEST(test_optional_nullopt_assignment);
    RUN_TEST(test_optional_value_assignment);
    RUN_TEST(test_optional_copy_assignment);
    
    // Method tests
    RUN_TEST(test_optional_hasValue);
    RUN_TEST(test_optional_value);
    RUN_TEST(test_optional_valueOr);
    RUN_TEST(test_optional_valueOr_string);
    
    // Operator tests
    RUN_TEST(test_optional_bool_operator);
    RUN_TEST(test_optional_dereference_operator);
    RUN_TEST(test_optional_arrow_operator);
    RUN_TEST(test_optional_equality_operator);
    RUN_TEST(test_optional_inequality_operator);
    
    // Complex type tests
    RUN_TEST(test_optional_complex_type);
    RUN_TEST(test_optional_complex_assignment);
    
    // Edge case tests
    RUN_TEST(test_optional_multiple_assignments);
    RUN_TEST(test_optional_chain_operations);
    
    // Function return tests
    RUN_TEST(test_optional_function_returns);
}

void setup() {
    // Wait for serial connection
    delay(5000);
    
    UNITY_BEGIN();
    tests();
    UNITY_END();
}

void loop() {
}
