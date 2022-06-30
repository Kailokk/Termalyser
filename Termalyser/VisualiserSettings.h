#pragma once
#include <string>
#include <sndfile.h>

enum VisualiserMode
{
	VISMODE_SCOPE,
	VISMODE_PARTICLESCOPE,
	VISMODE_SPECT,
	VISMODE_TIMESPECT
};

struct VisualiserSettings
{
	std::string path;
	VisualiserMode visMode;
};


struct AudioData
{
	SNDFILE* file;
	SF_INFO info;
	int position;//index to the file
};