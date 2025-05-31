#include <array>
#include <etl/array.h>
#include "Array.h"
#include "log.h"

void outOfBounds()
{
    uint32_t uintArray[] = {1, 2, 3};
    Array<uint32_t> array = Array<uint32_t>(uintArray);
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

// #include "fmt/ranges.h"
void arraySetup()
{
    // int intArrayData[] = {1, 2, 3, 4, 5};
    // Array<int> intArray = Array<int>(intArrayData);
    // Serial.println(intArray.size());

    // Array<int> intArray2 = createArray();
    // outOfBounds();

    std::array std = {1, 2, 3, 4, 5};
    etl::array etl = {1, 2, 3, 4, 5};

    etl.insert(etl.begin() + 2, 99);
    LOG_I("etl size: {}", etl.size());
    // LOG_I("etl array: [{}]", fmt::join(etl, ", "));

    std[2] = 99;
    LOG_D("std size: {}", std.size());
    // LOG_D("std array: {}", std)
}
void arrayLoop()
{
    // uint32_t uintArray[] = {1, 2, 3};
    // passArray(uintArray);
    // delay(5000);
}