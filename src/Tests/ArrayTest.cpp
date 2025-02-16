#include <Array.h>

void outOfBounds()
{
    uint32_t uintArray[] = {1, 2, 3};
    Array<uint32_t> array = Array(uintArray);
    Serial.println(array[5]); // ! assert failed: index < length
}

void passArray(Array<uint32_t> array)
{
    for (uint32_t i = 0; i < array.size(); i++)
    {
        Serial.println(array[i]);
    }
}

Array<int> createArray()
{
    int intArrayData[] = {1, 2, 3, 4, 5}; // ! Bad intArrayData is stack allocated and will be destroyed after the function returns
    return Array<int>(intArrayData);
}

// ============================================================================= //

void arraySetup()
{
    int intArrayData[] = {1, 2, 3, 4, 5};
    Array intArray = Array(intArrayData);
    Serial.println(intArray.size());

    // Array<int> intArray2 = createArray();
    // outOfBounds();
}
void arrayLoop()
{
    uint32_t uintArray[] = {1, 2, 3};
    passArray(uintArray);
    delay(5000);
}