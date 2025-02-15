#include <Arduino.h>
#include <CSV.h>

void csvSetup()
{
    Serial.println();
    Serial.println("-------- Create CSV --------");
    String csv;
    CSV Csv = CSV(&csv);
    Csv.addRow("Hellos", "int", "float"); // Header
    Csv.addRow("Hello1", 1, 1.11);
    Csv.add("Hello2");
    Csv.add(2);
    Csv.add(2.22);
    Csv.addNewLineChar();
    Csv.addRow("Hello3", 3, 3.33);
    Csv.addRow("Hello4", 4, 4.44);
    Serial.print(csv);

    Serial.println();
    Serial.println("-------- Parse Rows CSV --------");
    uint16_t rowNums = Csv.getRowCount();
    Serial.println(String("row count: ") + rowNums);
    String row[rowNums];
    uint8_t rowIndex = 1;
    Serial.println(String("data at index: ") + rowIndex);
    Csv.getRowData(row, rowIndex);
    for (size_t i = 0; i < rowNums; i++)
    {
        Serial.println(row[i]);
    }

    Serial.println();
    Serial.println("-------- Parse Columns CSV --------");
    uint16_t columnNums = Csv.getColumnCount();
    Serial.println(String("column count: ") + columnNums);
    String column[columnNums];
    uint8_t columnIndex = 1;
    Serial.println(String("data at column: ") + columnIndex);
    Csv.getColumnData(column, columnIndex);
    for (size_t i = 0; i < columnNums; ++i)
    {
        Serial.println(column[i]);
    }

    Serial.println();
    Serial.println("-------- Parse at Column/Row CSV --------");
    String dataAt;
    columnIndex = 0;
    rowIndex = 2;
    Csv.getDataAtColumnRow(dataAt, columnIndex, rowIndex);
    // Serial.printf("data: %s at column: %d, row: %d", dataAt.c_str(), columnIndex, rowIndex);
}

void csvLoop()
{
}