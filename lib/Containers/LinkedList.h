#pragma once

#include <stdint.h>

// * Note: Adding a stack allocated node to the list will cause a crash on deletion

/**--------------------------------------------------------------------------------------
 * Example
 *-------------------------------------------------------------------------------------*/

//   LinkedList<int> list;
//   list.addLast(1);
//   list.addLast(2);
//   list.addLast(3);
//   list.removeFirst();
//   Serial.println(list.getFirst()->value); // prints 2

/**--------------------------------------------------------------------------------------
 * Node for Linked List
 *-------------------------------------------------------------------------------------*/

/**
 * Node for circular doubly linked list
 * \tparam T Node type
 * \param value Node data
 */
template <class T>
class LinkedListNode
{
public:
    T value;
    LinkedListNode *next;
    LinkedListNode *prev;
    LinkedListNode(T value) : next(nullptr), prev(nullptr), value(value) {}
};

/**--------------------------------------------------------------------------------------
 * Circular Doubly Linked List
 *-------------------------------------------------------------------------------------*/

/**
 * Circular doubly linked list
 * \tparam T LinkedListNode type
 */
template <class T>
class LinkedList
{
private:
    LinkedListNode<T> *head;
    uint16_t count;

    void insertNodeToEmptyList(LinkedListNode<T> *newNode)
    {
        // Set the head and tail to the new node
        head = newNode;
        head->next = newNode;
        head->prev = newNode;
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
    ~LinkedList()
    {
        clear();
    }

    /**
     * Clears the list
     */
    void clear()
    {
        LinkedListNode<T> *node = head;
        for (size_t i = 0; i < count; i++)
        {
            removeNode(node);
            node = node->next;
        }
        head = nullptr;
    }

    /**
     * \return Size of the list
     */
    uint16_t size()
    {
        return count;
    }

    /**
     * Adds a node at the given index
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

    /**
     * Adds an element at the given index
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
     * Adds a node to the beginning of the list
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
            insertNodeBefore(head, node); // Add new node to the tail
            head = node;                  // Then make new node the head
        }
    }

    /**
     * Adds an element to the beginning of the list
     * \param value Data to be stored in the new node
     */
    void addFirst(T value)
    {
        // Create new node with past value
        LinkedListNode<T> *node = new LinkedListNode<T>(value);
        addFirst(node);
    }

    /**
     * Adds a node to the end of the list
     * \param node Linked list node to be stored
     */
    void addLast(LinkedListNode<T> *node)
    {
        if (head == nullptr) // If the list is empty
        {
            insertNodeToEmptyList(node);
        }
        else
        {
            insertNodeBefore(head, node); // Add new node to the tail
        }
    }

    /**
     * Adds an element to the end of the list
     * \param value Data to be stored in the new node
     */
    void addLast(T value)
    {
        // Create new node with past value
        LinkedListNode<T> *node = new LinkedListNode<T>(value);
        addLast(node);
    }

    /**
     * Removes the given node
     * \param node Linked list node to be removed
     */
    void remove(LinkedListNode<T> *node)
    {
        removeNode(node);
    }

    /**
     * Removes the node at the given index
     * \param index The index of where the node is stored
     */
    void remove(uint16_t index)
    {
        removeNode(get(index));
    }

    /**
     * Removes the node from the beginning of the list
     */
    void removeFirst()
    {
        removeNode(head);
    }

    /**
     * Removes the node from the end of the list
     */
    void removeLast()
    {
        removeNode(head->prev);
    }

    /**
     * Gets a node at the given index
     * \param index The index of where the node is stored
     * \return Pointer to node at the given index from the list
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

    /**
     * Gets the beginning node
     * \return Pointer to first node of the list
     */
    LinkedListNode<T> *getFirst()
    {
        return head;
    }

    /**
     * Gets the ending node
     * \return Pointer to last node of the list
     */
    LinkedListNode<T> *getLast()
    {
        return head->prev;
    }
};