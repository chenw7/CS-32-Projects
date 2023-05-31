#ifndef RECOMMENDER_INCLUDED
#define RECOMMENDER_INCLUDED

#include "Recommender.h"
#include "UserDatabase.h"
#include "MovieDatabase.h"
#include <queue>
#include <string>
#include <vector>
#include <map>

struct MovieAndRank
{
    MovieAndRank(const std::string& id, int score)
     : movie_id(id), compatibility_score(score)
    {}

    std::string movie_id;
    int compatibility_score;
};

class Recommender
{
  public:
    Recommender(const UserDatabase& user_database,
                const MovieDatabase& movie_database);
    std::vector<MovieAndRank> recommend_movies(const std::string& user_email,
                                               int movie_count) const;

  private:
    const UserDatabase& user_database;
    const MovieDatabase& movie_database;
    
    bool compareMovie(const MovieAndRank& lhs, const MovieAndRank& rhs) const;
};

#endif // RECOMMENDER_INCLUDED
