#pragma once
#include <Arduino.h>

#define b2kb(b) ((float)b / 1024.0)
#define b2mb(b) ((float)b / (1024.0 * 1024.0))
void printMemCapsInfo()
{
    multi_heap_info_t info;
    size_t total = heap_caps_get_total_size(MALLOC_CAP_INTERNAL);
    heap_caps_get_info(&info, MALLOC_CAP_INTERNAL);
    Serial.printf("Internal Memory Info:\n");
    Serial.printf("------------------------------------------\n");
    Serial.printf("  Total Size        : %8u B (%6.1f KB)\n", total, b2kb(total));
    Serial.printf("  Free Bytes        : %8u B (%6.1f KB)\n", info.total_free_bytes, b2kb(info.total_free_bytes));
    Serial.printf("  Allocated Bytes   : %8u B (%6.1f KB)\n", info.total_allocated_bytes, b2kb(info.total_allocated_bytes));
    Serial.printf("  Minimum Free Bytes: %8u B (%6.1f KB)\n", info.minimum_free_bytes, b2kb(info.minimum_free_bytes));
    Serial.printf("  Largest Free Block: %8u B (%6.1f KB)\n", info.largest_free_block, b2kb(info.largest_free_block));
}