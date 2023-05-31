//
//  newSet.h
//  hw1
//
//  Created by Simba Chen on 1/21/23.
//

#ifndef NEWSET_INCLUDED
#define NEWSET_INCLUDED

#include <string>
using ItemType = std::string;

class Set
{
  public:
    Set();
    
    //Destructor
    ~Set();

    //Copy Constructor
    Set(const Set& other);
    
    //Assignment Operator
    Set& operator=(const Set& rhs);
    
    bool empty() const;  // Return true if the set is empty, otherwise false.

    int size() const;    // Return the number of items in the set.

    bool insert(const ItemType& value);
      // Insert value into the set if it is not already present.  Return
      // true if the value is actually inserted.  Leave the set unchanged
      // and return false if value is not inserted (perhaps because it
      // was already in the set or because the set has a fixed capacity and
      // is full).

    bool erase(const ItemType& value);
      // Remove the value from the set if it is present.  Return true if the
      // value was removed; otherwise, leave the set unchanged and
      // return false.
     
    bool contains(const ItemType& value) const;
      // Return true if the value is in the set, otherwise false.
     
    bool get(int i, ItemType& value) const;
      // If 0 <= i < size(), copy into value the item in the set that is
      // strictly greater than exactly i items in the set and return true.
      // Otherwise, leave value unchanged and return false.

    void swap(Set& other);
      // Exchange the contents of this set with the other one.
    
  private:
    struct Node{
        ItemType val;
        Node* prev;
        Node* next;
    };
    Node* head;
    Node* tail;
    int m_size;
    
};

void unite(const Set& s1, const Set& s2, Set& result);
//Store non-duplicate value of s1 and s2 in result

void butNot(const Set& s1, const Set& s2, Set& result);
//Store values in s1 that are not in s2 in result

#endif /* NEWSET_INCLUDED */
