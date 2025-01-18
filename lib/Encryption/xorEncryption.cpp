#include "xorEncryption.h"

void XorEncryption::encrypt(char *data)
{
    xorEncryptDecrypt((uint8_t *)data, strlen(data));
}

void XorEncryption::decrypt(char *data)
{
    xorEncryptDecrypt((uint8_t *)data, strlen(data));
}

void XorEncryption::xorEncryptDecrypt(uint8_t *data, size_t length)
{
    for (size_t i = 0; i < length; i++)
    {
        uint8_t key = _keys[i % _numKeys];
        data[i] ^= key;
    }
}