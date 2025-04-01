#include "MovieDatabase.h"
#include "Movie.h"
#include <fstream>
#include <string>
#include <vector>
using namespace std;

MovieDatabase::MovieDatabase()
{
    notFirstCall = false;
}

// Loading a database of M movies must run in O(M log M) time.
bool MovieDatabase::load(const string& filename)
{   
    if (notFirstCall == true) 
    {
        return false; // can only be called once per UserDatabase
    }
    notFirstCall = true;
    
    // components of a movie
    string ID;
    string title;
    string release_year;
    vector<string> directors;
    vector<string> actors;
    vector<string> genres;
    float rating = 0;

    int count = 0; // represents each line
    string line;

    ifstream infile(filename);

    while (infile)
    {
        getline(infile, line);

        if (count == 0)
        {
            ID = line;
        }
        else if (count == 1)
        {
            title = line;
        }
        else if (count == 2)
        {
            release_year = line;
        }
        else if (count == 3)
        {
            string director; 
            // separates line into char to get each director
            for (int i = 0; i < line.length(); i++)
            {
                if (line.at(i) == ',')
                {
                    directors.push_back(director);
                    director = "";
                }
                else
                {
                    director = director + line.at(i);
                }
            }
            directors.push_back(director);
        }
        else if (count == 4)
        {
            string actor;
            // separates line into char to get each actor
            for (int i = 0; i < line.length(); i++)
            {
                if (line.at(i) == ',')
                {
                    actors.push_back(actor);
                    actor = "";
                }
                else
                {
                    actor = actor + line.at(i);
                }
            }
            actors.push_back(actor);
        }
        else if (count == 5)
        {
            string genre;
            // separates line into char to get each genre
            for (int i = 0; i < line.length(); i++)
            {
                if (line.at(i) == ',')
                {
                    genres.push_back(genre);
                    genre = "";
                }
                else
                {
                    genre = genre + line.at(i);
                }
            }
            genres.push_back(genre);
        }
        else if (count == 6)
        {
            rating = stof(line); // changes string to float
        }
        else 
        {
            // creates 4 trees: maps IDs -> movies, directors -> movies, actors -> movies, genres -> movies 
            Movie movie(ID, title, release_year, directors, actors, genres, rating);
            tree_ID.insert(ID, movie); // key is id 

            for (int i = 0; i < directors.size(); i++)
            {
                tree_dir.insert(directors[i], movie); // key is director 
            }
            directors.clear();

            for (int i = 0; i < actors.size(); i++)
            {
                tree_act.insert(actors[i], movie); // key is actor
            }
            actors.clear();

            for (int i = 0; i < genres.size(); i++)
            {
                tree_gen.insert(genres[i], movie); // key is genre
            }
            genres.clear();

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

// Assuming there are M movies in the database, searching by a movie ID must run in O(log M) time
Movie* MovieDatabase::get_movie_from_id(const string& id) const
{
    // iterator finds the id of the movie
    TreeMultimap<string, Movie>::Iterator it = tree_ID.find(id);
    if (it.is_valid())
    {
        return &it.get_value();
    }
    return nullptr; // movie doesn't exist
}

// must run in O(log D + md) time where D = unique directors and md = # of directed movies
vector<Movie*> MovieDatabase::get_movies_with_director(const string& director) const
{
    vector<Movie*> movies; 

    // iterator finds the director in the director tree
    TreeMultimap<string, Movie>::Iterator it = tree_dir.find(director);
    while (it.is_valid())
    {
        Movie* moviePtr = &it.get_value();
        movies.push_back(moviePtr);
        it.advance();
    }
    return movies; // will be empty if there are no movies
}

// must run in O(log A + ma) time where A = unique actors and ma = # of movies acted in
vector<Movie*> MovieDatabase::get_movies_with_actor(const string& actor) const
{
    vector<Movie*> movies;

    // iterator finds the actor in the actor tree
    TreeMultimap<string, Movie>::Iterator it = tree_act.find(actor);
    while (it.is_valid())
    {
        Movie* moviePtr = &it.get_value();
        movies.push_back(moviePtr);
        it.advance();
    }
    return movies; // will be empty if there are no movies  
}

// must run in O(log G + mg) time where G = unique genres and mg = # of movies in that genre
vector<Movie*> MovieDatabase::get_movies_with_genre(const string& genre) const
{
    vector<Movie*> movies;

    // iterator finds the director 
    TreeMultimap<string, Movie>::Iterator it = tree_gen.find(genre);
    while (it.is_valid())
    {
        Movie* moviePtr = &it.get_value();
        movies.push_back(moviePtr);
        it.advance();
    }
    return movies; // will be empty if there are no movies
}