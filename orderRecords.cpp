// Program: Order Records (hashT implementation)
// Author: Bruce Nebergall
// Last updated: 2025-11-17
// Purpose: Use the hashT template with Order records using a simple hash (orderNumber % tableSize). Order numbers are auto-assigned and unique. Stores cusomer name and order total. ChatGPT referenced in troubleshooting hashT class definition

#include <iostream>
#include <string>
#include <limits>
#include <iomanip>
#include "hashT.h"   // hashT template

using namespace std;

// Simple struct for Order Type
struct Order
{
    int    id;          // internal ID (insertion order number)
    string customerName;
    int    orderNumber; // external order number (hash key, auto-assigned)
    double orderTotal;  // order total in dollars

    bool operator==(const Order& other) const
    {
        return orderNumber == other.orderNumber;
    }

    bool operator!=(const Order& other) const
    {
        return !(*this == other);
    }
};

// For hashT::print(), define an output operator for Order.
ostream& operator<<(ostream& os, const Order& o)
{
    os << "Order ID: " << o.id
       << " | Customer: " << o.customerName
       << " | Order Number: " << o.orderNumber
       << " | Total: $" << fixed << setprecision(2) << o.orderTotal;
    return os;
}

//  Simple hash function for orderNumber -----
int simpleHash(int orderNumber, int size)
{
    return orderNumber % size;
}

int main()
{
    cout << "=======================\n";
    cout << "     Order Records    \n";
    cout << "=======================\n";

    const int TABLE_SIZE = 11;        // small size to see collisions
    hashT<Order> table(TABLE_SIZE);   // our hash table
    int nextInternalId   = 1;         // internal ID to label insertion order
    int nextOrderNumber  = 1000;      // starting external order number

    int choice = 0;
    while (true)
    {
        cout << "\n==== Records Table Menu ====\n"
             << "1. Insert new order\n"
             << "2. Search for order by order number\n"
             << "3. Remove order by order number\n"
             << "4. Print hash table\n"
             << "5. Exit\n"
             << "Enter choice: ";

        cin >> choice;

        if (!cin)
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == 1)
        {
            // Insert new order
            if (table.getLength() == table.getTableSize())
            {
                cout << "Hash table is full. Cannot insert new order.\n";
                continue;
            }

            Order o;
            o.id = nextInternalId++;

            cout << "Enter customer name: ";
            getline(cin, o.customerName);

            cout << "Enter order total (e.g., 49.99): ";
            cin >> o.orderTotal;
            if (!cin)
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid order total. Insert cancelled.\n";
                continue;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            // Auto-assign a unique orderNumber
            // Try numbers starting from nextOrderNumber until we find one that does not already exist in the table
            bool uniqueFound = false;
            while (!uniqueFound)
            {
                int candidate = nextOrderNumber;
                Order key;
                key.orderNumber = candidate;

                int index = simpleHash(candidate, TABLE_SIZE);
                bool found = false;

                table.search(index, key, found);

                if (!found)
                {
                    // This candidate is unique in the table
                    o.orderNumber   = candidate;
                    nextOrderNumber = candidate + 1;
                    uniqueFound     = true;

                    table.insert(o, simpleHash(o.orderNumber, TABLE_SIZE));
                    cout << "\nOrder inserted into hash table.\n";
                    cout << "Assigned order number: " << o.orderNumber << "\n";
                }
                else
                {
                    ++nextOrderNumber;
                }
            }
        }
        else if (choice == 2)
        {
            // Search for order by orderNumber
            int searchNumber;
            cout << "Enter order number to search for: ";
            cin >> searchNumber;

            if (!cin)
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid number. Search cancelled.\n";
                continue;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            Order key;
            key.orderNumber = searchNumber;   // key field

            int index = simpleHash(searchNumber, TABLE_SIZE);
            bool found = false;

            table.search(index, key, found);

            if (found)
            {
                Order foundOrder;
                table.retrieve(index, foundOrder);
                cout << "Order found at index " << index << ":\n";
                cout << foundOrder << "\n";
            }
            else
            {
                cout << "No order with order number " << searchNumber
                     << " found in the table.\n";
            }
        }
        else if (choice == 3)
        {
            // Remove order by orderNumber
            int removeNumber;
            cout << "Enter order number to remove: ";
            cin >> removeNumber;

            if (!cin)
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid number. Remove cancelled.\n";
                continue;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            Order key;
            key.orderNumber = removeNumber;

            int index = simpleHash(removeNumber, TABLE_SIZE);
            bool found = false;

            table.search(index, key, found);

            if (found)
            {
                table.remove(index, key);
                cout << "Order with order number " << removeNumber
                     << " removed (slot marked deleted).\n";
            }
            else
            {
                cout << "No order with order number " << removeNumber
                     << " found in the table.\n";
            }
        }
        else if (choice == 4)
        {
            // Print entire hash table
            cout << "\nHash table contents (occupied slots only):\n";
            table.print();
        }
        else if (choice == 5)
        {
            cout << "Exiting hash table test. Goodbye!\n";
            break;
        }
        else
        {
            cout << "Invalid choice. Please select 1–5.\n";
        }
    }

    return 0;
}
