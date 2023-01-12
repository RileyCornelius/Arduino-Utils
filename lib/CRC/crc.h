#pragma once

#include <Arduino.h>

uint16_t calculateCRC16(uint8_t *data, uint16_t length);
uint16_t calculateCRC16(String &data);
bool checkCRC16(String &data);