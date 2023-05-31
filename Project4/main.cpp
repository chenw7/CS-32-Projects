#include <iostream>
#include <string>
#include <chrono>

#include "User.h"
#include "UserDatabase.h"
#include "Movie.h"
#include "MovieDatabase.h"
#include "treemm.h"
#include "Recommender.h"
using namespace std;

const string USER_DATAFILE  = "/Users/simba0617/Downloads/Pnetphlix 3/users.txt";
const string TESTUSER_DATAFILE  = "/Users/simba0617/Downloads/Pnetphlix 3/users.txt";
const string MOVIE_DATAFILE = "/Users/simba0617/Downloads/Pnetphlix 3/movies.txt";
const string TESTMOVIE_DATAFILE = "/Users/simba0617/Downloads/Pnetphlix 3/movies.txt";

int testUsers(bool outputLength) {
    const string file = USER_DATAFILE;
    UserDatabase udb;
    if (!udb.load(file)) {
        cout << "Failed to load user data file " << file << endl;
        return 1;
    }
    
    string email;
    User* u;
    while (true) {
        cout << "Enter user email address (or quit): ";
        getline(cin, email);
        if (email == "quit" || email == "q")
            return 0;
        
        u = udb.get_user_from_email(email);
        if (u != nullptr) {
            cout << u->get_full_name() << "  " << u->get_watch_history().size() << endl;
            if (outputLength) for (auto a : u->get_watch_history()) cout << a << " ";
            cout << endl << endl;
        }
    }
    
    return 0;
}

int testMovies(bool outputLength) {
    const string file = MOVIE_DATAFILE;
    MovieDatabase mdb;
    if (!mdb.load(file)) {
        cout << "Failed to load movie data file " << file << endl;
        return 1;
    }
    
    string searchBy, prompt;
    vector<Movie*> m;
    while (true) {
        while (true) {
            cout << "Search by id, director, actor, or genre (or quit): ";
            getline(cin, searchBy);
            if (searchBy == "id" || searchBy == "director" || searchBy == "actor" || searchBy == "genre" || searchBy == "i" ||
                searchBy == "d" || searchBy == "a" || searchBy == "g") break;
            else if (searchBy == "quit" || searchBy == "q") return 0;
        }
        
        while (true) {
            cout << "Enter " << searchBy << " (or quit to reselect search by selection): ";
            getline(cin, prompt);
            if (prompt == "quit" || prompt == "q") break;
            
            m.clear();
            if (searchBy == "id" || searchBy == "i") {
                m.push_back(mdb.get_movie_from_id(prompt));
                if (m[0] == nullptr) m.pop_back();
            }
            else if (searchBy == "director" || searchBy == "d") m = mdb.get_movies_with_director(prompt);
            else if (searchBy == "actor" || searchBy == "a") m = mdb.get_movies_with_actor(prompt);
            else if (searchBy == "genre" || searchBy == "g") m = mdb.get_movies_with_genre(prompt);
            
            for (auto a : m) {
                cout << a->get_title() << "  " << a->get_id() << "  " << a->get_release_year() << "  " << a->get_rating() << endl;
                
                if (outputLength) {
                    for (auto b : a->get_directors()) cout << b << "  ";
                    cout << endl;
                    for (auto b : a->get_actors()) cout << b << "  ";
                    cout << endl;
                    for (auto b : a->get_genres()) cout << b << "  ";
                }
                cout << endl << endl;
            }
        }
    }
    
    return 0;
}

int testRec() {
    auto start = chrono::steady_clock::now();
    UserDatabase udb;
    if (!udb.load(USER_DATAFILE)) {
        cout << "Failed to load user data file " << USER_DATAFILE << endl;
        return 1;
    }
    
    MovieDatabase mdb;
    if (!mdb.load(MOVIE_DATAFILE)) {
        cout << "Failed to load movie data file " << MOVIE_DATAFILE << endl;
        return 1;
    }
    auto stop = chrono::steady_clock::now();
    cout << "Took " << (chrono::duration_cast<chrono::milliseconds>(stop - start).count()) << "ms to load" << endl;
    
    Recommender r(udb, mdb);
    
    string email;
    int number;
    while (true) {
        cout << "Enter an email: ";
        getline(cin, email);
        cout << "How many reccomendations: ";
        cin >> number;
        cin.ignore(10000, '\n');
        start = chrono::steady_clock::now();
        
        vector<MovieAndRank> recommendations = r.recommend_movies(email, number);
        stop = chrono::steady_clock::now();
        cout << "Took " << (chrono::duration_cast<chrono::milliseconds>(stop - start).count()) << "ms to produce recs" << endl;

        for (int i = 0; i < recommendations.size(); i++) {
            const MovieAndRank& mr = recommendations[i];
            Movie* m = mdb.get_movie_from_id(mr.movie_id);
            cout << i + 1 << ". " << m->get_title() << " ("
            << m->get_release_year() << ")\n Rating: "
            << m->get_rating() << "\n Compatibility Score: "
            << mr.compatibility_score << "\n";
        }
    }
    
    return 0;
}

int main() {
    string whatToTest;
    while (true) {
        cout << "Would you like to test user, movie, or rec?" << endl;
        getline(cin, whatToTest);
        if (whatToTest == "user" || whatToTest == "movie" || whatToTest == "u" || whatToTest == "m" || whatToTest == "rec" || whatToTest == "r") break;
    }
    
    string fullData;
    while (true && whatToTest != "rec" && whatToTest != "r") {
        cout << "Would you like short or long output?" << endl;
        getline(cin, fullData);
        if (fullData == "short" || fullData == "long" || fullData == "s" || fullData == "l") break;
    }
    
    if (whatToTest == "rec" || whatToTest == "r") return testRec();
    bool outputLength = fullData == "short" || fullData == "s" ? false : true;
    return whatToTest == "user" || whatToTest == "u" ? testUsers(outputLength) : testMovies(outputLength);
}
