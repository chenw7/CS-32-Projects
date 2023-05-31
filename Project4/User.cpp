#include "User.h"

#include <string>
#include <vector>
using namespace std;

User::User(const string& full_name, const string& email,
           const vector<string>& watch_history)
{
    m_name = full_name;
    m_email = email;
    
    for(auto p = watch_history.begin(); p != watch_history.end(); p++)
        m_watched.push_back(*p);
}
