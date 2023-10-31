#include <iostream>
#include <string>
#include <cassert>
using namespace std;

//FUNCTION 1/10: --- appendToAll ---
int appendToAll(string a[], int n, string value) {
	if(n < 0){ // no negative n
		return -1;
	}

	for (int i = 0; i < n; i++) { // loops through array entries
		//a[i] = a[i] + value;	  // append string of value to each entry
		a[i].append(value);			  // alternative: a[i] += value;
	}

	return n;
}


//FUNCTION 2/10: lookup
int lookup(const string a[], int n, string target) {
	if (n < 0) { // no negative n
		return -1;
	}
	int locMatch = -1; // if not found (or zero elements) return -1 at end
	for (int i = 0; i < n; i++) {
		if (a[i] == target) { // if we find a match at ith-index
			locMatch = i; // assign location of max to ith-index
		}
	}

	return locMatch;

}


//FUNCTION 3/10: positionOfMax
int positionOfMax(const string a[], int n) {
	if (n <= 0) { // no negative n (and returns -1 when n = 0, no interesting elements)
		return -1;
	}

	int locMax = 0;
	for (int i = 0; i < n; i++) {
		if (a[i] > a[locMax]) // anytime we find something greater
			locMax = i; // new location is i-th index
	}

	return locMax; // return the max's index

}


//FUNCTION 4/10: rotateLeft
int rotateLeft(string a[], int n, int pos) {
	if (n < 0) { // no negative n
		return -1;
	}
	if (pos >= n || pos < 0) return -1; // cannot search out of bounds of array size, so return -1
	// also, position index cannot be negative since array index starts at 0

	string temp = a[pos]; // store a[pos] value
	for (int i = pos; i < n - 1; i++) { // use i < n - 1 for condition to prevent a[i+1] overflow
		a[i] = a[i + 1]; // reassign i-th index val with (i+1)-th index val
	}
	a[n - 1] = temp; // replace FINAL index with a[pos] original value (temp)
	return pos;

	// no additional arrays created!
}


//FUNCTION 5/10: countRuns
int countRuns(const string a[], int n) {
	if (n < 0) { // no negative n
		return -1;
	}

	int runs = 0;
	if (n == 0) return runs; // if there are no elements, there are zero consecutive sequence runs by default

	if (n >= 1) // unnecessary if condition, but just for sanity-checking
		runs++; // start at runs = 1 at first element

	for (int i = 1; i < n; i++) { // start at SECOND index comparing to FIRST index, and so on
		if (a[i] != a[i - 1])
			runs++;
	}

	return runs;
}


//FUNCTION 6/10: flip
int flip(string a[], int n) {
	if (n < 0) { // no negative n
		return -1;
	}
	
	string temp = "";
	for (int i = 0; i < n / 2; i++) { // only swap first half with second half
		temp = a[i];			      // (not counting middle index for odd arrays)
		a[i] = a[n - 1 - i];
		a[n - 1 - i] = temp;
	}

	return n;

	// no additional arrays created!
}


//FUNCTION 7/10: differ
int differ(const string a1[], int n1, const string a2[], int n2) {
	if (n1 < 0 || n2 < 0) { // no negative n
		return -1;
	}
	int index = 0;
	int nMin = n1;
	if (n2 < n1)
		nMin = n2; // choose the array with minimum index count and set it to nMin value
	for (int i = 0; i < nMin; i++) { // check only upto the last index of the shorter array
		if (a1[i] == a2[i])
			index++; // if they don't differ, check the very next index on next iteration
		else
			break; // if we find a differ, break out and return index
	}

	return index;
	// the arrays a1 and a2 are same until what index?
}

//FUNCTION 8/10: subsequence
int subsequence(const string a1[], int n1, const string a2[], int n2) {
	if (n1 < 0 || n2 < 0) { // no negative n
		return -1;
	}

	if (n2 == 0) return 0; // if subsequence is empty, there will be an automatic match at 0th index

	int subsequenceStart = 0;
	for (int i = 0; i < n1; i++) {
		if (a1[i] == a2[0]) { // as soon as we find a match, set the beginning index of subsequence
			subsequenceStart = i;
			if (n1 - subsequenceStart >= n2) { // are there enough remaining values in a1 to match all of a2
				for (int j = 0; j < n2; j++) {
					if (a2[j] != a1[j + subsequenceStart])
						return -1; // if non-match at any point of subsequence, return -1
				}
				return subsequenceStart; // if we match all of a2, return the starting index
			}
			else
				return -1; // not enough values for subsequence
		}
	}
	return -1; // fails to find match
}


//FUNCTION 9/10: lookupAny
int lookupAny(const string a1[], int n1, const string a2[], int n2) {
	if (n1 < 0 || n2 < 0) { // no negative n
		return -1;
	}

	int	locMatch = 0;

	for (int i = 0; i < n1; i++) { // check each item in a1 sequentially
		for (int j = 0; j < n2; j++) { // for each one, check every a2 value sequentially
			if (a2[j] == a1[i])		   // for the earliest match
				return locMatch;
		}
		locMatch++; // if it doesn't match at this index of a1, restart search for next index
	}

	return -1; // if we fail to return a match after running through whole loop
}


