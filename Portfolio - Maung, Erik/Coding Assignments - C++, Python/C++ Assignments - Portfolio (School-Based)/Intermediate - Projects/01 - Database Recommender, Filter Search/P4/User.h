#ifndef USER_INCLUDED
#define USER_INCLUDED

#include <string>
#include <vector>

class User
{
  public:
    User(const std::string& full_name, const std::string& email,
         const std::vector<std::string>& watch_history);
    std::string get_full_name() const;
    std::string get_email() const; // serves as key - id
    std::vector<std::string> get_watch_history() const;

  private:
      // store name, email, and vector of movie IDs:
      std::string m_full_name;
      std::string m_email;
      std::vector<std::string> m_watch_history;
};

#endif // USER_INCLUDED
