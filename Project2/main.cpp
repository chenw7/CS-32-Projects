#include "Set.h"
#include <iostream>
#include <string>
#include <cassert>
using namespace std;

void testEmpty() {
    Set ss;
    assert(ss.empty()); // tests empty default constructor
    assert(ss.insert("abc"));
    assert(!ss.empty()); // shouldn't be ampty after inserting a node
    assert(ss.erase("abc"));
    assert(ss.empty()); // should be empty after erasing a node
}

// Tests size, insert, and erase
void testSize() {
    Set ss;
    assert(ss.size() == 0);
    assert(ss.insert("abc"));
    assert(ss.size() != 0 && ss.size() == 1); // node abc should have been added, size = 1
    assert(ss.insert("def"));
    assert(ss.size() == 2); // another node inserted, size = 2
    assert(!ss.insert("abc"));// attempt to insert duplicate
    assert(ss.size() != 3 && ss.size() == 2); // duplicate is not added, size = 2
    assert(ss.erase("abc"));
    assert(ss.size() != 3 && ss.size() == 1); // abc is removed, size = 1
    assert(!ss.erase("abc")); // attempts to erase nonexistant element
    assert(ss.size() == 1);
}

//Tests contains and erase
void testContains() {
    Set ss;
    assert(ss.insert("abc"));
    assert(ss.insert("def"));
    assert(ss.insert("ghi"));
    
    // check contains
    assert(ss.contains("abc"));
    assert(ss.contains("def"));
    assert(ss.contains("ghi"));
    assert(ss.size() == 3);
    
    // erase all nodes in linked list
    assert(ss.erase("def") && ss.size() == 2);
    assert(ss.erase("abc") && ss.size() == 1);
    assert(ss.erase("ghi") && ss.size() == 0);
    
    // check that nodes are erased
    assert(!ss.contains("abc"));
    assert(!ss.contains("def"));
    assert(!ss.contains("ghi"));
    assert(ss.empty());
}

void testGet()
{
    Set ss;
    assert(ss.insert("abc"));
    assert(!ss.insert("abc")); // attempt to insert duplicate
    assert(ss.insert("ghi")); // not inserted in lexiographical order, tests inserting in order
    assert(ss.insert("def"));
    
    // check get
    ItemType x = "";
    assert(ss.get(0, x) && (x == "abc"));
    assert(ss.get(1, x) && (x == "def"));
    assert(ss.get(2, x) && (x == "ghi"));
    
    // check after deleting items
    assert(ss.erase("abc"));
    assert(ss.get(0, x) && (x == "def"));
    assert(ss.get(1, x) && (x == "ghi"));
    assert(ss.erase("def"));
    assert(ss.get(0, x) && (x == "ghi"));
    
    assert(!ss.get(-6, x)); // checks when pos is negative
    assert(!ss.get(1, x)); // check when pos == ss.size()
    assert(!ss.get(100, x)); // check when pos > ss.size()
}

// Checks swap and copy constructor
void testSwap()
{
    Set ss;
    assert(ss.insert("abc"));
    assert(ss.insert("def"));
    assert(ss.insert("ghi"));
    assert(ss.size() == 3);
    
    // Check swap with empty set
    Set ss2;
    ss2.swap(ss);
    
    // ckeck ss2 swapped values
    assert(ss2.contains("abc"));
    assert(ss2.contains("def"));
    assert(ss2.contains("ghi"));
    assert(ss2.size() == 3); // s2 now contains ss values
    
    // check ss swapped values
    assert(!ss.contains("abc"));
    assert(!ss.contains("def"));
    assert(!ss.contains("ghi"));
    assert(ss.empty()); // ss now contians the empty set
    
    // Values inserted to ss aren't inserted to ss2
    assert(ss.insert("yo"));
    assert(ss.contains("yo"));
    assert(ss.size() == 1);
    assert(!ss2.contains("yo"));
    assert(ss2.size() == 3);
    
    // Check swap bwtween non empty sets
    ss2.swap(ss);
    assert(ss.contains("abc"));
    assert(ss.contains("def"));
    assert(ss.contains("ghi"));
    assert(!ss.contains("yo"));
    assert(ss.size() == 3);
    assert(ss2.contains("yo"));
    assert(!ss2.contains("abc"));
    assert(ss2.size() == 1);
}

