// Set.cpp

#include "Set.h"

Set::Set() {
	head = nullptr; // initializes every newly created Set to the empty set. 
	tail = nullptr; // note: already done in Set.h to avoid member address misalignment - redoing it here is harmless
	m_size = 0;
}

Set::~Set() {
	Node* p = head;
	while (p != nullptr) { // run through every Node item and delete it, from head to tail
		p = p->next;
		delete head;
		head = p;
	}
	head = nullptr;
	tail = nullptr; // resets both head and tail to nullptr so it doesn't point to arbitrary areas in code. NOTE: head is already nullptr do to while loop, but redone here just in case
}

Set::Set(const Set& other) {
	Set temp;
	for (Node* p = other.head; p != nullptr; p = p->next) { // loop through all nodes in Set& other and insert
		temp.insert(p->m_value); // insert takes care of the values being in order and the m_size value
	}
	this->swap(temp); // temp gets properly deallocated by destructor, going out of scope
}

Set& Set::operator=(const Set& rhs) {
	if (this != &rhs) {
		Set temp(rhs); // copy-and-swap
		this->swap(temp); // exchanges this Set with temp (rhs) Set!
	}
	return *this;
}

bool Set::empty() const {
	if (m_size == 0) return true; // could've been checked with either size or head/tail being nullptr
	return false;
}

int Set::size() const {
	return m_size;
}

bool Set::insert(const ItemType& value) {

	if (m_size == 0) { // create a single-item list
		head = new Node(value);
		tail = head; // both head and tail point to the same single node
		m_size++;
		return true; // exit out, nothing below is called
	}	
	Node* curr;
	for (curr = tail; curr != nullptr; curr = curr->prev) {
		if (curr->m_value == value)
			return false;
	} // check through all elements to see if ItemType& value is already in Set

	// if not already in set: add new Node(value) to end of list
	curr = new Node(value);
	Node* temp = tail;
	tail = curr;
	if (temp != nullptr) // extra check unnecessary
		temp->next = tail;
	curr->prev = temp; // adds new Node to the END of the linked list
	m_size++; // update size
	return true;
}

bool Set::erase(const ItemType& value) {
	if (m_size == 0) return false; // CASE 0: you can't erase anything from an empty set
	Node* curr;
	for (curr = head; curr != nullptr; curr = curr->next) {
		if (curr->m_value == value) {
			// CASE 1: points to the only element in the list
			if (m_size == 1) {
				tail = nullptr;
				head = nullptr;
				delete curr;
				m_size--;
				return true; // returns to empty list state
			}
			// CASE 2: points to FIRST element
			if (curr == head) {
				head = head->next;
				head->prev = nullptr;
				delete curr;
				m_size--;
				return true;
			}
			// CASE 3: points to LAST element
			if (curr == tail) {
				tail = tail->prev;
				tail->next = nullptr;
				delete curr;
				m_size--;
				return true;
			}
			// CASE 4: general case
			curr->next->prev = curr->prev;
			curr->prev->next = curr->next;
			delete curr;
			m_size--;
			return true; // CHECKED!
		}
	}
	return false; // not found
}

bool Set::contains(const ItemType& value) const {
	if (m_size == 0) return false; // CASE 0: empty set doesn't contain it by default
	Node* curr;
	for (curr = head; curr != nullptr; curr = curr->next) {
		if (curr->m_value == value) return true; // CASE 1: match
	}
	return false; // CASE 2: no matches
}

bool Set::get(int pos, ItemType& value) const {

	Node* toBeChecked;
	Node* curr;
	for (toBeChecked = head; toBeChecked != nullptr; toBeChecked = toBeChecked->next) { // for every node A, we will run a comparison between A and every node in the list
		int numGreater = 0; // counter for how many nodes are greater than node A
		for (curr = head; curr != nullptr; curr = curr->next) {
			if (toBeChecked->m_value > curr->m_value)
				numGreater++; // add 1 to integer for evry node value greater than current
		}
		if (numGreater == pos) {
			value = toBeChecked->m_value;
			return true; // only return true if we counted exactly pos # of greater values
		}
	}

	return false; // note: false if empty set because the for loop is automatically skipped due to nullptr condition
}

void Set::swap(Set& other) {
	int temp = other.m_size; // swap m_size values
	other.m_size = this->m_size;
	this->m_size = temp;
	Node* tHead = other.head; // swap head
	other.head = this->head;
	this->head = tHead;
	Node* tTail = other.tail; // swap tail
	other.tail = this->tail;
	this->tail = tTail;
}

// NON-MEMBER IMPLEMENTATIONS

void unite(const Set& s1, const Set& s2, Set& result) {
	if (&s1 != &result && &s2 != &result) { // anti-aliasing check & emptying out result
		while (!result.empty()) {
			ItemType x;
			result.get(0, x);
			result.erase(x); // gets rid of all elements in result

		}
	}

	if (&result != &s1)
	for (int i = 0; i < s1.size(); i++) {
		ItemType x1;
		s1.get(i, x1); // add each individual element in order of value in s1
		result.insert(x1); // insert s1 items
	}

	if (&result != &s2)
	for (int i = 0; i < s2.size(); i++) {
		ItemType x2;
		s2.get(i, x2); // then, add each individual element in order of value in s2
		result.insert(x2); // insert s2 items. NOTE: insert function will handle duplicates automatically
	}
}

void butNot(const Set& s1, const Set& s2, Set& result) { // if s1 but not in s2

	Set newSet; // operate on an empty set

	if (&s1 != &s2) { // if s1 == s2, don't have do anything!
		for (int i = 0; i < s1.size(); i++) {
			ItemType x1;
			s1.get(i, x1);
			newSet.insert(x1); // insert s1's items into the emptySet, same as unite function implementation (could have also used assignment operator or copy constructor)
		}

		for (int i = 0; i < s2.size(); i++) {
			ItemType x2;
			s2.get(i, x2);
			if (newSet.contains(x2)) {
				newSet.erase(x2); // call erase for every element we find in newSet that also is contained in s2.
			}
		}
	}

	result = newSet; // copy newSet into result. newSet is deleted after function goes out of scope
}