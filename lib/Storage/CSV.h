#pragma once

#include <Arduino.h>

/**
 * number, name, data (column 0, column 1, column 2)
 * 1, name1, data1
 * 2, name2, data2
 * 3, name3, data3
 */

class CSV
{
private:
    String *csv;
    bool isFirstColumn;
    const char newLineChar = '\n';

public:
    /**
     * Reads and writes CSV data
     * @param csv Reference to string in csv format
     */
    CSV(String *csv)
        : csv(csv), isFirstColumn(true){};

    /**
     * Adds a new line character to the csv string to start a new row
     */
    void addNewLineChar()
    {
        *csv += newLineChar;
        isFirstColumn = true;
    }

    /**
     * Adds a single new element to the csv string
     * @param data Data to add to the csv string
     */
    template <typename T>
    void add(T data)
    {
        if (isFirstColumn)
            isFirstColumn = false;
        else
            *csv += ',';

        *csv += data;
    }

    /**
     * Adds final element and a new line char to the csv string
     */
    template <typename T>
    void addRow(T data)
    {
        *csv += data;
        *csv += newLineChar;
        isFirstColumn = true;
    }

    /**
     * Adds multiple new elements to the csv string
     * @param args Data to add to the csv string
     */
    template <typename T, typename... Args>
    void addRow(T data, Args... args)
    {
        *csv += data;
        *csv += ',';
        addRow(args...);
    }

    /**
     * Get the number of column in the csv string
     * @return Number of columns
     */
    uint16_t getColumnCount()
    {
        uint16_t currentColumn = 0;
        for (size_t i = 0; i < csv->length(); i++)
        {
            if (csv->charAt(i) == ',')
            {
                currentColumn++;
            }

            if (csv->charAt(i) == newLineChar)
                break;
        }
        return currentColumn + 1; //
    }

    /**
     * Get the number of rows in the csv string
     * @return Number of rows
     */
    uint16_t getRowCount()
    {
        uint16_t currentRow = 0;
        for (size_t i = 0; i < csv->length(); i++)
        {
            if (csv->charAt(i) == newLineChar)
            {
                currentRow++;
            }
        }
        return currentRow;
    }

    /**
     * Get the data at the column given
     * @param row Reference to a string array that will be used to store the data
     * @param columnIndex Index of the column to get the data from
     */
    void getRowData(String *row, uint16_t columnIndex)
    {
        uint16_t currentRow = 0;
        uint16_t currentColumn = 0;
        String currentData = "";

        for (size_t i = 0; i < csv->length(); i++)
        {
            if (csv->charAt(i) == newLineChar)
            {
                currentRow++;
                currentColumn = 0;
                currentData = "";
            }
            else if (csv->charAt(i) == ',')
            {
                currentColumn++;
                currentData = "";
            }
            else
            {
                currentData += csv->charAt(i);
            }

            if (currentColumn == columnIndex)
            {
                row[currentRow] = currentData;
            }
        }
    }

    /**
     * Get the data at the row given
     * @param column Reference to a string array that will be used to store the data
     * @param rowIndex Index of the row to get the data from
     */
    void getColumnData(String *column, uint16_t rowIndex)
    {
        uint16_t currentRow = 0;
        uint16_t currentColumn = 0;
        String currentData = "";

        for (size_t i = 0; i < csv->length(); i++)
        {
            if (csv->charAt(i) == newLineChar)
            {
                currentRow++;
                currentColumn = 0;
                currentData = "";
            }
            else if (csv->charAt(i) == ',')
            {
                currentColumn++;
                currentData = "";
            }
            else
            {
                currentData += csv->charAt(i);
            }

            if (currentRow == rowIndex)
            {
                column[currentColumn] = currentData;
            }
        }
    }

    /**
     * Get the data at the column and row given
     * @param data Reference to a string that will be used to store the data
     * @param columnIndex Index of the column to get the data from
     * @param rowIndex Index of the row to get the data from
     */
    void getDataAtColumnRow(String &data, uint16_t columnIndex, uint16_t rowIndex)
    {
        uint16_t currentRow = 0;
        uint16_t currentColumn = 0;
        String currentData = "";

        for (size_t i = 0; i < csv->length(); i++)
        {
            if (csv->charAt(i) == newLineChar)
            {
                currentRow++;
                currentColumn = 0;
                currentData = "";
            }
            else if (csv->charAt(i) == ',')
            {
                currentColumn++;
                currentData = "";
            }
            else
            {
                currentData += csv->charAt(i);
            }

            if (currentRow == rowIndex && currentColumn == columnIndex)
            {
                data = currentData;
            }
        }
    }
};
