#include <Arduino.h>

#include <SimpleTimer.h>
#include <DigitalOutput.h>
#include <Types.h>
#include <LinkedList.h>
#include <Benchmark.h>
#include <Logger.h>
#include <CSV.h>
#include <CTime.h>
#include <FSM.h>
#include <Button.h>
#include <CallbackButton.h>
#include <SimpleButton.h>
#include <StopWatch.h>

static const char *LOG_TAG = "[Main]";

// CTime ntpTime;

uint16 x = 0;
uint32 count = 0;

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
  LOG_DEBUG(LOG_TAG, "1");
  LOG_INFO(LOG_TAG, "2 %d %d", 222, 333);
  LOG_WARN(LOG_TAG, String("3 ") + "333");
  LOG_ERROR(LOG_TAG, "4");
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

void buttonTest()
{
  static Button button(2);

  button.check();

  if (button.released())
  {
    Serial.println("Button released");
  }

  if (button.pressed())
  {
    Serial.println("Button pressed");
  }

  if (button.clicked())
  {
    Serial.println("Button clicked 1");
  }

  if (button.doubleClicked())
  {
    Serial.println("Button clicked 2");
  }

  if (button.tripleClicked())
  {
    Serial.println("Button clicked 3");
  }

  if (button.longPressed())
  {
    Serial.println("Button long pressed");
  }

  if (button.longPressReleased())
  {
    Serial.println("Button long press released");
  }
}

void buttonCallbacks()
{
  static CallbackButton callbackButton(2);

  callbackButton.setClickedCallback([]()
                                    { Serial.println("Button clicked"); });
  callbackButton.setDoubleClickedCallback([]()
                                          { Serial.println("Button double clicked"); });
  callbackButton.setTripleClickedCallback([]()
                                          { Serial.println("Button triple clicked"); });
  callbackButton.setLongPressedCallback([]()
                                        { Serial.println("Button long pressed"); });
  callbackButton.setLongPressReleasedCallback([]()
                                              { Serial.println("Button long press released"); });
  callbackButton.setPressedCallback([]()
                                    { Serial.println("Button pressed"); });
  callbackButton.setReleasedCallback([]()
                                     { Serial.println("Button released"); });
}

void stopWatchTest()
{
  static StopWatch stopWatch;
  static Timer timer(1000);
  static SimpleButton button(2);
  static SimpleButton button2(3);

  if (button.pressed())
  {
    if (stopWatch.isRunning())
    {
      Serial.println("stop");
      stopWatch.stop();
    }
    else
    {
      Serial.println("start");
      stopWatch.start();
    }
  }

  if (stopWatch.getTime() > 5000)
  {
    Serial.println("reset");
    stopWatch.reset();
  }

  if (timer.ready())
  {
    Serial.println(stopWatch.getTime() / 1000);
  }
}

// State Machine Example:

void enterLedOn()
{
  // digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("LED ON");
}

void enterLedOff()
{
  // digitalWrite(LED_BUILTIN, LOW);
  Serial.println("LED OFF");
}

void handleLedBlinking()
{
  static unsigned long lastTime = 0;
  static bool ledState = false;

  if (millis() - lastTime > 500)
  {
    lastTime = millis();
    ledState = !ledState;
    // digitalWrite(LED_BUILTIN, ledState);
    Serial.println("LED BLINKING: " + String(ledState));
  }
}

State LedOn(enterLedOn, NO_HANDLE, NO_EXIT);
State LedOff(enterLedOff, NO_HANDLE, NO_EXIT);
State LedBlinking(NO_ENTER, handleLedBlinking, NO_EXIT);

FiniteStateMachine ledStateMachine(LedOff);

void setupExample()
{
  ledStateMachine.addTimedTransition(&LedOff, &LedOn, 1000);
  ledStateMachine.addTimedTransition(&LedOn, &LedBlinking, 1000);
  ledStateMachine.addTimedTransition(&LedBlinking, &LedOff, 5000);
}

void loopExample()
{
  ledStateMachine.run();
}

void setup()
{
  Serial.begin(115200);
  // button.init(D2, INPUT, LOW);
  // buttonCallbacks();
  // stopWatch.start();
  setupExample();
}

void loop()
{
  loopExample();

  // stopWatchTest();
  // callbackButton.run();
  // buttonTest();

  // delay(2000);
  // stateMachineTest();
  // benchmarkTest();
  // csvTest();
  // CircularLinkedListTest();
  // timerLedTest();
  // loggerTest();
  // timeTest();
  // timeTest2();
}