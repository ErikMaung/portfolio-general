#include "UserDatabase.h"
#include "User.h"

#include <string>
#include <vector>
#include <fstream> // needed to read in users.txt
#include <iostream>
using namespace std;

UserDatabase::UserDatabase()
{
}

bool UserDatabase::load(const string& filename)
{
    ifstream userFile(filename);
    if (!userFile || !unloaded) { // return if either: we can't find the file OR we've already loaded it in
        return false;
    }
    else {
        // std::cerr << "LOADING..." << endl;
        string line;
        int movieCount;
        // int usersLoaded = 0; // removed counter, only for debugging purposes
        while (!userFile.eof()) {
            getline(userFile, line);
            if (line.empty()) {
                continue;
            }
            string name = line;
            getline(userFile, line);
            string email = line;
            userFile >> movieCount;
            vector<string> moviesWatched;
            getline(userFile, line); // eliminate newline separating Users
            for (int i = 0; i < movieCount; i++) {
                getline(userFile, line);
                moviesWatched.push_back(line);
            }
            User user(name, email, moviesWatched);
            bst.insert(user.get_email(), user);
        }        
    }
    unloaded = false; // set this filename to LOADED - we cannot load it again!
        return true;
}

User* UserDatabase::get_user_from_email(const string& email) const
{
    auto it = bst.find(email); // get iterator (automatically converted with auto) to thing in BST with key email
    if (it.is_valid())
        return &(it.get_value()); // return pointer to the associated User
    else
        return nullptr;
}
