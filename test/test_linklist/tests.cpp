// More information about PlatformIO Unit Testing:
// https://docs.platformio.org/en/latest/advanced/unit-testing/index.html

#include <unity.h>
#include <Arduino.h>
#include <LinkedList.h>
// #include <SimpleTimer.h>
// #include <DigitalOutput.h>

/*------------------------------------------------------------------------------
 * TESTS
 *----------------------------------------------------------------------------*/

// void test_function_true(void)
// {
//     TEST_ASSERT_EQUAL(32, 32);
// }

// void test_function_false(void)
// {
//     TEST_ASSERT_EQUAL(20, 0);
// }

void linkedListTest()
{
	LinkedList<int> list;
	list.addFirst(new LinkedListNode<int>(1));
	list.addLast(new LinkedListNode<int>(2));
	list.addLast(new LinkedListNode<int>(3));
	list.addLast(4);
	list.addLast(5);

	list.removeFirst();
	list.removeLast();

	LinkedListNode<int>* first = list.getFirst();
	LinkedListNode<int>* last = list.getLast();

	TEST_ASSERT_EQUAL(first->value, last->next->value); // is circular
	TEST_ASSERT_EQUAL(3, list.size()); // size correct
	TEST_ASSERT_EQUAL(3, list.get(1)->value); // get correct index
}

/*------------------------------------------------------------------------------
 * SETUP
 *----------------------------------------------------------------------------*/

// set stuff up here
void setUp(void)
{
}

// clean stuff up here
void tearDown(void)
{
}

// tests here
void tests()
{
	RUN_TEST(linkedListTest);
}

void setup()
{
	// NOTE!!! Wait for >2 secs
	// if board doesn't support software reset via Serial.DTR/RTS
	delay(3000);

	UNITY_BEGIN();
	tests();
	UNITY_END();
}

void loop()
{
}