//FUNCTION 10/10: split
int split(string a[], int n, string splitter) {
	if (n < 0) { // no negative n
		return -1;
	}

	int countLeft = 0;  //  three categories of values: < splitter goes to the left,
	int countEqual = 0; //  = splitter goes in the middle, and

	for (int i = 0; i < n; i++) {
		if (a[i] < splitter)
			countLeft++;
		else if (a[i] == splitter)
			countEqual++;
	} // this for loop tallies how many of each value category there is in the whole array

	string temp = ""; // empty temp for storage

	for (int i = 0; i < countLeft; i++) { // run through the total number of countLeft's
		for (int j = i; j < n; j++) {	  // check each array index at or after the i-th one
			if (a[j] < splitter) {		  // for values < splitter
				temp = a[i];
				a[i] = a[j];
				a[j] = temp;			  // swap over the < splitter value with i-th index
				break;
			}
		}
	}

	for (int i = 0; i < countEqual; i++) { // run through the total number of countEquals's
		for (int j = i+countLeft; j < n; j++) {	  // check the i-th array index AFTER < splitter values
			if (a[j] == splitter) {		  // for values = splitter
				temp = a[i+countLeft];
				a[i+countLeft] = a[j];
				a[j] = temp;			  // swap over the < splitter value with i-th index AFTER < splitter values
				break;
			}
		}
	}

	// NOTE: after these swaps, all the values > splitter are already to the right!
	// no additional arrays created!

	if (n == 0) return 0;
	return countLeft; // this will be the first position of any element >= splitter.
	  // if all elements are < splitter, then n = countLeft, so this still returns n.
}

// ASSERT TEST CASES

