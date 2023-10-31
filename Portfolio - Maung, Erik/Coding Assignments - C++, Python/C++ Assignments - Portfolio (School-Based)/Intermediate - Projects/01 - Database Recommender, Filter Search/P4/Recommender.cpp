#include "User.h"
#include "Movie.h"
#include "Recommender.h"
#include "UserDatabase.h"
#include "MovieDatabase.h"

#include <string>
#include <vector>
#include <utility>
#include <unordered_set>
#include <cctype>
#include <unordered_map>
#include <algorithm>
using namespace std;

Recommender::Recommender(const UserDatabase& user_database,
                         const MovieDatabase& movie_database)
: udb(user_database), mdb(movie_database) {}

vector<MovieAndRank> Recommender::recommend_movies(const string& user_email, int movie_count) const
{
    ////////////////////////// MovieAndRank INITIAL CASES ////////////////////////////////////////////////////////////////////////////////////

    // if movie_count passed in is negative, treat it as zero
    if (movie_count <= 0) movie_count = 0;

    vector<MovieAndRank> recVec;

    // if user searches for ZERO movies, return immediately
    if (movie_count == 0) return recVec;

    // make vectors that will contain all of the directors, actors, and genres found in the movies the user has watched:
    User* user = udb.get_user_from_email(user_email);
    vector<string> movieIDs = user->get_watch_history();
    vector<string> directorList, actorList, genreList;

    // for each movie, add all directors, actors, and genres
    for (int i = 0; i < movieIDs.size(); i++) {
        Movie* m = mdb.get_movie_from_id(movieIDs[i]);
        const vector<string>& mDirectors = m->get_directors();
        directorList.insert(directorList.end(), mDirectors.begin(), mDirectors.end());
        const vector<string>& mActors = m->get_actors();
        actorList.insert(actorList.end(), mActors.begin(), mActors.end());
        const vector<string>& mGenres = m->get_genres();
        genreList.insert(genreList.end(), mGenres.begin(), mGenres.end());
    }

    // get a vector of pointers to movies with a match with at least some director, actor, or genre of each List.
    unordered_set<string> relevantMovieIDs;
    for (const string& director : directorList) {
        vector<Movie*> relevant = mdb.get_movies_with_director(director);
        for (Movie* m : relevant) {
            relevantMovieIDs.insert(m->get_id());
        }
    }
    for (const string& actor : actorList) {
        vector<Movie*> relevant = mdb.get_movies_with_actor(actor);
        for (Movie* m : relevant) {
            relevantMovieIDs.insert(m->get_id());
        }
    }
    for (const string& genre : genreList) {
        vector<Movie*> relevant = mdb.get_movies_with_genre(genre);
        for (Movie* m : relevant) {
            relevantMovieIDs.insert(m->get_id());
        }
    }
    // cerr << "\n" << relevantMovieIDs.size() << " movies found.\n";

    
    //////////// CREATING MovieAndRank OBJECTS ///////////////////////////////////////////////////////////////////////////////////////////////

    // store pairs of pointers to Movies and their scores as a MAP
    unordered_map<Movie*, int> MovieAndRank_unsorted;

    for (string movieID0 : relevantMovieIDs) { // for each movie that we found some match
        int scoreTotal = 0; // we will start tallying points
        Movie* movie = mdb.get_movie_from_id(movieID0);

        for (string director : movie->get_directors()) { // for every director match we find
            int numOccurrences = std::count(directorList.begin(), directorList.end(), director);
            scoreTotal += 20 * numOccurrences; // we add 20 points each time they appear
        }

        for (string actor : movie->get_actors()) { // for every actor match we find
            int numOccurrences = std::count(actorList.begin(), actorList.end(), actor);
            scoreTotal += 30 * numOccurrences; // we add 30 points each time
        }

        for (string genre : movie->get_genres()) { // for every genre match we find
            int numOccurrences = std::count(genreList.begin(), genreList.end(), genre);
            scoreTotal += 1 * numOccurrences; // we add 1 points each time
        }


        string movieID = movie->get_id();
        // ONLY ADD as a recommended movie IF:
        // they haven't already seen it AND the score is above 0:
        if (scoreTotal > 0 && std::find(movieIDs.begin(), movieIDs.end(), movieID) == movieIDs.end()) { 
                // make sure movie is not already in MovieAndRank_unsorted
            MovieAndRank_unsorted.insert(make_pair(movie, scoreTotal));
            // all pairs consist of UNIQUE movie id pairs that have not already been watched
            // AND only consist of "relevant" movies, meaning a score rank above 0.
        }
    }


    // CONVERT TO MovieAndRank-appropriate type //////////////////////////////////////////////////////////////////////////////////////////////

    vector<pair<Movie*, int>>
        MAR_unsortedVector(MovieAndRank_unsorted.begin(), MovieAndRank_unsorted.end());

    vector<pair<string, int>> MAR_sortedPairs; // will be sorted later! named sorted for now
    for (int i = 0; i < MAR_unsortedVector.size(); i++) {
        string movieID = MAR_unsortedVector[i].first->get_id();
        int score = MAR_unsortedVector[i].second;
        MAR_sortedPairs.push_back(make_pair(movieID, score)); // convert from Movie* and scores to pairs of IDs and Scores!
    }

    std::sort(MAR_sortedPairs.begin(), MAR_sortedPairs.end()); // pairs are now sorted

    vector<pair<string, int>> MAR_sortedUniquePairs;

    for (int i = 0; i < MAR_sortedPairs.size(); i++) {
        pair<string, int> pairCurr = MAR_sortedPairs[i];
        if (i == 0 || MAR_sortedPairs[i] != MAR_sortedPairs[i - 1])
            // design note: i == 0 is checked first; if it is true, then we CANNOT check for [i - 1] entry. but this entry does not 
            // get checked because it is only checked in the other half of the OR || in the condition.
            // if i != 0, i > 0 by ++ increment style AND [i - 1] therefore exists. either condition should push in the i-th entry:
            MAR_sortedUniquePairs.push_back(MAR_sortedPairs[i]);
    }

    // SORT - Score, Rating, Title (case-insensitive) ////////////////////////////////////////////////////////////////////////////////////////

    std::sort(begin(MAR_sortedUniquePairs), end(MAR_sortedUniquePairs),
        [&](const pair<string, int>& val1, const pair<string, int>& val2) {
            if (val1.second != val2.second) {
                return val1.second > val2.second; // first pass - sort by descending SCORE
            }
            else {
                float rating1 = mdb.get_movie_from_id(val1.first)->get_rating();
                float rating2 = mdb.get_movie_from_id(val2.first)->get_rating();
                if (rating1 != rating2) {
                    return rating1 > rating2; // second pass - sort by descending RATING
                }
                else {
                    string title1 = mdb.get_movie_from_id(val1.first)->get_title();
                    string title2 = mdb.get_movie_from_id(val2.first)->get_title();
                    for (int i = 0; i < title1.size(); i++) {
                        title1[i] = tolower(title1[i]);
                    }
                    for (int i = 0; i < title2.size(); i++) {
                        title2[i] = tolower(title2[i]);
                    }
                    return title1 < title2; // third pass - sort alphabetically, case INSENSITIVE
                }
            }
        });
    // using Lambda function -> creates a function and immediately implements it
    // passed in as comparison boolean function for sorting purposes

    // lastly, we store every MovieID-Rank pair we sorted into the MovieAndRank vector, but only the first movie_count items as user requested:
    for (int i = 0; i < movie_count; i++) {
        if (i >= MAR_sortedUniquePairs.size()) break; // UNLESS there are LESS movies than movie_count requested, in which case just store everything into recVec
        recVec.push_back(MovieAndRank(MAR_sortedUniquePairs[i].first, MAR_sortedUniquePairs[i].second));
    }

    return recVec;
}
