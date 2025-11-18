Order Records (hashT Implementation)
=====================

A simple C++ console program that stores and looks up orders using a hash table
(`hashT<Order>`) 

Files
-----
- `hashT.h`              — template hash table class (linear probing)
- `orderRecords.cpp`     — main program (menu + Order struct)

Considerations
--------------
- Make sure `hashT.h` is in the same folder as `orderRecords.cpp`.
- The hash table size is small (11) to make collisions easy to see.
- Order numbers are auto-assigned and guaranteed unique.

Program Menu
------------
1. Insert new order  
2. Search for order by order number  
