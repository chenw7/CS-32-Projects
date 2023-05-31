#include "Recommender.h"
#include "UserDatabase.h"
#include "MovieDatabase.h"
#include "User.h"
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <map>
#include <unordered_map>

bool Recommender::compareMovie(const MovieAndRank& lhs, const MovieAndRank& rhs) const
{
    if(lhs.compatibility_score > rhs.compatibility_score)
        return true;
    else if(lhs.compatibility_score < rhs.compatibility_score)
        return false;
    else{
        Movie* lhsM = movie_database.get_movie_from_id(lhs.movie_id);
        Movie* rhsM = movie_database.get_movie_from_id(rhs.movie_id);
        if(lhsM->get_rating() > rhsM->get_rating())
            return true;
        else if(lhsM->get_rating() < rhsM->get_rating())
            return false;
        else if(lhsM->get_title() < rhsM->get_title()) //ASCENDING ORDER SO FLIPPED
            return true;
    }
    return false;
}


Recommender::Recommender(const UserDatabase& user_database, const MovieDatabase& movie_database)
:user_database(user_database), movie_database(movie_database)
{
}

std::vector<MovieAndRank> Recommender::recommend_movies(const std::string& user_email, int movie_count) const
{
    User* user = user_database.get_user_from_email(user_email);
    
    if(user == nullptr || movie_count <= 0){
        return std::vector<MovieAndRank>();
    }
    
    std::unordered_map<std::string, int> scores;
    
    std::vector<std::string> watchHistory = user->get_watch_history();
    
    //For every movie in watch history, do action based on directors, actors, and genres
    for(auto it = watchHistory.begin(); it != watchHistory.end(); it++){
        Movie* currentMovie = movie_database.get_movie_from_id(*it);
            
        //For each director associated, 20 points
        std::vector<std::string> directors = currentMovie->get_directors();
        for(auto p = directors.begin(); p != directors.end(); p++){
            std::vector<Movie*> matchingDirectors = movie_database.get_movies_with_director(*p);
            for(auto q = matchingDirectors.begin(); q != matchingDirectors.end(); q++){
                scores[(*q)->get_id()] += 20;
            }
        }
            
        //For each actor associated, 30 points
        std::vector<std::string> actors = currentMovie->get_actors();
        for(auto r = actors.begin(); r != actors.end(); r++){
            std::vector<Movie*> matchingActors = movie_database.get_movies_with_actor(*r);
            for(auto s = matchingActors.begin(); s != matchingActors.end(); s++){
                scores[(*s)->get_id()] += 30;
            }
        }
        
        //For each genre associated, 1 point
        std::vector<std::string> genres = currentMovie->get_genres();
        for(auto a = genres.begin(); a != genres.end(); a++){
            std::vector<Movie*> matchingGenres = movie_database.get_movies_with_genre(*a);
            for(auto b = matchingGenres.begin(); b != matchingGenres.end(); b++){
                scores[(*b)->get_id()] += 1;
            }
        }
    }
    
    //filter out watched movies
    for(auto it = watchHistory.begin(); it != watchHistory.end(); it++){
        scores.erase(*it);
    }
    
    std::vector<MovieAndRank> possibleMovies; //VECTOR TO STORE SO I CAN SORT
    for(auto it = scores.begin(); it != scores.end(); it++){
        possibleMovies.push_back(MovieAndRank((*it).first, (*it).second));
    }
    
    //Sorting by compatability score, rating, and name
    std::sort(possibleMovies.begin(), possibleMovies.end(), [&](const MovieAndRank& a, const MovieAndRank& b) {return compareMovie(a, b);});
    
    possibleMovies.erase(possibleMovies.begin() + fmin((int) possibleMovies.size(), movie_count), possibleMovies.end());
    
    return possibleMovies;
}
