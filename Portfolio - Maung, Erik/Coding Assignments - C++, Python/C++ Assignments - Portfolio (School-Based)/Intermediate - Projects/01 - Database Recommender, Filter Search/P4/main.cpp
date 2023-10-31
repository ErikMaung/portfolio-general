#include "UserDatabase.h"
#include "User.h"
#include "MovieDatabase.h"
#include "Movie.h"
#include "Recommender.h"
#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
using namespace std;


//////////////////////////i/////////////////////////////////////////////////////
//
// You are free to do whatever you want with this file, since you won't
// be turning it in.  Presumably, you will make changes to help you test
// your classes.  For example, you might try to implement only some User
// member functions to start out with, and so replace our main routine with
// one that simply creates a User and verifies that the member functions you
// implemented work correctly.
//
//////////////////////////i/////////////////////////////////////////////////////


  // If your program is having trouble finding these files. replace the
  // string literals with full path names to the files.  Also, for test
  // purposes, you may want to create some small, simple user and movie
  // data files to makde debuggiing easier, so you can replace the string
  // literals with the names of those smaller files.



const string USER_DATAFILE  = "users.txt";
const string MOVIE_DATAFILE = "movies.txt";

int main()
{
	
		TreeMultimap<std::string, int> tmm;
		int j = 3;
		tmm.insert("carey", 5);
		tmm.insert("carey", j);
		tmm.insert("carey", 7);
		tmm.insert("carey", 6);
		tmm.insert("carey", j);
		tmm.insert("david", 25);
		tmm.insert("david", 425);
		TreeMultimap<std::string, int>::Iterator it = tmm.find("carey");
		// prints 5, 6, and 7 in some order
		while (it.is_valid()) {
			std::cout << it.get_value() << std::endl;
			it.advance();
		}
		it = tmm.find("laura");
		if (!it.is_valid())
			std::cout << "laura is not in the multimap!\n";

	std::cerr << "\nExample Complete\n\n";
	UserDatabase udb;
	if (!udb.load(USER_DATAFILE))  // In skeleton, load always return false // GET RID OF 0 && 
	{
		cout << "Failed to load user data file " << USER_DATAFILE << "!" << endl;
		return 1;
	}
	MovieDatabase mdb;
	if (!mdb.load(MOVIE_DATAFILE))
	{
		cout << "Failed to load movie data file " << MOVIE_DATAFILE << "!" << endl;
		return 2;
	}

	for (;;)
	{
		cout << "Enter user email address (or quit, or movie, or D/A/G): ";
		string email;
		getline(cin, email);
		if (email == "D") {
			for (;;) {
				cout << "Enter director name (or quit): ";
				string dir;
				getline(cin, dir);
				if (dir == "quit")
					break;
				vector<Movie*> m = mdb.get_movies_with_director(dir);
				if (m.empty())
					cout << "No movie in the database has that director." << endl;
				else {
					for (int i = 0; i < m.size(); i++)
						cout << "   " << m[i]->get_title() << " (" << m[i]->get_id() << ")" << endl;
					cout << "\n";
				}
			}
		}
		if (email == "A") {
			for (;;) {
				cout << "Enter actor/actress name (or quit): ";
				string dir;
				getline(cin, dir);
				if (dir == "quit")
					break;
				vector<Movie*> m = mdb.get_movies_with_actor(dir);
				if (m.empty())
					cout << "No movie in the database has that actor/actress." << endl;
				else {
					for (int i = 0; i < m.size(); i++)
						cout << "   " << m[i]->get_title() << " (" << m[i]->get_id() << ")" << endl;
					cout << "\n";
				}
			}
		}
		if (email == "G") {
			for (;;) {
				cout << "Enter genre name (or quit): ";
				string dir;
				getline(cin, dir);
				if (dir == "quit")
					break;
				vector<Movie*> m = mdb.get_movies_with_genre(dir);
				if (m.empty())
					cout << "No movie in the database has that genre." << endl;
				else {
					for (int i = 0; i < m.size(); i++)
						cout << "   " << m[i]->get_title() << " (" << m[i]->get_id() << ")" << endl;
					cout << "\n";
				}
			}
		}
		if (email == "quit")
			return 0;
		if (email == "movie") {
			for (;;) {
				cout << "Enter movie ID (or quit): ";
				string movie;
				getline(cin, movie);
				if (movie == "quit")
					break;
				Movie* m = mdb.get_movie_from_id(movie);
				if (m == nullptr)
					cout << "No movie in the database has that ID." << endl;
				else {
					cout << m->get_id() << "\n";
					cout << m->get_title() << "\n";
					cout << m->get_release_year() << "\n";
					for (string md : m->get_directors())
						cout << md << ",";
					cout << "\n";
					for (string ma : m->get_actors())
						cout << ma << ",";
					cout << "\n";
					for (string mg : m->get_genres())
						cout << mg << ",";
					cout << "\n";
					cout << m->get_rating() << "\n";
				}
			}
		}


		User* u = udb.get_user_from_email(email);
		if (u == nullptr)
			cout << "No user in the database has that email address." << endl;
		else {
			cout << u->get_full_name() << "\n";
			cout << u->get_email() << "\n";
			for (string wh : u->get_watch_history()) {
				cout << wh << ",";
			}
			cout << "\n";
			string name;
			for (int i = 0; i < u->get_full_name().size(); i++) {
				if (u->get_full_name()[i] == ' ') break;
				name += u->get_full_name()[i];
			}
			for (;;) {
				cout << "Would you like movie recs, " << name << "? (Y/N): ";
				string choice;
				getline(cin, choice);
				if (choice == "Y" || choice == "y") {
					cout << "How many movies would you like? ";
					int count;
					cin >> count;
					cin.ignore(10000, '\n');
					cout << "Your Watch History: \n";
					for (int i = 0; i < u->get_watch_history().size(); i++)
						cout << mdb.get_movie_from_id((u->get_watch_history()[i]))->get_title() << " (" << mdb.get_movie_from_id((u->get_watch_history()[i]))->get_id() << ")" << "\n";
					cout << "You've watched " << u->get_watch_history().size() << " movies!..." << endl;
					Recommender rec(udb, mdb);
					vector<MovieAndRank> recList = rec.recommend_movies(email, count);
					cerr << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"; // clearing the screen in a sophisticated way
					cout << "\nHere are recommendations for " << name << ":\n\n";
					for (int i = 0; i < count; i++) {
						if (i >= recList.size()) {
							cout << "I couldn't get enough movies, but here are " << recList.size() << " above!\n" << endl;
							break;
						}
						cout << i + 1 << ". " <<
							mdb.get_movie_from_id(recList[i].movie_id)->get_title() << "\n";
						cout << "   Rating: " << mdb.get_movie_from_id(recList[i].movie_id)->get_rating() << "\n"
							<< "   Compatibility Score: " << recList[i].compatibility_score << "\n\n";
					}
				}
				else {
					cout << "Exiting recommender...\n";
					break;
				}
			}
		}
	}
	
}