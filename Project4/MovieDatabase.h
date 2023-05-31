#ifndef MOVIEDATABASE_INCLUDED
#define MOVIEDATABASE_INCLUDED

#include <string>
#include <vector>
#include <cassert>
#include <ctype.h>
#include "Movie.h"
#include "treemm.h"

class Movie;

class MovieDatabase
{
  public:
    MovieDatabase();
    ~MovieDatabase();
    bool load(const std::string& filename);
    Movie* get_movie_from_id(const std::string& id) const;
    std::vector<Movie*> get_movies_with_director(const std::string& director) const;
    std::vector<Movie*> get_movies_with_actor(const std::string& actor) const;
    std::vector<Movie*> get_movies_with_genre(const std::string& genre) const;

  private:
    void splitByComma(std::string text, std::vector<std::string>& info);
    std::string everyToLower(std::string text) const;
    
    TreeMultimap<std::string, Movie*> movieID;
    TreeMultimap<std::string, Movie*> movieDirector;
    TreeMultimap<std::string, Movie*> movieActor;
    TreeMultimap<std::string, Movie*> movieGenre;
    
    std::vector<std::string> loadedData;
    std::vector<Movie*> toBeDeleted;
};

#endif // MOVIEDATABASE_INCLUDED
