#pragma once

// Line break
#define LINE_BREAK "------------------------------------------"
// Double line break
#define LINE_BREAK_2 "=========================================="

// return length of an array
#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))
// no operation
#define NOP() __asm__("nop")
// Join two symbols together
#define CONCAT(x, y) I_CONCAT(x, y)
#define I_CONCAT(x, y) x##y
// Turn symbol into string
#ifndef STRINGIFY
#define STRINGIFY_1(...) #__VA_ARGS__
#define STRINGIFY(...) STRINGIFY_1(__VA_ARGS__)
#endif