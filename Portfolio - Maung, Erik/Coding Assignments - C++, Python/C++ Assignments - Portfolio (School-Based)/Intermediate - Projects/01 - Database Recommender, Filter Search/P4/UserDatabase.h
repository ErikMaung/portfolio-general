#ifndef USERDATABASE_INCLUDED
#define USERDATABASE_INCLUDED

#include "treemm.h"

#include <string>
#include <vector>

class User;

class UserDatabase
{
  public:
    UserDatabase();
    bool load(const std::string& filename);
    User* get_user_from_email(const std::string& email) const;

  private:
      TreeMultimap<std::string, User> bst;
      bool unloaded = true; // true means load() hasn't been called with full load success
      // false means it was called
};

#endif // USERDATABASE_INCLUDED
