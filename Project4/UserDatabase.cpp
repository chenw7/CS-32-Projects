#include "UserDatabase.h"
#include "User.h"
#include <algorithm>
#include <string>
#include <vector>
using namespace std;

UserDatabase::UserDatabase()
{
    // Replace this line with correct code.
}

bool UserDatabase::load(const string& filename)
{
    //Check if file has previously been loaded
    string copy = filename;
    auto it = find(loadedData.begin(), loadedData.end(), copy);
    if(it != loadedData.end())
        return false;
    
    loadedData.push_back(filename);
    ifstream infile(filename);
    
    if (!infile)
    {
        cerr << "Error: Cannot open data.txt!" << endl;
        return false;
    }
    
    std::string user, s, email;
    std::vector<std::string> newHistory;
    int x;
    
    while(getline(infile, user)){
        getline(infile, email);
        infile >> x;
        infile.ignore(10000, '\n');
        newHistory.clear();
        
        for(int i = 0; i < x; i++){
            getline(infile, s);
            newHistory.push_back(s);
        }
        
        User* newUser = new User(user, email, newHistory);
        toBeDeleted.push_back(newUser);
        allUsers.insert(email, newUser);
        getline(infile, s);
    }
    
    infile.close();
    return true;
    //Must run in O(U log U) time, where U is number of distinct users in database
}

User* UserDatabase::get_user_from_email(const string& email) const
{
    TreeMultimap<std::string, User*>::Iterator it = allUsers.find(email);
    if(it.is_valid())
        return it.get_value();
    return nullptr;
    //Must run in O(log U) time, where U is number of distinct users in database
}

UserDatabase::~UserDatabase()
{
    for(auto it = toBeDeleted.begin(); it != toBeDeleted.end(); it++){
        delete (*it);
    }
}
