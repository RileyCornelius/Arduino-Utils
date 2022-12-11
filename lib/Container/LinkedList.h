#pragma once

#include <Arduino.h>

/*---------------------------------------------------------------------------------------
 * Example
 *-------------------------------------------------------------------------------------*/
//   LinkedList<int> list;
//   list.add(1);
//   list.add(2);
//   list.add(3);
//   list.removeFirst();
//   Serial.println(list.first()->value); // prints 2

/*---------------------------------------------------------------------------------------
 * Node for Linked List
 *-------------------------------------------------------------------------------------*/
template <class T>
class LinkedListNode
{
public:
    T value;
    LinkedListNode *next;
    LinkedListNode *prev;
    LinkedListNode(T value) : next(nullptr), prev(nullptr), value(value) {}
};

/*---------------------------------------------------------------------------------------
 * Circular Doubly Linked List
 *-------------------------------------------------------------------------------------*/
template <class T>
class LinkedList
{
private:
    LinkedListNode<T> *head;
    uint16_t count;

    void insertNodeToEmptyList(LinkedListNode<T> *node)
    {
        // Set the head and tail to the new node
        head = node;
        head->next = node;
        head->prev = node;
        // Keep track of the count of nodes
        count++;
    }

    void insertNodeBefore(LinkedListNode<T> *node, LinkedListNode<T> *newNode)
    {
        // Link new node to head and tail
        newNode->next = node;       // make list circular
        newNode->prev = node->prev; // link new node to previous tail
        // Link head to new node
        node->prev->next = newNode; // set tail without a tail member var
        node->prev = newNode;       // link head to new tail
        // Keep track of the count of nodes
        count++;
    }

    void removeNode(LinkedListNode<T> *deleteNode)
    {
        if (head == nullptr) // List is empty
        {
            return;
        }

        if (head == head->next) // List has only one element
        {
            // Delete last node and reset to an empty list
            delete head;
            head = NULL;
        }
        else // List has multiple nodes
        {
            // Remove node from the list
            LinkedListNode<T> *temp = deleteNode;
            deleteNode->next->prev = deleteNode->prev; // Replace the next nodes previous pointer with the deleted nodes prev pointer
            deleteNode->prev->next = deleteNode->next; // Replace the previous nodes next pointer to the deleted nodes next pointer
            if (head == deleteNode)
            {
                head = deleteNode->next; // If new node is also the head replace the head with the next node
            }
            delete temp;
        }
        // Keep track of the count of nodes
        count--;
    }

public:
    LinkedList() : head(nullptr), count(0) {}

    // TODO: ERRORS - corrupt heap: assert failed: multi_heap_free multi_heap_poisoning
    // ~LinkedList()
    // {
    //     // Delete all the elements in the list
    //     while (head != nullptr)
    //     {
    //         LinkedListNode<T> *temp = head;
    //         head = head->next;
    //         delete temp;
    //     }
    // }

    /**
     * \return Pointer to first element of the list
     */
    LinkedListNode<T> *first()
    {
        return head;
    }

    /**
     * \return Pointer to last element of the list
     */
    LinkedListNode<T> *last()
    {
        return head->prev;
    }

    /**
     * \return Size of the list
     */
    uint16_t size()
    {
        return count;
    }

    /**
     * \brief Adds a node to the end of the list
     * \param node Linked list node to be stored
     */
    void add(LinkedListNode<T> *node)
    {
        if (head == nullptr) // If the list is empty
        {
            insertNodeToEmptyList(node);
        }
        else
        {
            insertNodeBefore(head, node);
        }
    }

    /**
     * \brief Adds an element to the end of the list
     * \param value Data to be stored in the new node
     */
    void add(T value)
    {
        // Create new node with past value
        LinkedListNode<T> *node = new LinkedListNode<T>(value);

        add(node);
    }

    /**
     * \brief Adds a node at the given index
     * \param node Linked list node to be stored
     * \param index The index of where to store to node
     */
    void add(LinkedListNode<T> *node, uint16_t index)
    {
        if (index >= count)
            return;

        if (head == nullptr) // If the list is empty
        {
            insertNodeToEmptyList(node);
        }
        else
        {
            insertNodeBefore(get(index), node);
        }
    }

    // Add an element at the given index

    /**
     * \brief Adds an element at the given index
     * \param value Data to be stored in the new node
     * \param index The index of where to store to node
     */
    void add(T value, uint16_t index)
    {
        // Create new node with past value
        LinkedListNode<T> *node = new LinkedListNode<T>(value);

        add(node);
    }

    /**
     * \brief Adds a new node to the beginning of the list
     * \param node Linked list node to be stored
     */
    void addFirst(LinkedListNode<T> *node)
    {
        if (head == nullptr) // If the list is empty
        {
            insertNodeToEmptyList(node);
        }
        else
        {
            insertNodeBefore(head, node);
            head = node; // Move new node to the head
        }
    }

    /**
     * \brief Adds an element to the beginning of the list
     * \param value Data to be stored in the new node
     */
    void addFirst(T value)
    {
        // Create new node with past value
        LinkedListNode<T> *node = new LinkedListNode<T>(value);

        addFirst(node);
    }

    /**
     * \brief Removes the head node from the list
     */
    void removeFirst()
    {
        removeNode(head);
    }

    /**
     * \brief Removes the tail node from the list
     */
    void removeLast()
    {
        removeNode(head->prev);
    }

    /**
     * \brief Removes the node at the given index
     */
    void remove(uint16_t index)
    {
        removeNode(get(index));
    }

    /**
     * \brief Gets a node at the given index
     * \param index The index of where the node is stored
     * \return Pointer to node from the linked list
     */
    LinkedListNode<T> *get(uint16_t index)
    {
        if (index >= count) // Index out of bounds
        {
            return nullptr;
        }

        LinkedListNode<T> *current = head;
        for (size_t i = 0; i < index; i++)
        {
            current = current->next;
        }

        return current;
    }
};