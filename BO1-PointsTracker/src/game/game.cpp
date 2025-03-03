#include "game.hpp"

#include <windows.h>

#include "process.hpp"

#include "../utils/memory.hpp"

#include "../constants.h"

int last_points = 0;
int points_gained = 0;
int points_lost = 0;

namespace game {
	int GetMapId()
	{
		HANDLE handle = process::GetBlackOpsProcess();
		if (handle == NULL || handle == INVALID_HANDLE_VALUE) {
			return -1;
		}

		int map = utils::memory::ReadInt(handle, Constants::C_MAPADDRESS);
		return map;
	}

	int GetGameTime()
	{
		HANDLE handle = process::GetBlackOpsProcess();
		if (handle == NULL || handle == INVALID_HANDLE_VALUE) {
			return -1;
		}

		int time = utils::memory::ReadInt(handle, Constants::C_TIMEADDRESS);
		return time;
	}

	void UpdatePoints()
	{
		HANDLE handle = process::GetBlackOpsProcess();

		// set the stats if the game isnt open
		if (handle == NULL || handle == INVALID_HANDLE_VALUE) {
			last_points = 0;
			points_gained = 0;
			points_lost = 0;
			return; // don't try to calculate stats
		}

		// make sure the stats of the points get reset when we arent on a map with points
		int current_map = GetMapId();
		if (GetGameTime() == 0
			|| current_map == Constants::NO_MAP
			|| current_map == Constants::INVALID_MAP 
			|| current_map == Constants::MAIN_MENU_ID)
		{
			last_points = 0;
			points_gained = 0;
			points_lost = 0;
			return; // don't try to calculate stats
		}

		// calculate the difference for between the current points and last points amount tracked
		int current_points = utils::memory::ReadInt(handle, Constants::C_WHITE_POINTS);
		int difference = current_points - last_points;

		// track points gained
		if (difference > 0)
		{
			last_points = current_points;
			points_gained += difference;
		}
		// track points lost
		else if (difference < 0)
		{
			last_points = current_points;
			points_lost += -difference;
		}
	}

	// the points gained string that gets put on the display
	std::string GetPointsGained()
	{
		std::string prefix = "Points Gained: ";

		// if the game isnt open then display zero
		// we already reset the stats in UpdatePoints
		if (!game::process::IsGameOpen())
		{
			return prefix + std::to_string(0);
		}

		return prefix + std::to_string(points_gained);
	}

	// the points lost string that gets put on the display
	std::string GetPointsLost()
	{
		std::string prefix = "Points Lost: ";

		// if the game isnt open then display zero
		// we already reset the stats in UpdatePoints
		if (!game::process::IsGameOpen())
		{
			return prefix + std::to_string(0);
		}

		return prefix + std::to_string(points_lost);
	}
}