#include <Arduino.h>

#include <Button.h>
#include <SimpleTimer.h>
#include <DigitalOutput.h>
#include <Types.h>
#include <LinkedList.h>
#define DEBUG_BENCHMARK 1
#include <Benchmark.h>
#define LOG_LEVEL LOG_LEVEL_WARN
#define LOG_LEVEL_MIN LOG_LEVEL_INFO
#include <Logger.h>
#include <CSV.h>

uint16 x = 0;
uint32 count = 0;

void buttonTest()
{
  static Button button = Button(2);
  if (button)
  {
    x++;
    Serial.println("Pressed " + x);
  }
}

void timerLedTest()
{
  static Timer timer = Timer(1000);
  static DigitalOutput led = DigitalOutput(3);
  if (timer)
  {
    led.toggle();
  }
}

void CircularLinkedListStackTest()
{
  LinkedList<int> list;
  LinkedListNode<int> stackNode = LinkedListNode<int>(1);
  LinkedListNode<int> *pStackNode = &stackNode;
  list.addLast(pStackNode);
  list.removeFirst(); // ! assert failed: multi_heap_free - caused by calling delete on a stack allocated object
}

void CircularLinkedListTest()
{
  LinkedList<int> list;
  list.addLast(1);
  list.addLast(new LinkedListNode<int>(2));
  list.addLast(new LinkedListNode<int>(3));
  list.addLast(new LinkedListNode<int>(4));
  list.addLast(5);
  list.remove(3); // remove index 3 - value 4

  LinkedListNode<int> *first = list.getFirst();
  for (size_t i = 0; i < list.size(); i++)
  {
    Serial.println(first->value);
    first = first->next;
  }

  Serial.println("");

  LinkedListNode<int> *last = list.getLast();
  for (size_t i = 0; i < list.size(); i++)
  {
    Serial.println(last->value);
    last = last->prev;
  }

  Serial.println("-----------");
}

void loggerTest()
{
  LOG_DEBUG("1");
  LOG_INFO("2 %d %d", 222, 333);
  LOG_WARN(String("3 ") + "333");
  LOG_ERROR("4");
}

void benchmarkTest()
{
  BENCHMARK_BEGIN();
  for (size_t i = 0; i < 100; i++)
  {
    analogRead(0);
  }
  BENCHMARK_END("100 analogReads: ");

  BENCHMARK_MICROS_BEGIN();
  for (size_t i = 0; i < 100; i++)
  {
    analogRead(0);
  }
  BENCHMARK_MICROS_END("100 analogReads: ");
}

void csvTest()
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
  Serial.printf("data: %s at column: %d, row: %d", dataAt.c_str(), columnIndex, rowIndex);
}

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  delay(2000);
  benchmarkTest();
  // csvTest();
  // CircularLinkedListTest();
  // timerLedTest();
  // loggerTest();
}