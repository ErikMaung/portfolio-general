#include "Set.h"
#include <string>
#include <iostream>
#include <cassert>
using namespace std;

void test()
{
    Set s1; // test insert function
    assert(s1.insert("1"));
    s1.insert("2");
    s1.insert("3");
    Set s2;
    s2.insert("5");
    s2.insert("6");
    assert(s2.insert("3")); // test bool value insert
    s2.insert("4");
    assert(!s2.insert("4")); // test bool value insert
    assert(s2.insert("99"));
    Set res;
    res.insert("1");
    cout << "1: ";
    unite(res, res, res); // test unite between same referenced Set
    cout << res.size() << endl;
    cout << "3: ";
    unite(s1, res, res); // test unite (different same same)
    cout << res.size() << endl;
    Set res2;
    res2.insert("1");
    cout << "6: ";
    unite(res2, s2, res2); // test unite (same different same)
    cout << res2.size() << endl;
    cout << "7: ";
    unite(s2, s1, res2); // test unite, not special
    cout << res2.size() << endl;

    Set aa1;
    Set a2;
    Set r;
    r.insert("0");
    assert(r.size() == 1);
    aa1.insert("A");
    aa1.insert("B");
    assert(aa1.size() == 2);

    Set a1(aa1);
    assert(a1.size() == 2); // test copy constructor
    cout << "0: ";
    butNot(a1, a1, r);
    cout << r.size() << endl; // butNot test a1 excluding a1 -> nothing ends up in result
    cout << "0: ";
    butNot(a2, a2, r);
    cout << r.size() << endl; // butNot test a2 excluding a2 -> nothing ends up in result
    a2.insert("A");
    a2.insert("B");
    a2.insert("C");
    a2.insert("D"); // size == 4
    
    Set aa2;
    aa2.insert("ERROR");
    assert(aa2.size() == 1);
    aa2 = a2;
    assert(aa2.size() == 4);
    ItemType testAA2;
    for (int i = 0; i < aa2.size(); i++) {
        a2.get(i, testAA2);
        cout << testAA2 << " ";
        aa2.get(i, testAA2);
        cout << "== " << testAA2 << endl;
    }
    assert(a2.contains("A"));
    assert(aa2.contains("A"));

    aa2.erase("A");
    assert(aa2.size() == 3);
    assert(a2.size() == 4);
    assert(!aa2.erase("ERROR"));
    cout << "1: ";
    butNot(a1, aa2, r);
    cout << r.size() << endl;
    cout << "2: ";
    butNot(aa2, a1, r);
    cout << r.size() << endl;

    Set ss;  // ItemType is std::string
    ss.insert("ccc");
    ss.insert("aaa");
    ss.insert("bbb");
    ItemType x = "xxx";
    assert(!ss.get(3, x) && x == "xxx");  // x is unchanged
    assert(ss.get(1, x) && x == "bbb");   // "bbb" is greater than
    // exactly 1 item
    
    Set z;
    assert(z.size() == 0);
    z.insert("[");
    z.insert("|");
    z.insert("]");
    Set zz;
    zz.insert("[[");
    zz.insert("||");
    unite(z, z, z);
    assert(z.size() == 3);
    unite(zz, z, z);
    assert(zz.size() == 2 && z.size() == 5);
    butNot(z, zz, z);
    assert(z.size() == 3 && zz.size() == 2);
    unite(z, zz, z);
    assert(z.size() == 5);
    butNot(zz, z, z);
    assert(z.size() == 0);
    assert(zz.size() == 2);

    Set y = z;
    y = zz;
    z = y;
    Set yy(zz);
    
    assert(z.size() == 2 && yy.size() == 2 && zz.size() == 2 && y.size() == 2);
    assert(y.contains("[["));
    assert(z.contains("||"));
    assert(yy.contains("[["));
    assert(zz.contains("||"));

    Set getZero;
    ItemType getX;
    assert(!getZero.get(0, getX)); // calling get on the empty set should always return false
}

int main()
{
    test();
    cout << "Passed all tests" << endl;
}