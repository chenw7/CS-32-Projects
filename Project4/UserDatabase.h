#ifndef USERDATABASE_INCLUDED
#define USERDATABASE_INCLUDED

#include <string>
#include <fstream>
#include <cassert>
#include <vector>
#include "treemm.h"

class User;

class UserDatabase
{
  public:
    UserDatabase();
    bool load(const std::string& filename);
    User* get_user_from_email(const std::string& email) const;
    ~UserDatabase();

  private:
    TreeMultimap<std::string, User*> allUsers;
    std::vector<std::string> loadedData;
    std::vector<User*> toBeDeleted;
};

#endif // USERDATABASE_INCLUDED
