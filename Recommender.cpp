#include "Recommender.h"
#include "treemm.h"

#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
using namespace std;

Recommender::Recommender(const UserDatabase& user_database,
    const MovieDatabase& movie_database)
    : userData(user_database), movieData(movie_database) {}

vector<MovieAndRank> Recommender::recommend_movies(const string& user_email, int movie_count) const
{
    vector<string> watchedMovies = userData.get_user_from_email(user_email)->get_watch_history();

    // containers for each factor of a movie where int is the # of times it appears in watchedMovies
    unordered_map<string, int> mapDirectors;
    unordered_map<string, int> mapActors;
    unordered_map<string, int> mapGenre;

    // adds into maps for directors, actors, and genres from user's watchedMovies
    for (int a = 0; a < watchedMovies.size(); a++) 
    {
        Movie* movie = movieData.get_movie_from_id(watchedMovies[a]); 

        // If string is already there, it only increments the int (# of times). if not, it adds the string into the map
        vector<string> directors = movie->get_directors(); // directors
        for (int b = 0; b < directors.size(); b++)
        {
            mapDirectors[directors.at(b)]++;
        }

        vector<string> actors = movie->get_actors(); // actors
        for (int c = 0; c < actors.size(); c++)
        {
            mapActors[actors.at(c)]++;
        }

        vector<string> genres = movie->get_genres(); // genres
        for (int d = 0; d < genres.size(); d++)
        {
            mapGenre[genres.at(d)]++;
        }
    }

    // put all values from the 3 maps into one map for all movies (all have > 0 compatibility score)
    unordered_map<string, int> movieScore;
    for (unordered_map<string, int>::iterator it = mapDirectors.begin(); it != mapDirectors.end(); it++) 
    { 
        vector<Movie*> movieDir = movieData.get_movies_with_director(it->first); 
        for (int k = 0; k < movieDir.size(); k++) // for each movie that has the director
        { 
            movieScore[movieDir[k]->get_id()] += (it->second * 20); // +20 in score if a movie has the unique director
        }
    } 

    for (unordered_map<string, int>::iterator it = mapActors.begin(); it != mapActors.end(); it++) 
    { 
        vector<Movie*> movieAct = movieData.get_movies_with_actor(it->first);
        for (int k = 0; k < movieAct.size(); k++) // for each movie that has the actor
        { 
            movieScore[movieAct[k]->get_id()] += (it->second * 30); // +30 in score if a movie has the unique actor
        }
    }

    for (unordered_map<string, int>::iterator it = mapGenre.begin(); it != mapGenre.end(); it++) 
    { 
        vector<Movie*> movieGen = movieData.get_movies_with_genre(it->first);
        for (int k = 0; k < movieGen.size(); k++) // for each movie that has the genre
        { 
            movieScore[movieGen[k]->get_id()] += (it->second * 1); // +1 in score if a movie has the unique genre
        }
    }

    // copy map movieScore into a vector of pairs to be able to sort based on score
    vector<pair<string, int>> containMovies(movieScore.size());
    copy(movieScore.begin(), movieScore.end(), containMovies.begin());
 
    // sorts movies by score, rating, then title 
    sort(containMovies.begin(), containMovies.end(), [this](pair<string, int>& a, pair<string, int>& b) 
        { 
            // lambda function! if true, a comes before b in ordering and vice versa
            Movie* firstMovie = movieData.get_movie_from_id(a.first);
            Movie* secMovie = movieData.get_movie_from_id(b.first);

            if (a.second < b.second)
            {
                return false; 
            }
            else if (a.second > b.second)
            {
                return true; // true if score is greater
            }
            else if (a.second == b.second) // scores are equal
            {
                if (firstMovie->get_rating() < secMovie->get_rating()) 
                {
                    return false; 
                }
                else if (firstMovie->get_rating() > secMovie->get_rating())
                {
                    return true; // true if rating is greater
                }
                else if (firstMovie->get_rating() == secMovie->get_rating()) // ratings are equal
                {
                    if (firstMovie->get_title() < secMovie->get_title()) 
                    {
                        return true; // true if title is alphabetically lower (a->z)
                    }
                }
            }
            return false; // lowest of everything 
        });

    vector<MovieAndRank> movieRanks;

    if (movie_count < 0) // if user inputted neg #, return empty vector
    {
        return movieRanks;
    }
    else if (movie_count > containMovies.size())
    {
        movie_count = containMovies.size(); // only returns movies with > 0 score
    }

    // inputs all movies into movieRanks vector
    for (int i = 0; i < containMovies.size(); i++)
    {
        if (movie_count == 0)
        {
            break;
        }
        
        // if user hasn't already watched the movie (would reach == watchedMovies.end() if it was never found)
        if (find(watchedMovies.begin(), watchedMovies.end(), containMovies[i].first) == watchedMovies.end()) 
        { 
            movieRanks.push_back(MovieAndRank(containMovies[i].first, containMovies[i].second));
            movie_count--;
        }
    }

    return movieRanks;
}