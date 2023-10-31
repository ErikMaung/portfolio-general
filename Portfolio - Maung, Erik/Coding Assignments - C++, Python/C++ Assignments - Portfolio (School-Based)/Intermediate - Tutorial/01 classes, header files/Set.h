// Set.h

#ifndef SET_INCL
#define SET_INCL

#include <string>

using ItemType = std::string;

class Set
{
public:
    Set(); // upon creation, have head and tail Node pointers be nullptr
    ~Set(); // DESTRUCTOR
    Set(const Set& other); // COPY CONSTRUCTOR
    Set& operator=(const Set& rhs); // ASSIGNMENT OPERATOR
    bool empty() const;
    int size() const;
    bool insert(const ItemType& value);
    bool erase(const ItemType& value);
    bool contains(const ItemType& value) const;
    bool get(int pos, ItemType& value) const;
    void swap(Set& other);
private:
    struct Node{ // private declaration of struct Node inside Set class
        ItemType m_value;
        Node* prev;
        Node* next;
        Node(ItemType value) : m_value(value), prev(nullptr), next(nullptr) {};
    // initializer list for a new node
    };
    Node* head = nullptr;
    Node* tail = nullptr;
    int m_size = 0;
};

void unite(const Set& s1, const Set& s2, Set& result); // add {s1} U {s2} into result
void butNot(const Set& s1, const Set& s2, Set& result); // add {s1} \ {s2} into result

#endif // SET_INCL