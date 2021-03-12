#pragma once
#include <string>
#include <vector>

class Leaderboard
{
	std::vector<std::pair<int, std::string>> entries;
	std::string leaderboards_filename;
	int max_size;
public:
	Leaderboard()
	{
		max_size = 10;
		entries.reserve(max_size);
		leaderboards_filename = "Leaderboards.txt";
	}
	std::vector<std::pair<int, std::string>>& get_entries()
	{
		return entries;
	}
	void load_leaderboards();
	void save_leaderboards();
	void add_to_leaderboard(int value, std::string player_name);
};

