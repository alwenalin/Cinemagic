#include "UserDatabase.h"
#include "User.h"
#include "MovieDatabase.h"
#include "Movie.h"
#include "treemm.h" 
#include "Recommender.h"

#include <chrono>
#include <iostream>
#include <string>
using namespace std;

//////////////////////////i/////////////////////////////////////////////////////
//
// You are free to do whatever you want with this file, since you won't
// be turning it in.  Presumably, you will make changes to help you test
// your classes.  For example, you might try to implement only some User
// member functions to start out with, and so replace our main routine with
// one that simply creates a User and verifies that the member functions you
// implemented work correctly.
//
//////////////////////////i/////////////////////////////////////////////////////


  // If your program is having trouble finding these files. replace the
  // string literals with full path names to the files.  Also, for test
  // purposes, you may want to create some small, simple user and movie
  // data files to makde debuggiing easier, so you can replace the string
  // literals with the names of those smaller files.

void findMatches(const Recommender& r, const MovieDatabase& md, const string& user_email, int num_recommendations)
{
	vector<MovieAndRank> recommendations = r.recommend_movies(user_email, num_recommendations);
	if (recommendations.empty())
		cout << "We found no movies to recommend :(.\n";
	else
	{
		for (int i = 0; i < recommendations.size(); i++)
		{
			const MovieAndRank& mr = recommendations[i];
			{
				Movie* m = md.get_movie_from_id(mr.movie_id);
				cout << i + 1 << ". " << m->get_title() << " ("
					<< m->get_release_year() << ")\n Rating: "
					<< m->get_rating() << "\n Compatibility Score: "
					<< mr.compatibility_score << "\n";

				}
		}
	}
}

int main()
{
	// Recommender test cases
	UserDatabase urec;
	urec.load("users2.txt");
	MovieDatabase mrec;
	mrec.load("movies2.txt");
	Recommender rec = Recommender(urec, mrec); 

	for (;;)
	{
		cout << "Enter email (or quit): ";
		string email;
		getline(cin, email);
		if (email == "quit")
			return 0;
		User* u = urec.get_user_from_email(email);
		if (u == nullptr)
			cout << "No user in the database has that email address." << endl;
		else
		{
			cout << "Enter number of rec movies: ";
			int movieRecs;
			cin >> movieRecs;
			cin.ignore(10000, '\n');
			auto start = chrono::steady_clock::now();
			findMatches(rec, mrec, email, movieRecs);
			auto stop = chrono::steady_clock::now();
			cout << "Took " << (chrono::duration_cast<chrono::milliseconds>(stop - start).count()) << "ms" << endl;
		}
	}
	
	//// MovieDatabase test cases
	//MovieDatabase mdb; 
	//mdb.load("movies1.txt");

	//for (;;)
	//{
	//	cout << "Enter director (or quit): ";
	//	string director;
	//	getline(cin, director);
	//	if (director == "quit")
	//		return 0;
	//	vector<Movie*> movies = mdb.get_movies_with_director(director);
	//	if (movies.empty())
	//		cout << "No movie in the database has that director." << endl;
	//	else
	//	{
	//		cout << "Found!" << endl;
	//		for (int i = 0; i < movies.size(); i++)
	//		{
	//			cout << movies.at(i)->get_id() << endl;
	//		}
	//	}
	//}

	//for (;;)
	//{
	//	cout << "Enter actor (or quit): ";
	//	string actor;
	//	getline(cin, actor);
	//	if (actor == "quit")
	//		return 0;
	//	vector<Movie*> movies = mdb.get_movies_with_actor(actor);
	//	if (movies.empty())
	//		cout << "No movie in the database has that actor." << endl;
	//	else
	//	{
	//		cout << "Found!" << endl;
	//		for (int i = 0; i < movies.size(); i++)
	//		{
	//			cout << movies.at(i)->get_id() << endl;
	//		}
	//	}
	//}

	//for (;;)
	//{
	//	cout << "Enter genre (or quit): ";
	//	string genre;
	//	getline(cin, genre);
	//	if (genre == "quit")
	//		return 0;
	//	vector<Movie*> movies = mdb.get_movies_with_genre(genre);
	//	if (movies.empty())
	//		cout << "No movie in the database has that genre." << endl;
	//	else
	//	{
	//		cout << "Found!" << endl;
	//		for (int i = 0; i < movies.size(); i++)
	//		{
	//			cout << movies.at(i)->get_id() << endl;
	//		}
	//	}
	//}

	// UserDatabase test cases
	/*UserDatabase udb;
	udb.load("users1.txt");

	for (;;)
	{
		cout << "Enter user email address (or quit): ";
		string email;
		getline(cin, email);
		if (email == "quit")
			return 0;
		User* u = udb.get_user_from_email(email);
		if (u == nullptr)
			cout << "No user in the database has that email address." << endl;
		else
			cout << "Found " << u->get_full_name() << endl;
	}*/

	//// tree multi map test cases 
	//TreeMultimap<string, int> a;
	//a.insert("hi", 3);
	//a.insert("hi", 4);
	//a.insert("get", 5);
	//a.insert("hi", 6);

	//TreeMultimap<string, int>::Iterator it1 = a.find("hi");
	//while (it1.is_valid())
	//{
	//	cout << it1.get_value() << endl;
	//	it1.advance();
	//}

	//TreeMultimap<int, int>::Iterator it2; // doesn't point to any value
	//if (!it2.is_valid())
	//{
	//	cout << "This will print!\n";
	//}

	//TreeMultimap<std::string, int> tmm;
	//tmm.insert("carey", 5);
	//tmm.insert("carey", 6);
	//tmm.insert("carey", 7);
	//tmm.insert("david", 25);
	//tmm.insert("david", 425);
	//TreeMultimap<std::string, int>::Iterator it = tmm.find("carey");
	//// prints 5, 6, and 7 in some order
	//while (it.is_valid()) {
	//	std::cout << it.get_value() << std::endl;
	//	it.advance();
	//}
	//it = tmm.find("laura"); // key doesn't exist
	//if (!it.is_valid())
	//	std::cout << "laura is not in the multimap!\n"; 
}