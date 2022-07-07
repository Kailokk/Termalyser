#pragma once
#include <string>
#include <sndfile.h>
#include <portaudio.h>

#include "VisualiserSettings.h"




	static int Callback(const void* input,
		void* output,
		unsigned long frameCount,
		const PaStreamCallbackTimeInfo* timeInfo,
		PaStreamCallbackFlags statusFlags,
		void* audioData);

	bool PlayAudio(std::string* path, std::string* OutputMessage, float*** bufferOut);