void testUnite()
{
    Set ss, ss2, ss3, res;
    
    // checks when s1, s2, and res are empty
    unite(ss, ss2, res);
    assert(ss.empty() && ss2.empty() && res.empty());
    // ss: a, b, c
    assert(ss.insert("a") && ss.insert("b") && ss.insert("c"));
    
    // ss2: b, c, d, e
    assert(ss2.insert("b") && ss2.insert("c") && ss2.insert("d") && ss2.insert("e"));
    
    // ss3: d, e, f
    assert(ss3.insert("d") && ss3.insert("e") && ss3.insert("f"));
    
    // Checks when s1, s2, and result aren't empty
    unite(ss, ss2, ss3);
    assert(ss3.contains("a") && ss3.contains("b"));
    assert(ss3.contains("c") && ss3.contains("d") && ss3.contains("e")); // ss3: a, b, c, d, e
    assert(ss3.size() == 5);
    
    // Checks when result is empty
    unite(ss, ss2, res);
    assert(res.contains("a") && res.contains("b"));
    assert(res.contains("c") && res.contains("d") && res.contains("e"));
    assert(res.size() == 5);
    
    // Checks when s2 is empty and result isn't empty
    unite(ss, res, ss2);
    assert(ss2.contains("a") && ss2.contains("b"));
    assert(ss2.contains("c") && ss2.contains("d") && ss2.contains("e"));
    assert(ss2.size() == 5);
    
    // Checks when s1, s2, and result are the same set
    unite(ss, ss, ss);
    assert(ss.contains("a") && ss.contains("b") && ss.contains("c"));
    assert(ss.size() == 3);
    
    // Checks when s1 and result are the same set
    unite(ss, ss2, ss);
    assert(ss.contains("a") && ss.contains("b"));
    assert(ss.contains("c") && ss.contains("d") && ss.contains("e"));
    assert(ss.size() == 5);
}

void testbutNot()
{
    Set ss, ss2, res;
    
    // Tests when s1, s2, and result are empty
    butNot(ss, ss2, res);
    assert(ss.empty() && ss2.empty() && res.empty());
    
    // ss: a, b, c, d
    assert(ss.insert("a") && ss.insert("b") && ss.insert("c") && ss.insert("d"));
    
    // ss2: b, c, e
    assert(ss2.insert("b") && ss2.insert("c") && ss2.insert("e"));
    
    // Test when result is empty
    butNot(ss, ss2, res);
    assert(res.contains("a") && res.contains("d"));
    assert(res.size() == 2);
    
    // Test when s1, s2, and result are not empty
    // ss: a, b, c, d, res: a, d
    butNot(ss, res, ss2);
    assert(ss2.contains("b") && ss2.contains("c"));
    assert(ss2.size() == 2);
    
    // Checks when s1 and result are the same set
    butNot(ss, res, ss);
    assert(ss.contains("b") && ss.contains("c"));
    assert(ss.size() == 2);
    
    // Checks when s1, s2, and result are the same set
    // ss: b, c
    butNot(ss, ss, ss);
    assert(!ss.contains("b") && !ss.contains("c"));
    assert(ss.size() == 0);
}


