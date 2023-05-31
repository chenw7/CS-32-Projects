#include "Movie.h"

#include <string>
#include <vector>
using namespace std;

Movie::Movie(const string& id, const string& title, const string& release_year,
             const vector<string>& directors, const vector<string>& actors,
             const vector<string>& genres, float rating)
{
    m_id = id;
    m_title = title;
    m_year = release_year;
    m_rating = rating;
    
    for(auto p = directors.begin(); p != directors.end(); p++)
        m_directors.push_back(*p);
    for(auto p = actors.begin(); p != actors.end(); p++)
        m_actors.push_back(*p);
    for(auto p = genres.begin(); p != genres.end(); p++)
        m_genres.push_back(*p);
}
