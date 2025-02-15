#include <Arduino.h>
#include <Benchmark.h>

void benchmarkSetup()
{
    BENCHMARK_BEGIN(Read);
    for (size_t i = 0; i < 100; i++)
    {
        analogRead(0);
    }
    BENCHMARK_END(Read);

    BENCHMARK_MICROS_BEGIN(ReadMicro);
    for (size_t i = 0; i < 100; i++)
    {
        analogRead(0);
    }
    BENCHMARK_MICROS_END(ReadMicro);
}

void benchmarkLoop()
{
}