int main()
{
    // default constructor & empty set behavior
    Set alberta;
    ItemType ai = "dummy";
    assert(alberta.empty());
    assert(alberta.size() == 0);
    assert(!alberta.contains(ai));
    assert(!alberta.get(0, ai) && ai == "dummy");
    
    // item insertion + duplicate checking for insertion
    ItemType bi = "";
    assert(alberta.insert(ai));
    assert(!alberta.empty());
    assert(alberta.size() == 1);
    assert(alberta.get(0, bi) && bi == "dummy");
    bi = "";
    assert(!alberta.get(1, bi) && bi == "");
    assert(alberta.contains("dummy"));
    assert(!alberta.insert("dummy"));
    assert(alberta.size() == 1);
    
    ai = "dummy2";
    assert(alberta.insert(ai));
    assert(alberta.size() == 2);
    assert(alberta.get(0, bi) && bi == "dummy");
    assert(alberta.get(1, bi) && bi == "dummy2");
    assert(alberta.contains("dummy"));
    assert(alberta.contains("dummy2"));
    
    // item deletion
    assert(!alberta.erase("dne"));
    assert(alberta.size() == 2);
    assert(alberta.contains("dummy"));
    assert(alberta.contains("dummy2"));
    assert(alberta.erase("dummy"));
    assert(!alberta.contains("dummy"));
    assert(alberta.contains("dummy2"));
    assert(alberta.size() == 1);
    assert(alberta.get(0, bi) && bi == "dummy2");
    assert(!alberta.get(1, bi) && bi == "dummy2");
    
    // larger example + ordering
    Set albertb;
    assert(albertb.insert("b"));
    assert(albertb.insert("a"));
    assert(albertb.insert("e"));
    assert(albertb.insert("c"));
    assert(albertb.insert("d"));
    assert(albertb.size() == 5);
    assert(albertb.get(0, bi) && bi == "a");
    assert(albertb.get(1, bi) && bi == "b");
    assert(albertb.get(2, bi) && bi == "c");
    assert(albertb.get(3, bi) && bi == "d");
    assert(albertb.get(4, bi) && bi == "e");
    
    // swap function
    alberta.swap(albertb);
    assert(alberta.size() == 5);
    assert(albertb.size() == 1);
    assert(alberta.get(0, bi) && bi == "a");
    assert(alberta.get(1, bi) && bi == "b");
    assert(alberta.get(2, bi) && bi == "c");
    assert(alberta.get(3, bi) && bi == "d");
    assert(alberta.get(4, bi) && bi == "e");
    assert(!alberta.get(5, bi) && bi == "e");
    assert(albertb.get(0, bi) && bi == "dummy2");
    assert(!albertb.get(1, bi) && bi == "dummy2");
    albertb.swap(albertb);
    assert(albertb.get(0, bi) && bi == "dummy2");
    assert(!albertb.get(1, bi) && bi == "dummy2");
    
    // copy constructor
    Set albertc = albertb;
    assert(albertc.erase("dummy2"));
    assert(albertc.empty());
    assert(!albertc.contains("dummy2"));
    assert(albertb.size() == 1);
    assert(albertb.contains("dummy2"));
    
    // assignment operator
    Set albertd;
    albertd = albertb;
    assert(albertd.erase("dummy2"));
    assert(albertd.empty());
    assert(!albertd.contains("dummy2"));
    assert(albertb.size() == 1);
    assert(albertb.contains("dummy2"));
    
    // unite
    Set alberte, albertf;
    alberte.insert("b");
    alberte.insert("a");
    alberte.insert("d");
    alberte.insert("c");
    albertf.insert("d");
    albertf.insert("a");
    albertf.insert("x");
    albertf.insert("z");
    albertf.insert("y");
    Set alberth;
    unite(alberte, albertf, alberth);
    assert(alberth.size() == 7);
    assert(alberth.contains("a"));
    assert(alberth.contains("b"));
    assert(alberth.contains("c"));
    assert(alberth.contains("d"));
    assert(alberth.contains("x"));
    assert(alberth.contains("y"));
    assert(alberth.contains("z"));
    unite(alberte, alberte, alberth);
    assert(alberth.size() == 4);
    assert(alberth.contains("a"));
    assert(alberth.contains("b"));
    assert(alberth.contains("c"));
    assert(alberth.contains("d"));
    unite(alberte, alberte, alberte);
    assert(alberte.size() == 4);
    assert(alberte.contains("a"));
    assert(alberte.contains("b"));
    assert(alberte.contains("c"));
    assert(alberte.contains("d"));
    unite(alberte, albertf, alberte);
    assert(alberte.size() == 7);
    assert(alberte.contains("a"));
    assert(alberte.contains("b"));
    assert(alberte.contains("c"));
    assert(alberte.contains("d"));
    assert(alberte.contains("x"));
    assert(alberte.contains("y"));
    assert(alberte.contains("z"));
    
    // butNot
    assert(alberte.erase("x"));
    assert(alberte.erase("y"));
    assert(alberte.erase("z"));
    butNot(alberte, albertf, alberth);
    assert(alberth.size() == 2);
    assert(alberth.contains("b"));
    assert(alberth.contains("c"));
    butNot(albertf, alberte, alberth);
    assert(alberth.size() == 3);
    assert(alberth.contains("x"));
    assert(alberth.contains("y"));
    assert(alberth.contains("z"));
    butNot(alberte, alberte, alberth);
    assert(alberth.empty());
    butNot(alberte, alberte, alberte);
    assert(alberte.empty());
    butNot(albertf, alberte, alberte);
    assert(alberte.contains("a"));
    assert(alberte.contains("d"));
    assert(alberte.contains("x"));
    assert(alberte.contains("y"));
    assert(alberte.contains("z"));
    butNot(alberte, albertf, alberte);
    assert(alberte.empty());
    
    cerr << "passed all tests" << endl;
    
    //Temporary Variable for Testing
    ItemType temp = "no";
    Set empty;
    
    //Empty, Size, and Insert Function Testing
    Set a;
    assert(a.size() == 0 && a.empty());         //Empty set
    a.insert("hello");                          //Testing insert
    assert(a.size() == 1);                      //Testing if successfully inserted
    a.insert("asdf");                           //Inserting another element
    assert(a.size() == 2);                      //Testing if successfully inserted
    
    
    //Erase Function Testing
    assert(a.erase("hello") && a.size() == 1); //Erase element (tail)
    assert(!a.erase("what") && a.size() == 1); //Erase non-existent
    assert(!empty.erase("noo"));               //Erasing from empty set
    assert(a.erase("asdf") && a.size() == 0);  //Erasing element (head)
    
    
    //Contains Function Testing
    a.insert("asdf");
    assert(a.contains("asdf") && !a.contains("hello")); //One element set
    assert(!empty.contains("hello"));                   //Empty set
    assert(a.insert("3") && a.contains("3"));           //Multi-element set
    
    
    //Get Function Testing
    Set getTest;                                    //Multi-element set
    getTest.insert("aaa");
    getTest.insert("bbb");
    getTest.insert("ccc");
    assert(getTest.get(0, temp) && temp == "aaa");      //Testing if get function operates normally
    assert(getTest.get(1, temp) && temp == "bbb");
    assert(getTest.get(2, temp) && temp == "ccc");
    assert(!getTest.get(3, temp) && temp == "ccc");     //Variable isn't changed when input invalid
    
    Set getT;                                       //One element set
    getT.insert("aaa");
    assert(getT.get(0, temp) && temp == "aaa");     //Getting element from one element set
    assert(!getT.get(1, temp) && temp == "aaa");    //Variable isn't changed when input invalid
    
    assert(!empty.get(0, temp) && temp == "aaa");    //Get returns false when set empty
    
    
    //Swap Function Testing
    Set swapTest;               //Swapping a normal set and an empty set
    Set swapT;
    swapTest.insert("a");
    swapTest.insert("b");
    swapTest.insert("c");
    swapTest.insert("d");
    
    swapTest.swap(swapT);
    assert(swapTest.size() == 0 && !swapTest.get(0, temp));
    assert(swapT.size() == 4 && swapT.get(3, temp) && temp == "d");
    
    Set swapT2;                 //Swapping a normal set a one element set
    swapT2.insert("hi");
    swapT2.swap(swapT);
    assert(swapT.get(0, temp) && temp == "hi");
    
    Set swapT3;                 //Swapping two empty sets
    Set swapT4;
    swapT3.swap(swapT4);
    assert(swapT3.empty() && swapT4.empty());
    
    //Unite Function Testing
    Set ss2;
    ss2.insert("ddd");
    ss2.insert("eee");
    ss2.insert("fff");
    
    Set result;             //Ensuring Unite deletes pre-existing elements
    result.insert("aaa");
    result.insert("bbb");
    result.insert("ccc");
    unite(empty, ss2, result);
    assert(result.size() == 3);     //Uniting with empty set
    
    Set test;
    test.insert("aaa");
    test.insert("ddd");             //Duplicate elements in both sets
    test.insert("eee");
    unite(ss2, test, result);       //Uniting two multi-element set
    assert(result.size() == 4 && result.contains("ddd"));
    
    
    //Copy Constructor Testing
    Set s;                  //Copying empty set
    Set k(s);
    assert(k.size() == 0 && !k.get(0, temp));
    
    Set ss;                 //Copying one element set
    ss.insert("aaa");
    Set i(ss);
    assert(i.size() == 1 && i.get(0, temp) && temp == "aaa");
    
    Set j;                  //Copying multi-element set
    j.insert("aaa");
    j.insert("bbb");
    j.insert("ccc");
    Set z(j);
    assert(z.size() == 3 && z.get(2, temp) && temp == "ccc");
    
    //Assignment Operator Testing
    Set t;                  //Empty set
    t.insert("zzz");
    t = s;
    assert(t.size() == 0 && !t.get(0, temp));
    t = t;                    //Check for self assignment
    
    Set v;                  //One Element set
    v = ss;
    assert(v.size() == 1 && v.get(0, temp) && temp == "aaa");
    v = v;
    
    Set u;                  //Multi-element set
    u.insert("zzz");
    u = z;
    assert(u.size() == 3 && u.get(2, temp) && temp == "ccc");
    u = u;
    
    //butNot Function Testing
    Set bn1;                                 //s1, s2, result are same set
    bn1.insert("a");
    bn1.insert("b");
    bn1.insert("c");
    butNot(bn1, bn1, bn1);
    assert(bn1.size() == 0);
    
    
    Set bnresult;                             //s1, s2 are same set (result different set)
    bn1.insert("a");
    bn1.insert("b");
    bn1.insert("c");
    butNot(bn1, bn1, bnresult);
    assert(bnresult.size() == 0);
    
    Set bn2;                                  //s1, result are same set (s2 different set)
    bn2.insert("a");
    bn2.insert("d");
    butNot(bn1, bn2, bn1);
    assert(bn1.size() == 2 && !bn1.contains("a") && bn2.size() == 2);
    
    butNot(bn1, bn2, bn2);                    //s2 and result are same set (s1 different set)
    assert(bn2.size() == 2);
    assert(bn2.contains("b"));
    assert(!bn2.contains("a"));

    
    cout << "Passed all test cases" << endl;
    
    Set sschi;
    assert(sschi.size()==0); //size should be 0
    ItemType schi = "capybara";
    assert(!sschi.get(0,schi)); //getting first element of empty array should return false
    assert(schi=="capybara"); //failed get attempt shouldn't change s
    assert(sschi.empty()); //s is empty so empty() should return true
    assert(!sschi.erase(schi)); //attempting to erase value that isn't include should return false
    assert(sschi.size()==0); //failed erase shouldn't decrease size
    assert(!sschi.contains(schi)); //nothing has been inserted so contains should return false
    
    //testing insert
    assert(sschi.insert("capybara")); //capybara isn't in the list so should return
    assert(sschi.size()==1); //size should've been increase by 1
    assert(!sschi.empty()); //ss is no longer empty
    assert(!sschi.insert("capybara")); //capybara is already in the set so should return false
    assert(sschi.size()==1); //size should be unchanged by failed insertion
    ItemType tchi = "hello";
    assert(sschi.get(0,tchi)); //getting item at index 0 should succeed
    assert(tchi == "capybara"); //t should be set to the inserted value
    assert(sschi.contains("capybara")); //the set now contains the inserted value
    assert(sschi.erase("capybara")); //erasing the contained value should return true
    assert(sschi.size()==0); //the successful erase should've decreased size by 1
    assert(!sschi.get(0,tchi)); //getting item at index 0 should now fail
    assert(!sschi.contains("capybara")); //the set no longer contains the inserted value
    assert(sschi.empty()); //ss is empty again
    
    sschi.insert("cat");
    //testing copy constructor
    Set s2chi = sschi;
    assert(s2chi.size()==1); //copy constructor should copy size
    assert(s2chi.get(0,schi)); //getting first value should succeed
    assert(schi=="cat"); //values should be correctly copied
    assert(!s2chi.get(1,schi)); //getting second value should fail
    sschi.insert("dog");
    assert(sschi.size()==2 && s2chi.size()==1); //size of copy constructed set should be independent
    assert(sschi.get(1,schi) && !s2chi.get(1,schi)); //content of sets should be independent
    assert(schi=="dog"); //s shouldve been set to dog
    
    //testing assignment operator
    Set s3chi;
    s3chi = sschi;
    assert(s3chi.size()==2); //assignment operator should copy size
    assert(s3chi.get(1,schi)); //getting second value should succeed
    assert(schi=="dog"); //values should be correctly copied
    assert(!s3chi.get(2,schi)); //getting third value should fail
    sschi.insert("frog");
    assert(sschi.size()==3 && s3chi.size()==2); //size of assigned set should be independent
    assert(sschi.contains("cat") && sschi.contains("frog") && sschi.contains("dog")); //checking if correct values are contained
    assert(s3chi.contains("cat") && s3chi.contains("dog") &&!s3chi.contains("frog")); //checking if correct values are contained
    assert(sschi.get(2,schi) && !s3chi.get(2,schi)); //content of sets should be independent
    assert(schi=="frog"); //s shouldve been set to dog
    
    //testing if value order is correct
    sschi.insert("egg");
    ItemType p1, p2, p3,p4;
    assert(sschi.get(0,p1) && sschi.get(1,p2) && sschi.get(2,p3) &&sschi.get(3,p4));
    assert((p1 == "cat") && (p2 =="dog") && (p3=="egg") && (p4=="frog")); //test if organized order is correct for get
    
    
    //testing swap
    //ss: {cat, dog, egg, frog}
    
    sschi.swap(sschi); //swapping with itself shouldn't produce errors or change values
    assert(sschi.size()==4); //checking size didn't get change
    assert(sschi.get(0,tchi));
    assert(tchi=="cat");//checking values didn't get changedv
    assert(sschi.contains("cat")); //checking contains works correctly
    assert(!sschi.empty()); //checking empty works correctly
    
    Set s4;
    s4.insert("pig");
    s4.insert("dog");
    s4.insert("goldfish");
    
    sschi.swap(s4); //swapping should swap size and node values
    assert(sschi.size()==3 && s4.size()==4); //sizes should be swapped correctly
    assert(sschi.get(0,p1) && sschi.get(1,p2) && sschi.get(2,p3) &&!sschi.get(3,p4));
    assert((p1 == "dog") && (p2 =="goldfish") && (p3=="pig"));
    assert(s4.get(0,p1) && s4.get(1,p2) && s4.get(2,p3) &&s4.get(3,p4));
    assert((p1 == "cat") && (p2 =="dog") && (p3=="egg") && (p4=="frog"));
    
    //testing unite
    //ss: {dog, goldfish, pig}
    //s4: {cat, dog, egg, frog}
    
    Set s5;
    s5.insert("random"); //unite should work with a set with other data
    unite(sschi,s4,s5);
    ItemType p5,p6;
    assert(s5.get(0,p1) && s5.get(1,p2) && s5.get(2,p3) &&s5.get(3,p4) && s5.get(4, p5) && s5.get(5,p6));
    assert(p1=="cat" && p2 == "dog" && p3 =="egg" && p4 =="frog" && p5=="goldfish" && p6=="pig"); //unite should unite the sets properly
    assert(s5.size()==6); //the new set should have the correct number of items
    
    Set emptySet;
    unite(sschi,s4,emptySet); //unite should work with an empty set
    assert(emptySet.get(0,p1) && emptySet.get(1,p2) && emptySet.get(2,p3) &&emptySet.get(3,p4) && emptySet.get(4, p5) && emptySet.get(5,p6));
    assert(p1=="cat" && p2 == "dog" && p3 =="egg" && p4 =="frog" && p5=="goldfish" && p6=="pig"); //unite should unite the sets properly
    assert(emptySet.size()==6); //the new set should have the correct number of items
    
    Set s6 = s4;
    unite(sschi, s6, s6); //unite should work even if two of the arguments are the same set
    assert(s6.get(0,p1) && s6.get(1,p2) && s6.get(2,p3) &&s6.get(3,p4) && s6.get(4, p5) && s6.get(5,p6));
    assert(p1=="cat" && p2 == "dog" && p3 =="egg" && p4 =="frog" && p5=="goldfish" && p6=="pig"); //unite should unite the sets properly
    assert(s6.size()==6); //the new set should have the correct number of items
    
    Set s7 = s4;
    unite(s7, sschi, s7); //unite should work even if two of the arguments are the same set
    assert(s7.get(0,p1) && s7.get(1,p2) && s7.get(2,p3) &&s7.get(3,p4) && s7.get(4, p5) && s7.get(5,p6));
    assert(p1=="cat" && p2 == "dog" && p3 =="egg" && p4 =="frog" && p5=="goldfish" && p6=="pig"); //unite should unite the sets properly
    assert(s7.size()==6); //the new set should have the correct number of items
    
    unite(s7, s7, s7); //unite should work even if all three arguments are the same set
    assert(s7.get(0,p1) && s7.get(1,p2) && s7.get(2,p3) &&s7.get(3,p4) && s7.get(4, p5) && s7.get(5,p6));
    assert(p1=="cat" && p2 == "dog" && p3 =="egg" && p4 =="frog" && p5=="goldfish" && p6=="pig"); //unite should unite the sets properly
    assert(s7.size()==6); //the new set should have the correct number of items
    
    
    //testing butNot
    //ss: {dog, goldfish, pig}
    //s4: {cat, dog, egg, frog}
    
    Set s8;
    s8.insert("random"); //butNot should work with a set with other data
    butNot(sschi,s4,s8);
    
    assert(s8.get(0,p1) && s8.get(1,p2) && !s8.get(2,p3)); //butNot should only have 2 items
    assert(p1=="goldfish" && p2 == "pig"); //butNot should have goldfish and pig in that order
    assert(s8.size()==2); //the new set should have the correct number of items
    
    Set emptySet2;
    unite(sschi,s4,emptySet2); //unite should work with an empty set
    butNot(sschi,s4,emptySet2);
    
    assert(emptySet2.get(0,p1) && emptySet2.get(1,p2) && !emptySet2.get(2,p3)); //butNot should only have 2 items
    assert(p1=="goldfish" && p2 == "pig"); //butNot should have goldfish and pig in that order
    assert(emptySet2.size()==2); //the new set should have the correct number of items
    
    Set s9 = s4;
    butNot(sschi,s9,s9); //butNot should work even if two of the parameters are the same set
    
    assert(s9.get(0,p1) && s9.get(1,p2) && !s9.get(2,p3)); //butNot should only have 2 items
    assert(p1=="goldfish" && p2 == "pig"); //butNot should have goldfish and pig in that order
    assert(s9.size()==2); //the new set should have the correct number of items
    
    Set s10 = sschi;
    butNot(s10,s4,s10); //butNot should work even if two of the parameters are the same set
    
    assert(s10.get(0,p1) && s10.get(1,p2) && !s10.get(2,p3)); //butNot should only have 2 items
    assert(p1=="goldfish" && p2 == "pig"); //butNot should have goldfish and pig in that order
    assert(s10.size()==2); //the new set should have the correct number of items
    
    
    butNot(s10,s10,s10); //butNot should work even if 3 of the parameters are the same set
    
    p1 = "testing1";
    p2 = "testing2";
    
    assert(!s10.get(0,p1) && !s10.get(1,p2) && !s10.get(2,p3)); //butNot should only have 2 items
    
    assert(p1=="testing1" && p2 == "testing2"); //butNot not have changed the values of p1 p2
    assert(s10.size()==0); //the new set should have the correct number of items
    
    cout << "passed chiwei" << endl;
    
    testEmpty();
    testSize();
    testContains();
    testGet();
    testSwap();
    testUnite();
    testbutNot();
    cout << "Passed all tests" << endl;
}
