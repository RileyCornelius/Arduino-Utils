#pragma once

// return length of an array
#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))
// no operation
#define NOP() __asm__("nop");
// Join two symbols together
#define CONCAT(x, y) I_CONCAT(x, y)
#define I_CONCAT(x, y) x##y