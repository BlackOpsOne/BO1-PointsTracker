#include <SFML/Graphics.hpp>

#include "settings/settings.hpp"

#include "game/game.hpp"

#include "resources/icon.h"

#include "resources/font.h"

#include "constants.h"

#include <thread>

#include <atomic>

#include <sstream>

#include <iostream>

#include <windows.h>

std::atomic<bool> pointsThreadRunning(true);

static void UpdatePointsThread()
{
	while (pointsThreadRunning)
	{
		game::UpdatePoints();
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// load settings
	settings::LoadSettings();

	// start update thread
	std::thread pointsThread(UpdatePointsThread);

	bool show_lost = settings::GetValue("DisplayPointsLost") == "Yes";

	int window_width = 375;
	int window_height = show_lost ? 100 : 75;

	// load the icon
	sf::Image icon;
	if (!icon.loadFromMemory(resources::icon_png, resources::icon_png_len))
	{
		MessageBoxA(NULL, "Could not load asset \"icon_png\".", "BO1 Points Tracker", MB_OK | MB_ICONERROR);
		return -1;
	}

	// load font
	sf::Font font;
	if (!font.loadFromMemory(resources::font_ttf, resources::font_ttf_len))
	{
		MessageBoxA(NULL, "Could not load asset \"font_ttf\".", "BO1 Points Tracker", MB_OK | MB_ICONERROR);
		return -1;
	}

	// gained points text
	sf::Text gained_points_text("", font, 20);
	gained_points_text.setFillColor(sf::Color::White);

	// lost points text
	sf::Text lost_points_text("", font, 20);
	lost_points_text.setFillColor(sf::Color::White);

	// create the main_window
	sf::RenderWindow main_window(sf::VideoMode(window_width, window_height), L"BO1 Points Tracker", sf::Style::Titlebar | sf::Style::Close);
	main_window.setFramerateLimit(10);

	// remove minimize button
	HWND hwnd = main_window.getSystemHandle();
	LONG style = GetWindowLong(hwnd, GWL_STYLE);
	style &= ~WS_MINIMIZEBOX;
	SetWindowLong(hwnd, GWL_STYLE, style);
	SetWindowPos(hwnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

	// set the icon
	main_window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

	while (main_window.isOpen())
	{
		// wait for the close button to be pressed
		sf::Event event;
		while (main_window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) {
				main_window.close();
			}
		}

		float y_offset = 20;
		if (!show_lost)
		{
			y_offset = 5;
		}

		// draw gained points
		gained_points_text.setString(game::GetPointsGained());
		float gained_width = gained_points_text.getGlobalBounds().width;
		float gained_height = gained_points_text.getGlobalBounds().height;
		gained_points_text.setPosition((window_width - gained_width) / 2, ((window_height - gained_height) / 2) - y_offset);

		// draw lost points
		if (show_lost)
		{
			lost_points_text.setString(game::GetPointsLost());
			float lost_width = lost_points_text.getGlobalBounds().width;
			float lost_height = lost_points_text.getGlobalBounds().height;
			lost_points_text.setPosition((window_width - lost_width) / 2, ((window_height - lost_height) / 2) + (y_offset / 2));
		}

		// draw everything to the main_window
		main_window.clear(sf::Color(5, 5, 5));
		main_window.draw(gained_points_text);

		if (show_lost)
		{
			main_window.draw(lost_points_text);
		}
		
		main_window.display();
	}

	// clean up the update thread
	pointsThreadRunning = false;
	pointsThread.join();

	// when its closed save settings
	settings::SaveSettings(false);

	return 0;
}