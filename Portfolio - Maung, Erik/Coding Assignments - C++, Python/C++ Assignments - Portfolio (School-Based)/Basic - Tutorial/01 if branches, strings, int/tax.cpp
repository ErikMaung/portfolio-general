#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

int main()
{
    string name;
    double income;
    string occupation;
    int children;


    cout << "Name: ";
    getline(cin, name);
    if (name == "") {
        cout << "---" << endl;
        cout << "You must enter a name" << endl;
        return -1; // empty name terminates code
    }

    cout << "Taxable income: ";
    cin >> income;
    if (income < 0) {
        cout << "---" << endl;
        cout << "The taxable income must not be negative" << endl;
        return -1; // income below $0 terminates code
    }

    cin.ignore(10000, '\n'); // fix income input followed by occupation input

    cout << "Occupation: ";
    getline(cin, occupation);
    if (occupation == "") {
        cout << "---" << endl;
        cout << "You must enter an occupation" << endl;
        return -1; // empty occupation terminates code
    }

    cout << "Number of children: ";
    cin >> children;
    if (children < 0) {
        cout << "---" << endl;
        cout << "The number of children must not be negative" << endl;
        return -1; // negative integer for children terminates code
    }

    double tax;
    if (income <= 55000) { // first bracket  [ $0 - $55,000 ]
        tax = income * 0.04; // 4% rate
    }

    if (income > 55000 && income <= 125000) { // next bracket [ $55,000.01 - $125,000 ]
        if (occupation == "nurse" || occupation == "teacher") {
            tax = (55000 * 0.04) + ((income - 55000) * 0.05);
            // 5% bracket rate exception for "nurse" and "teacher"
        }
        else {
            tax = (55000 * 0.04) + ((income - 55000) * 0.07); // 4% rate until $55,000, then 7%
        }
    }

    if (income < 125000) { // cut only applies to taxable income UNDER $125,000
        tax -= children * 200;  // lower tax by $200 for each child
    }

    if (income > 125000) { // last bracket [ exceeds $125,000 ]
        if (occupation == "nurse" || occupation == "teacher") {
            tax = (55000 * 0.04) + (70000 * 0.05) + ((income - 125000) * 0.093);
            // 5% bracket rate exception for "nurse" and "teacher"
        }
        else {
            tax = (55000 * 0.04) + (70000 * 0.07) + ((income - 125000) * 0.093);
        }
        // 4% rate until $55,000, 7% rate until $125,000, 9.3% rate beyond
    }

    if (tax < 0) { // default tax to $0 if calulation produces a negative tax
        tax = 0;
    }

    cout << "---" << endl;
    cout << name << " would pay $" << fixed << setprecision(2) << tax << endl; // EDIT STARTING HERE



    return 0;
}

