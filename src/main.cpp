#include <Arduino.h>

#include <Button.h>
#include <SimpleTimer.h>
#include <DigitalOutput.h>
#include <types.h>
#include <LinkedList.h>

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
  list.add(pStackNode);
  list.removeFirst(); // ! assert failed: multi_heap_free - caused by calling delete on a stack allocated object
}

void CircularLinkedListTest()
{
  LinkedList<int> list;
  list.add(1);
  list.add(new LinkedListNode<int>(2));
  list.add(new LinkedListNode<int>(3));
  list.add(new LinkedListNode<int>(4));
  list.add(5);
  list.remove(3); // remove index 3 - value 4

  LinkedListNode<int> *first = list.first();
  for (size_t i = 0; i < list.size(); i++)
  {
    Serial.println(first->value);
    first = first->next;
  }

  Serial.println("");

  LinkedListNode<int> *last = list.last();
  for (size_t i = 0; i < list.size(); i++)
  {
    Serial.println(last->value);
    last = last->prev;
  }

  Serial.println("-----------");
}

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  delay(1000);
  CircularLinkedListTest();

  timerLedTest();
}