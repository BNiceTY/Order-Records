#ifndef H_hashT
#define H_hashT

#include <iostream>
#include <cassert>

// Class definition header file for hashT - Last modified 11/17/2025 - ChatGPT referenced on while troubleshooting errors on class definition
// Status codes for each table slot:
enum SlotStatus { EMPTY = 0, OCCUPIED = 1, DELETED = 2 };

template <class elemType>
class hashT
{
public:
    // Constructor: create a table with default size set to 101
    hashT(int size = 101);

    // Destructor
    ~hashT();

    // Insert a record using a given starting hash index
    void insert(const elemType& rec, int hashIndex);

    // Search for rec starting at hashIndex using linear probing.
    // On return: found == true if rec is present, and hashIndex is its location
    //            found == false and hashIndex is the first slot where rec could be inserted
    void search(int& hashIndex, const elemType& rec, bool& found) const;

    // Check whether the element at index loc equals rec (based on your key).
    bool isItemEqual(int loc, const elemType& rec) const;

    // Copy the element at loc into rec (assuming slot is OCCUPIED).
    void retrieve(int loc, elemType& rec) const;

    // Remove rec from the table (if present)
    void remove(int loc, const elemType& rec);

    // Print contents of table (occupied slots only).
    void print() const;

    // Some helpers
    int getTableSize() const { return HTSize; }
    int getLength()   const { return length; }

private:
    elemType*  HTable;          // array of elements
    int*       indexStatusList; // 0=empty, 1=occupied, 2=deleted
    int        HTSize;          // table size
    int        length;          // number of occupied elements
};

// Constructor
template <class elemType>
hashT<elemType>::hashT(int size)
{
    if (size <= 0)
        size = 101;

    HTSize  = size;
    length  = 0;

    HTable          = new elemType[HTSize];
    indexStatusList = new int[HTSize];

    for (int i = 0; i < HTSize; ++i)
        indexStatusList[i] = EMPTY;
}

// Destructor
template <class elemType>
hashT<elemType>::~hashT()
{
    delete[] HTable;
    delete[] indexStatusList;

    HTable          = nullptr;
    indexStatusList = nullptr;
}

// isItemEqual: delegate to operator==

template <class elemType>
bool hashT<elemType>::isItemEqual(int loc, const elemType& rec) const
{
    return (HTable[loc] == rec);
}

// search: linear probing
template <class elemType>
void hashT<elemType>::search(int& hashIndex,
                             const elemType& rec,
                             bool& found) const
{
    int location = hashIndex;
    int count    = 0;

    found = false;

    // Probe until we hit an EMPTY slot or all are checked
    while (indexStatusList[location] != EMPTY &&
           count < HTSize &&
           !found)
    {
        if (indexStatusList[location] == OCCUPIED &&
            isItemEqual(location, rec))
        {
            found     = true;
            hashIndex = location; // where found
        }
        else
        {
            location = (location + 1) % HTSize; // next slot (wrap)
            ++count;
        }
    }

    // If not found, hashIndex will point to first EMPTY or DELETED slot
    if (!found)
        hashIndex = location;
}

// insert: use search to find spot; if not found, insert there
template <class elemType>
void hashT<elemType>::insert(const elemType& rec, int hashIndex)
{
    if (length == HTSize)
    {
        std::cout << "Cannot insert into a full table.\n";
        return;
    }

    bool found = false;
    int  loc   = hashIndex;

    search(loc, rec, found);

    if (!found)
    {
        // loc is first available slot (EMPTY or DELETED)
        if (indexStatusList[loc] != OCCUPIED)
        {
            HTable[loc]          = rec;
            indexStatusList[loc] = OCCUPIED;
            ++length;
        }
        else
        {
            std::cout << "Collision resolution failed: no available slot.\n";
        }
    }
    else
    {

        HTable[loc] = rec;
    }
}

// retrieve: copy element at loc into rec
template <class elemType>
void hashT<elemType>::retrieve(int loc, elemType& rec) const
{
    assert(loc >= 0 && loc < HTSize);
    assert(indexStatusList[loc] == OCCUPIED);

    rec = HTable[loc];
}

// remove: mark slot as DELETED if rec is found
template <class elemType>
void hashT<elemType>::remove(int loc, const elemType& rec)
{
    bool found = false;
    int  index = loc;

    search(index, rec, found);

    if (found && indexStatusList[index] == OCCUPIED)
    {
        indexStatusList[index] = DELETED;
        --length;
    }
    else
    {
        std::cout << "Item to remove not found in the table.\n";
    }
}

// print: show all occupied slots
template <class elemType>
void hashT<elemType>::print() const
{
    for (int i = 0; i < HTSize; ++i)
    {
        if (indexStatusList[i] == OCCUPIED)
        {
            std::cout << "[" << i << "] ";
            std::cout << HTable[i] << "\n"; // requires operator<< for elemType
        }
    }
}

#endif // H_hashT