int main() // default testing collection, provided by CS 31 Website Project 4 spec
{
	string a1[5] = { "a", "b", "c", "d", "e" };
	assert(appendToAll(a1, -99, "!") == -1); // test -1 return for negative n
	assert(appendToAll(a1, 0, "!") == 0 && a1[0] == "a" && a1[4] == "e"); // appends "!" to emptry string
	assert(appendToAll(a1, 1, "!") == 1 && a1[0] == "a!" && a1[1] == "b"); // only first index
	string b1[5] = { "a", "b", "c", "d", "e" };
	assert(appendToAll(b1, 5, "") == 5 && b1[0] == "a" && b1[4] == "e"); // appends empty string to all indices
	assert(appendToAll(b1, 5, "!") == 5 && b1[0] == "a!" && b1[4] == "e!"); // all indices

	string a2[3] = { "a", "b", "c" };
	assert(lookup(a2, -99, "a") == -1); // negative n returns -1
	assert(lookup(a2, 0, "a") == -1); // search empty string and returns -1
	assert(lookup(a2, 1, "b") == -1); // search zeroth index ("a") fails to find "b"
	assert(lookup(a2, 2, "a") == 0); // finds ("a") at index zero
	assert(lookup(a2, 3, "c") == 2); // finds ("c") at index two
	assert(lookup(a2, 3, "d") == -1); // fails to find "d"

	string a3[6] = { "1", "3", "3", "4", "2", "zzz" };
	assert(positionOfMax(a3, -99) == -1); // negative n returns -1
	assert(positionOfMax(a3, 1) == 0); // for first value, first value is max
	assert(positionOfMax(a3, 2) == 1); // for first 2 values, 1st index is max
	assert(positionOfMax(a3, 3) == 1); // for first 3 values, 1st index is earliest max
	assert(positionOfMax(a3, 5) == 3); // for first 5 values, 3rd index is max
	assert(positionOfMax(a3, 6) == 5); // for whole arrat, last index is max
	assert(positionOfMax(a3, 0) == -1); // for zero element array, return -1

	string a4[4] = { "a", "b", "c", "d" };
	assert(rotateLeft(a4, -99, 0) == -1); // negative n returns -1
	assert(rotateLeft(a4, 4, 4) == -1); // pos value is outside of array indices, return -1
	assert(rotateLeft(a4, 4, 3) == 3 && a4[0] == "a" && a4[3] == "d"); // rotateLeft from last index pos does nothing
	assert(rotateLeft(a4, 4, 1) == 1 && a4[0] == "a" && a4[1] == "c"
		&& a4[2] == "d" && a4[3] == "b"); // rotate indices left, move "b" to end
	assert(rotateLeft(a4, 0, 0) == -1); // size 0 array, no valid pos integers can be called
	string b4[3] = { "a", "b", "c", };
	assert(rotateLeft(b4, 3, 0) == 0 && b4[0] == "b" && b4[1] == "c" && b4[2] == "a"); // rotate whole array over

	string a5[7] = { "1", "1", "2", "3", "2", "5", "5" };
	assert(countRuns(a5, -99) == -1); // negative n returns -1
	assert(countRuns(a5, 0) == 0); // zero consecutive runs in a zero-element array
	assert(countRuns(a5, 1) == 1); // check for one run for one-element array
	assert(countRuns(a5, 2) == 1); // two-in-a-row means one run counted
	assert(countRuns(a5, 3) == 2); // 1, 1, 2 has two runs
	assert(countRuns(a5, 4) == 3); // 1, 1, 2, 3 has three runs
	assert(countRuns(a5, 5) == 4); // make sure duplicate non-consecutive values count as separate runs
	assert(countRuns(a5, 6) == 5); // make sure above condition doesn't impede on count
	assert(countRuns(a5, 7) == 5); // make sure above conditions don't impede consecutive values (not counted)

	string a6[4] = { "1", "2", "3", "4" };
	assert(flip(a6, -99) == -1); // negative n returns -1
	assert(flip(a6, 0) == 0 && a6[0] == "1" && a6[1] == "2" && a6[2] == "3" && a6[3] == "4"); // zero elements doesn't change array
	assert(flip(a6, 4) == 4 && a6[0] == "4" && a6[1] == "3" && a6[2] == "2" && a6[3] == "1"); // flip whole array (check for even)
	string b6[3] = { "1", "2", "3" };
	assert(flip(b6, 3) == 3 && b6[0] == "3" && b6[1] == "2" && b6[2] == "1"); // check flip for odd number of indices

	string a7[3] = { "a", "b", "c" };
	string b7[4] = { "a", "b", "d", "c" };
	string c7[3] = { "0", "1", "2" };
	assert(differ(a7, -99, b7, 4) == -1); // negative n cases return -1
	assert(differ(a7, 3, b7, -99) == -1); // negative n
	assert(differ(a7, -99, b7, -99) == -1); // negative n's for both
	assert(differ(a7, 3, b7, 4) == 2); // index 2 differs
	assert(differ(a7, 2, b7, 2) == 2); // return n1/n2 value for no differences
	assert(differ(a7, 1, b7, 2) == 1); // return minimum value of n1 and n2 for no differences (n1 < n2)
	assert(differ(a7, 2, b7, 1) == 1); // return min value (case n2 < n1)
	assert(differ(a7, 0, b7, 4) == 0); // return 0 when an empty string involved
	assert(differ(b7, 4, c7, 3) == 0); // return 0th index because b7 and c7 differ from the beginning

	string a8[4] = { "w", "x", "y", "z" };
	string b8[2] = { "x", "y" };
	assert(subsequence(a8, -99, b8, 2) == -1); // negative n cases return -1
	assert(subsequence(a8, 4, b8, -99) == -1); // negative n
	assert(subsequence(a8, -99, b8, -99) == -1); // negative n's for both
	assert(subsequence(a8, 4, b8, 2) == 1); // a8[1] is first value of subsequence
	assert(subsequence(b8, 2, a8, 4) == -1); // flipping the correct inputs fails, return -1
	assert(subsequence(a8, 4, b8, 0) == 0); // subsequence of zero starts at index 0
	assert(subsequence(a8, 2, b8, 2) == -1); // has start but isn't complete subsequence
	assert(subsequence(a8, 1, b8, 2) == -1); // n2 cannot be greater than n1

	string a9[5] = { "f", "r", "e", "e", "!" };
	string b9[3] = { "e", "r", "f" };
	string c9[3] = { "a", "b", "c" };
	string d9[1] = { "!" };
	assert(lookupAny(a9, -99, b9, 3) == -1); // negative n1 value returns -1
	assert(lookupAny(a9, 5, b9, -99) == -1); // negative n2 value returns -1
	assert(lookupAny(a9, -99, b9, -99) == -1); // both negative n's, returns -1
	assert(lookupAny(a9, 5, b9, 3) == 0); // first occurrence of either f, r, or e is at index 0
	assert(lookupAny(a9, 5, b9, 2) == 1); // first occurrence of either e or r is at index 1
	assert(lookupAny(a9, 5, b9, 1) == 2); // first occurrence of e is at index 2
	assert(lookupAny(a9, 5, c9, 3) == -1); // no elements match, return -1
	assert(lookupAny(a9, 5, d9, 1) == 4); // a9 match for d9 at 4th index
	assert(lookupAny(b9, 3, a9, 5) == 0); // test that it works while n2 > n1, position 0 match

	string a10[7] = { "e", "b", "g", "a", "d", "c", "f" };
	string b10[7] = { "a", "a", "b", "b", "b", "c", "a"};
	assert(split(a10, -99, "c") == -1); // negative n returns -1
	assert(split(a10, 0, "a") == 0); // no elements, returns n by default since n == 0.
	assert(split(a10, 7, "a") == 0); // case where nothing is before splitter
	assert(split(a10, 7, "d") == 3); // case with elements before and after splitter
	assert(split(a10, 7, "g") == 6); // case with nothing after splitter
	assert(split(b10, 7, "a") == 0); // test if splitter is repeated but has nothing before it
	assert(split(b10, 7, "b") == 3); // test if splitter is repeated and has some things before it
	assert(split(b10, 7, "zzzzz") == 7); // return n, n == 7, since out of bounds

	cerr << "All test cases succeeded" << endl;
}