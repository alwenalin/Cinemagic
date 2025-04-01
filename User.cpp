#include "User.h"

#include <string>
#include <vector>

User::User(const std::string& full_name, const std::string& email,
    const std::vector<std::string>& watch_history)
{
    m_name = full_name;
    m_email = email;
    m_watchHistory = watch_history;
}

std::string User::get_full_name() const
{
    return m_name;
}

std::string User::get_email() const
{
    return m_email;
}

std::vector<std::string> User::get_watch_history() const
{
    return m_watchHistory;
}