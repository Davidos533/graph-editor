#ifndef WINDOW_CONFIGURATION_HPP
#define WINDOW_CONFIGURATION_HPP

#include <string>
#include <iostream>

struct WindowConfiguration final
{
public:
	int Width;
	int Height;
	std::string Title;

	WindowConfiguration(int width, int height, const std::string& title)
		:Width(width), Height(height), Title(title)
	{

	}
};

#endif