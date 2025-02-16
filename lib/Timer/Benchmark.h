#pragma once

class Benchmark
{
    const char *label;
    unsigned long startTime;
    Stream *serial;

    virtual uint32_t now() { return millis(); }

public:
    Benchmark(const char *l = "Benchmark", Stream &s = Serial) : label(l), serial(&s) {}

    void start()
    {
        startTime = now();
    }

    void end(const char *newLabel = nullptr)
    {
        if (newLabel)
            label = newLabel;

        serial->printf("%s: %lu ms\n", label, now() - startTime);
    }
};

class BenchmarkMicros : public Benchmark
{
    using Benchmark::Benchmark;

    uint32_t now() override { return micros(); }
};

// Benchmark Macros Settings

// Toggle debug benchmarking
#ifndef DEBUG_BENCHMARK
#define DEBUG_BENCHMARK 1
#endif

// Benchmark serial port
#ifndef DEBUG_BENCHMARK_SERIAL
#define DEBUG_BENCHMARK_SERIAL Serial
#endif

// Benchmark Macros

#if DEBUG_BENCHMARK
// Creates previous time with label, use same label with BENCHMARK_END(label) to print elapsed milliseconds
// label must be a symbol
#define BENCHMARK_BEGIN(label) I_BENCHMARK_BEGIN(CONCAT(_prevTime_, label))
#define I_BENCHMARK_BEGIN(prevTime) \
    static unsigned long prevTime;  \
    prevTime = millis();

// Prints elapsed time since BENCHMARK_BEGIN(label) in milliseconds
// label must be a symbol
#define BENCHMARK_END(label) I_BENCHMARK_END(CONCAT(_prevTime_, label), #label)
#define I_BENCHMARK_END(prevTime, label) DEBUG_BENCHMARK_SERIAL.printf("%s: %lu ms\n", label, millis() - prevTime);

// Creates previous time with label, use same label with BENCHMARK_MICROS_END(label) to print elapsed microseconds
// label must be a symbol
#define BENCHMARK_MICROS_BEGIN(label) I_BENCHMARK_MICROS_BEGIN(CONCAT(_prevTime_, label))
#define I_BENCHMARK_MICROS_BEGIN(prevTime) \
    static unsigned long prevTime;         \
    prevTime = micros();

// Prints elapsed time since BENCHMARK_MICROS_BEGIN(label) in microseconds
// label must be a symbol
#define BENCHMARK_MICROS_END(label) I_BENCHMARK_MICROS_END(CONCAT(_prevTime_, label), #label)
#define I_BENCHMARK_MICROS_END(prevTime, label) DEBUG_BENCHMARK_SERIAL.printf("%s: %lu us\n", label, micros() - prevTime);

#ifndef CONCAT
#define CONCAT(x, y) I_CONCAT(x, y)
#define I_CONCAT(x, y) x##y
#endif

#else
#define BENCHMARK_BEGIN(label)
#define BENCHMARK_END(label)

#define BENCHMARK_MICROS_BEGIN(label)
#define BENCHMARK_MICROS_END(label)

#endif