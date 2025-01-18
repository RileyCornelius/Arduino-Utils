#pragma once

#include <Arduino.h>

class XorEncryption
{
private:
    const uint8_t *_keys;
    size_t _numKeys;

    void xorEncryptDecrypt(uint8_t *data, size_t length);

public:
    XorEncryption(const uint8_t *keys, size_t numKeys) : _keys(keys), _numKeys(numKeys) {}

    void encrypt(char *data);
    void decrypt(char *data);
};