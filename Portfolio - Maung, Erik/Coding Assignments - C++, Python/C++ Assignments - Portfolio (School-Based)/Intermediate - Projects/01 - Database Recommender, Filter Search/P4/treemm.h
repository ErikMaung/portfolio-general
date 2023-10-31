#ifndef TREEMULTIMAP_INCLUDED
#define TREEMULTIMAP_INCLUDED

#include <vector>
#include <iostream>

struct Node; // good practice

template <typename KeyType, typename ValueType>
class TreeMultimap
{
private:
    struct Node {
        Node(const KeyType& key, const ValueType& value)
            : m_key(key), left(nullptr), right(nullptr) {
            m_values.push_back(value);
        }

        KeyType m_key; // find nodes with email, movieID, for example
        std::vector<ValueType> m_values;
        Node* left;
        Node* right;
    };
    Node* root;
    void insertHelper(Node*& p, const KeyType& key, const ValueType& value) { // check if reference & are correct
        if (p == nullptr) {
            p = new Node(key, value);
        }
        if (key < p->m_key) {
            insertHelper(p->left, key, value);
        }
        else if (key > p->m_key) {
            insertHelper(p->right, key, value);
        }
        else { // keys are equal!
            p->m_values.push_back(value);
        }
    }
    void deleteNodes(Node* node) { // destructor helper function
        if (node != nullptr) { // DO NOT delete empty node pointers
            deleteNodes(node->left);
            deleteNodes(node->right); // traverse recursively to the bottom of the tree first before deleting
            delete node; // deletes only when at a node without children! prevent leaking
        }
    }
    // NOTE: PRIVATE MUST GO FIRST IN ORDER TO DEFINE NODE.
public:
    class Iterator
    {
    public:
        Iterator()
            : m_validState(false) // 'empty' iterators are invalid
        {
        }

        Iterator(typename std::vector<ValueType>::iterator it,
            typename std::vector<ValueType>::iterator endIt) : m_it(it), m_endIt(endIt) {
            m_validState = (m_it != m_endIt);
        }

        ValueType& get_value() const
        {
            if (!is_valid())
                throw 1; // exception
            return (*m_it);
        }

        bool is_valid() const
        {
            return (m_validState && m_it != m_endIt); // if the iterator was valid AND we don't reach the end of the list we were searching
        }

        void advance()
        {
            if (is_valid()) {
                m_it++; // increment iterator simply
            }
            // else std::cerr << "INVALID ADVANCE CALL" << std::endl; DEBUG
        }

    private:
        bool m_validState;
        typename std::vector<ValueType>::iterator m_it; // iterates through each User in a vector<User> for example
        typename std::vector<ValueType>::iterator m_endIt; // should point at spot just past the end of vector<e.g. User>
    };

    TreeMultimap()
    {
        root = nullptr;
    }

    ~TreeMultimap()
    {
        deleteNodes(root); // recursively remove all nodes starting from terminal nodes working up
    }

    void insert(const KeyType& key, const ValueType& value)
    {
        insertHelper(root, key, value);
    }

    Iterator find(const KeyType& key) const
    {
        Node* curr = root;
        while (curr != nullptr) { // basic binary search
            if (key < curr->m_key) {
                curr = curr->left;
            }
            else if (key > curr->m_key) {
                curr = curr->right;
            }
            else if (key == curr->m_key) {
                return Iterator(curr->m_values.begin(), curr->m_values.end()); // create iterator with start and end
            }
        }
        return Iterator(); // if curr ends up as nullptr, then we didn't find it. return invalid Iterator object
    }
};

#endif // TREEMULTIMAP_INCLUDED