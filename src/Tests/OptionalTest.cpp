// #undef NDEBUG  // Comment to disable asserts
// #define CONFIG_COMPILER_OPTIMIZATION_ASSERTIONS_SILENT 1 // Uncomment to make asserts silent
// #define CONFIG_COMPILER_ASSERT_NDEBUG_EVALUATE 1 // Uncomment to evaluate but not crash

#include <Optional.h>
#include <optional>
#include <Arduino.h>

Optional<int> createInt(bool b) {
    if (b)
        return 42;
    return nullopt;
}

Optional<String> createString(bool b) {
    if (b)
        return String("Hello");
    return nullopt;
}

void optionalSetup() {
    Serial.println();
    Serial.println("-------- Optional Test --------");

    // Test basic construction and value access
    Optional<int> optInt = 42;
    if (optInt.hasValue()) {
        Serial.println("Optional int value(): " + String(optInt.value()));
        Serial.println("Optional int *value: " + String(*optInt));
    } else {
        Serial.println("Optional int is empty");
    }

    // Test empty optional
    Optional<int> optInt2;
    if (optInt2) {
        Serial.println("Optional int2 value(): " + String(optInt2.value()));
    } else {
        Serial.println("Optional int2 is empty");
    }

    // Test valueOr
    int defaultVal = optInt2.valueOr(99);
    Serial.println("Optional int2 valueOr(99): " + String(defaultVal));

    // Test string optional
    Optional<String> optStr = createString(true);
    if (optStr.hasValue()) {
        Serial.println("Optional string has value: " + optStr.value());
        Serial.println("Optional string length: " + String(optStr->length()));
    }

    Optional<String> optStrEmpty = createString(false);
    String defaultStr = optStrEmpty.valueOr("default");
    Serial.println("Empty string optional valueOr: " + defaultStr);

    // Test equality
    Optional<int> optEq1 = 10;
    Optional<int> optEq2 = 10;
    Optional<int> optEq3 = 20;
    Optional<int> optEmpty1;
    Optional<int> optEmpty2;

        Serial.println("optEq1 == optEq2: " + String(optEq1 == optEq2 ? "true" : "false"));
    Serial.println("optEq1 != optEq3: " + String(optEq1 != optEq3 ? "true" : "false"));
    Serial.println("optEq1 == optEmpty1: " + String(optEq1 == optEmpty1 ? "true" : "false"));
    Serial.println("optEmpty1 == optEmpty2: " + String(optEmpty1 == optEmpty2 ? "true" : "false"));

    // Test assignment
    Optional<int> optAssign;
    optAssign = 123;
    Serial.println("After assignment, optAssign: " + String(optAssign.value()));
    
    optAssign = nullopt;
    Serial.println("After nullopt assignment, has value: " + String(optAssign.hasValue() ? "true" : "false"));

    Serial.println("-------- Optional Test Complete --------");

    // Test crash behavior - uncomment ONE of these lines to test crashing:
    
    // // Test 1: std::optional operator* (assert-based, may not crash)
    // Serial.println("std::optional operator* test:");
    // std::optional<int> stdOptInt = std::nullopt;
    // int d = *stdOptInt;  // May not crash - assert might be disabled
    // Serial.println("std::optional operator* completed, value: " + String(d));

    // // Test 2: std::optional .value() (exception-based, should crash)
    // Serial.println("std::optional .value() test:");
    // std::optional<int> stdOptInt2 = std::nullopt;
    // int d2 = stdOptInt2.value();  // Should crash with exception
    // Serial.println("std::optional .value() completed, value: " + String(d2));

    // // Test 3: Our Optional operator* (assert-based)
    // Serial.println("Our Optional operator* test:");
    // Optional<int> optAssignCrash;
    // int t = *optAssignCrash;  
    // Serial.println("Optional operator* completed, value: " + String(t));

    // // Test 4: Our Optional .value() (always crashes)
    // Serial.println("Our Optional .value() test:");
    // Optional<int> optAssignCrash2 = nullopt;
    // int t2 = optAssignCrash2.value(); 
    // Serial.println("Optional .value() completed, value: " + String(t2));

    // Serial.println("=== If you see this, no crashes occurred ===");
}

void optionalLoop() {
    // Nothing to do here for this test
    delay(1000); 
    // Serial.println("Optional loop running...");
}