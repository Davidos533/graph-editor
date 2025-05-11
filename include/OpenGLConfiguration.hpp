#ifndef OPEN_GL_CONFIGURATION_HPP
#define OPEN_GL_CONFIGURATION_HPP

#include "OpenGLColor.hpp"

#include <iostream>

enum class OpenGLProfile
{
	Core,
	Combat
};

struct OpenGLConfiguration final
{
public:

	int MajorVersion;
	int MinorVersion;
	OpenGLProfile UserOpenGLProfile;
	int NumberOfSamples;
	OpenGLColor BackgroundFill;

	OpenGLConfiguration(int majorVersion, int minorVersion, OpenGLProfile userOpenGLProfile, int numberOfSamples, OpenGLColor backgroundFill)
		:MajorVersion(majorVersion), MinorVersion(minorVersion), UserOpenGLProfile(userOpenGLProfile), NumberOfSamples(numberOfSamples), BackgroundFill(backgroundFill)
	{

	}
};

#endif