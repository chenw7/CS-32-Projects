#include "MovieDatabase.h"
#include "Movie.h"
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
using namespace std;

MovieDatabase::MovieDatabase()
{
    // Replace this line with correct code.
}

void MovieDatabase::splitByComma(std::string text, std::vector<std::string>& info)
{
    string temp = "";
    for(int i = 0; i < text.size(); i++){
        if(text.at(i) != ',')
            temp += text.at(i);
        else{
            info.push_back(temp);
            temp = "";
        }
    }
    info.push_back(temp);
}

std::string MovieDatabase::everyToLower(std::string text) const
{
    string temp = "";
    for(int i = 0; i < text.size(); i++){
        if(isalpha(text.at(i))){
            temp += tolower(text.at(i));
        }
        else{
            temp += text.at(i);
        }
    }
    return temp;
}

bool MovieDatabase::load(const string& filename)
{
    //Check if file has previously been loaded
    string copy = filename;
    auto it = find(loadedData.begin(), loadedData.end(), copy);
    if(it != loadedData.end())
        return false;
    
    loadedData.push_back(filename);
    
    ifstream infile(filename);
    
    if(!infile)
    {
        cerr << "Error: Cannot open data.txt!" << endl;
        return false;
    }
    
    std::string id, movie, year, directors, actors, genres;
    float rating;
    
    while(getline(infile, id)){
        getline(infile, movie);
        getline(infile, year);
        getline(infile, directors);
        getline(infile, actors);
        getline(infile, genres);
        infile >> rating;
        infile.ignore(10000, '\n');
        
        std::vector<std::string> directorsV;
        std::vector<std::string> actorsV;
        std::vector<std::string> genresV;
        
        splitByComma(directors, directorsV);
        splitByComma(actors, actorsV);
        splitByComma(genres, genresV);
        
        Movie* cur = new Movie(id, movie, year, directorsV, actorsV, genresV, rating);
        toBeDeleted.push_back(cur);
        
        movieID.insert(everyToLower(id), cur);
        
        for(int i = 0; i < directorsV.size(); i++){
            movieDirector.insert(everyToLower(directorsV[i]), cur);
        }
        
        for(int i = 0; i < actorsV.size(); i++){
            movieActor.insert(everyToLower(actorsV[i]), cur);
        }
        
        for(int i = 0; i < genresV.size(); i++){
            movieGenre.insert(everyToLower(genresV[i]), cur);
        }
        
        getline(infile, id);
    }
    
    infile.close();
    return true;
}

Movie* MovieDatabase::get_movie_from_id(const string& id) const
{
    TreeMultimap<std::string, Movie*>::Iterator it = movieID.find(everyToLower(id));
    if(it.is_valid())
        return it.get_value();
    return nullptr;
}

vector<Movie*> MovieDatabase::get_movies_with_director(const string& director) const
{
    TreeMultimap<std::string, Movie*>::Iterator it = movieDirector.find(everyToLower(director));
    
    vector<Movie*> movies;
    while(it.is_valid()){
        movies.push_back(it.get_value());
        it.advance();
    }
    return movies;
}

vector<Movie*> MovieDatabase::get_movies_with_actor(const string& actor) const
{
    TreeMultimap<std::string, Movie*>::Iterator it = movieActor.find(everyToLower(actor));
    
    vector<Movie*> movies;
    while(it.is_valid()){
        movies.push_back(it.get_value());
        it.advance();
    }
    return movies;
}

vector<Movie*> MovieDatabase::get_movies_with_genre(const string& genre) const
{
    TreeMultimap<std::string, Movie*>::Iterator it = movieGenre.find(everyToLower(genre));
    
    vector<Movie*> movies;
    while(it.is_valid()){
        movies.push_back(it.get_value());
        it.advance();
    }
    return movies;
}

MovieDatabase::~MovieDatabase()
{
    for(auto it = toBeDeleted.begin(); it != toBeDeleted.end(); it++){
        delete (*it);
    }
}
