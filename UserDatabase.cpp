#include "UserDatabase.h"
#include "User.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

UserDatabase::UserDatabase() 
{
    notFirstCall = false;
}

// Loading a user database of U users must run in O(U log U) time
bool UserDatabase::load(const string& filename)
{   
    if (notFirstCall == true) // this function can only be called once per UserDatabase
    {
        return false;
    }
    notFirstCall = true;
    
    // components of a user
    string name;
    string email;
    int size = 0;
    vector<string> watch_history;

    int count = 0;
    string line;

    ifstream infile(filename);
    while (infile)
    {
        getline(infile, line);
        
        if (count == 0)
        {
            name = line;
        }
        else if (count == 1)
        {
            email = line;
        }
        else if (count == 2)
        {
            size = stoi(line); // converts string to int
        }
        else if (size > 0)
        {
            watch_history.push_back(line);
            size--;
        }
        else 
        {
            User newUser(name, email, watch_history);
            m_tree.insert(email, newUser); // creates tree of users, ordered by email
            watch_history.clear();
            count = 0;
            continue; // skips iterating count
        }
        count++;
    }

    // if we finished reading the file
    if (infile.eof())
    {
        return true;
    }

    return false; // didn't read it all
}

// Finding a user profile from an email address must run in O(log U) time
User* UserDatabase::get_user_from_email(const string& email) const
{
    // iterator finds the email of the user 
    TreeMultimap<string, User>::Iterator it = m_tree.find(email);
    if (it.is_valid())
    {
        return &it.get_value();
    }
    return nullptr; // user doesn't exist
}