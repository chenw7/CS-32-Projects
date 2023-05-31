//
//  Set.cpp
//  Project2
//
//  Created by Simba Chen on 1/23/23.
//

#include "Set.h"
#include <iostream>

Set::Set()
{
    head = new Node;
    head->prev = nullptr;
    head->next = nullptr;
    tail = head;
    m_size = 0;
}

Set::~Set()
{
    Node* p = head;
    while(p->next != nullptr){
        p = p->next;
        delete p->prev;
    }
    delete p;
}

Set::Set(const Set& other)
{
    //No Elements
    head = new Node;
    head->prev = nullptr;
    head->next = nullptr;
    tail = head;
    m_size = 0;
    
    //More than 1 Element
    ItemType temp;
    for(int i = 0; i < other.size(); i++){
        other.get(i, temp);
        this->insert(temp);
    }
}

Set& Set::operator=(const Set& rhs) //SHOULD BE DONE
{
    if(this != &rhs){
        Set temp = rhs;
        swap(temp);
    }
    return *this;
}

bool Set::empty() const
{
    return (m_size == 0);
}

int Set::size() const
{
    return m_size;
}

bool Set::insert(const ItemType& value)
{
    if(contains(value))
        return false;
    
    //Inserting value at front of linked list and pointing head to new Node
    Node *p = new Node;
    p->val = value;
    p->next = head;
    p->prev = nullptr;
    head->prev = p;
    head = p;
    
    if(m_size == 0){
        tail = head->next;
    }
    
    m_size++;
    return true;
}

bool Set::erase(const ItemType& value)
{
    if(m_size == 0 || !contains(value))
        return false;
    
    Node* p = head;
    while(p != nullptr){
        if(p->val == value){
            //If the value we are searching for is contained in the head node
            if(p == head){
                head = p->next;
                head->prev = nullptr;
                delete p;
            }
            //If the value we are searching for is contained in the tail node
            else if(p == tail){
                Node* temp = tail->prev;
                delete tail;
                tail = temp;
                tail->next = nullptr;
            }
            //If the value we are searching for is contained in the middle of the list
            else{
                p->prev->next = p->next;
                p->next->prev = p->prev;
                delete p;
            }
            m_size--;
            return true;
        }
        p = p->next;
    }
    return true;
}

bool Set::contains(const ItemType& value) const
{
    Node* p = head;
    //Loop through whole linked list and find value
    while(p != nullptr){
        if(p->val == value)
            return true;
        p = p->next;
    }
    return false;
}

bool Set::get(int i, ItemType& value) const
{
    if(i < 0 || i >= m_size)
        return false;
    
    int count;
    //For each node, loop through whole linked list and compare with other values
    //If the number of items larger than matches i, make value variable that node's value
    for(Node* p = head; p->next != nullptr; p = p->next){
        count = 0;
        for(Node* np = head; np->next != nullptr; np = np->next){
            if(p->val > np->val)
                count++;
        }
        if(count == i){
            value = p->val;
        }
    }
    return true;
}

void Set::swap(Set& other)
{
    //Create temporary variables to store this set's pointers and size
    Node* tempHead = head;
    Node* tempTail = tail;
    int tempSize = m_size;
    
    //Swapping pointers and size for the two sets
    head = other.head;
    tail = other.tail;
    m_size = other.m_size;
    
    other.head = tempHead;
    other.tail = tempTail;
    other.m_size = tempSize;
}

void unite(const Set& s1, const Set& s2, Set& result)
{
    ItemType temp;
    int oriSize = result.size();
    
    //Deleting everything in result array (only if result is not one of them)
    if(&s1 != &result && &s2 != &result){
        for(int i = 0; i < oriSize; i++){
            result.get(0, temp);
            result.erase(temp);
        }
    }
    
    //Adding everything from s1 and s2 to result
    for(int j = 0; j < s1.size(); j++){
        if(s1.get(j, temp))
            result.insert(temp);
    }
    
    //Adding non-duplicate data from s2 to result
    for(int k = 0; k < s2.size(); k++){
        if(s2.get(k, temp))
            result.insert(temp);
    }
}

void butNot(const Set& s1, const Set& s2, Set& result)
{
    ItemType temp;
    int oriSize = result.size();
    
    //s1 is same set as s2 => delete everything (even if result is also same)
    if(&s1 == &s2){
        for(int i = 0; i < oriSize; i++){
            result.get(0, temp);
            result.erase(temp);
        }
        return;
    }
    
    //s1, result are same, but s2 different
    if(&s1 == &result && &s1 != &s2){
        for(int i = 0; i < s2.size(); i++){
            s2.get(i, temp);
            result.erase(temp);
        }
        return;
    }
    
    //s2, result are same, but s1 different
    if(&s2 == &result && &s2 != &s1){
        for(int i = 0; i < s1.size(); i++){
            s1.get(i, temp);
            if(result.contains(temp)){
                result.erase(temp);
            }
            else{
                result.insert(temp);
            }
        }
        
        for(int j = 0; j < s2.size(); j++){
            s2.get(j, temp);
            if(!s1.contains(temp)){
                result.erase(temp);
                j--;
            }
        }
        return;
    }
    
    //Deleting everything in result array
    for(int i = 0; i < oriSize; i++){
        result.get(0, temp);
        result.erase(temp);
    }
    
    //Looping through s1 and checking if s2 contains same value
    for(int j = 0; j < s1.size(); j++){
        s1.get(j, temp);
        if(!s2.contains(temp)){
            result.insert(temp);
        }
    }
}
