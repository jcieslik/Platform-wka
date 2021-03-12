#include <iostream>
#include <fstream>
#include <algorithm>
#include "Leaderboard.h"

using namespace std;

void Leaderboard::load_leaderboards()
{
	int i = 0;
	pair<int, string> current_entry;
	ifstream file;
	file.open(leaderboards_filename);
	if (file.is_open())
	{
		while (!file.eof() && i < max_size)
		{
			file >> current_entry.first;
			file >> current_entry.second;
			entries.push_back(current_entry);
			i++;
		}
		file.close();
	}
}

void Leaderboard::save_leaderboards()
{
	ofstream file;
	file.open(leaderboards_filename);

	vector<pair<int, string>>::const_iterator iter;
	for(iter = entries.begin(); iter != entries.end(); ++iter)
	{
		file << (*iter).first << " " << (*iter).second;
		if (next(iter, 1) != entries.end()) 
			file << endl;
	}
}

void Leaderboard::add_to_leaderboard(int value, string player_name)
{
	pair<int, string> new_best = make_pair(value, player_name);
	if (entries.size() < max_size)  // jezeli jest miejsce w wektorze, dodaj  
		entries.push_back(new_best);
	else   // jezeli nie, usun najgorszy wynik
	{
		entries.pop_back();
		entries.push_back(new_best);
	}
	sort(entries.begin(), entries.end(), [](const std::pair<int, string>& a, const std::pair<int, string>& b)
		{ return (a.first > b.first); }); // sortowanie wynikow malejaco

}