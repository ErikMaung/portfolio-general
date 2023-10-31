#include "MovieDatabase.h"
#include "Movie.h"

#include <string>
#include <vector>
#include <cctype>
#include <sstream>
#include <fstream> // needed to read in movies.txt
#include <iostream>
using namespace std;

MovieDatabase::MovieDatabase()
{
}

bool MovieDatabase::load(const string& filename)
{
    ifstream movieFile(filename);
    if (!movieFile || !unloaded) {
        return false;
    }
    else {
        // std::cerr << "LOADING MOVIES..." << endl;
        string line;
        float rating;
        // int moviesLoaded = 0; // removed, for debugging purposes
        while (getline(movieFile, line)) {
            if (line.empty()) {
                continue;
            } // skip empty lines

            string id = line; // first line after empty line is ID, then name, year, etc...
            getline(movieFile, line);
            string name = line;
            getline(movieFile, line);
            string year = line;

            getline(movieFile, line);
            stringstream ss(line);
            string token;
            vector<string> directors;
            while (getline(ss, token, ',')) { // use getline with ',' separator and token value to insert into directors, actors, genre...
                directors.push_back(token);
            }
            
            getline(movieFile, line);
            stringstream ss2(line);
            vector<string> actors;
            while (getline(ss2, token, ',')) {
                actors.push_back(token);
            }

            getline(movieFile, line);
            stringstream ss3(line);
            vector<string> genres;
            while (getline(ss3, token, ',')) {
                genres.push_back(token);
            }
            movieFile >> rating;

            Movie movie(id, name, year, directors, actors, genres, rating); // parse Movie object with matching case, but put into tree in case-insensitive form:
            idTree.insert(toLowercase(movie.get_id()), movie);
            for (int i = 0; i < directors.size(); i++) {
                directorTree.insert(toLowercase(movie.get_directors()[i]), movie);
            }
            for (int i = 0; i < actors.size(); i++) {
                actorTree.insert(toLowercase(movie.get_actors()[i]), movie);
            }
            for (int i = 0; i < genres.size(); i++) {
                genreTree.insert(toLowercase(movie.get_genres()[i]), movie);
            }
            // moviesLoaded++;
        }
        // std::cerr << moviesLoaded << " loaded!" << std::endl;  // DEBUGGING PURPOSES ONLY
    }
    unloaded = false;
    return true;
}

Movie* MovieDatabase::get_movie_from_id(const string& id) const
{
    auto it = idTree.find(toLowercase(id)); // case-insensitive search
    if (it.is_valid())
        return &(it.get_value());
    else
        return nullptr;
}

vector<Movie*> MovieDatabase::get_movies_with_director(const string& director) const
{
    vector<Movie*> pMovies;
    auto it = directorTree.find(toLowercase(director)); // case-insensitive search
    while (it.is_valid()) {
        pMovies.push_back(&it.get_value());
        it.advance();
    }             
                  
    return pMovies;
}                 
                  
vector<Movie*> MovieDatabase::get_movies_with_actor(const string& actor) const
{                 
    vector<Movie*> pMovies;
    TreeMultimap<string, Movie>::Iterator it;
    it = actorTree.find(toLowercase(actor)); // case-insensitive search
    while (it.is_valid()) {
        pMovies.push_back(&it.get_value());
        it.advance();
    }

    return pMovies;
}

std::vector<Movie*> MovieDatabase::get_movies_with_genre(const std::string& genre) const
{
    vector<Movie*> pMovies;
    TreeMultimap<string, Movie>::Iterator it;
    it = genreTree.find(toLowercase(genre)); // case-insensitive search
    while (it.is_valid()) {
        pMovies.push_back(&it.get_value());
        it.advance();
    }

    return pMovies;
}


std::string MovieDatabase::toLowercase(std::string str) const
{
    for (int i = 0; i < str.size(); i++) {
        str[i] = tolower(str[i]);
        // reduces everything in string to lowercase
    }
    return str;
}
