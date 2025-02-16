#include <Array.h>

char cArray[] = {1, 2, 3, 4, 5};
Array charArray = Array(cArray);

void passArray(Array<uint32_t> array)
{
    for (uint32_t i = 0; i < array.size(); i++)
    {
        Serial.println(array[i]);
    }
}

void arraySetup()
{
    int intArrayData[] = {1, 2, 3, 4, 5};
    Array intArray = Array(intArrayData);
    Serial.println(intArray.size());
}
void arrayLoop()
{
    uint32_t uintArray[] = {1, 2, 3};
    passArray(uintArray);
    delay(5000);
}