#include "PlayAudio.h"
#include <iostream>
//Audio Buffers
float monoBufferSize[FRAMES_PER_BUFFER/2];
float* monoBuffer = (float*)malloc(sizeof(monoBufferSize));
float* leftChannelBuffer = (float*)malloc(sizeof(monoBufferSize));
float* rightChannelBuffer = (float*)malloc(sizeof(monoBufferSize));

StereoSignal stereoBuffer;

//Buffer Read Size
int readSize = 0;

bool PlayAudio(std::string* path, std::string* OutputMessage, float*** monoBufferOut, StereoSignal* stereoBufferOut)
{
	//Initialisations
	AudioData* data = (AudioData*)malloc(sizeof(AudioData));
	PaError error;
	PaStreamParameters outputParameters;
	PaStream* stream;
	data->position = 0;
	data->info.format = 0;
	
	stereoBuffer.leftChannel = leftChannelBuffer;
	stereoBuffer.rightChannel = rightChannelBuffer;

	//Open File
	data->file = sf_open(path->c_str(), SFM_READ, &data->info);
	if (sf_error(data->file) != SF_ERR_NO_ERROR)
	{
		*OutputMessage = "Problem Opening File";
		return false;
	}
	error = Pa_Initialize();
	
	if (error != paNoError)
	{
		*OutputMessage = "Problem Initialising Portaudio";
		return false;
	}

	outputParameters.device = Pa_GetDefaultOutputDevice();
	outputParameters.channelCount = data->info.channels;
	outputParameters.sampleFormat = paFloat32;
	outputParameters.suggestedLatency = 0.2;
	outputParameters.hostApiSpecificStreamInfo = 0;
	
	error = Pa_OpenStream(&stream,0, &outputParameters, data->info.samplerate, FRAMES_PER_BUFFER, paNoFlag, Callback, data);
	if (error != paNoError)
	{
		*OutputMessage = "Problem Opening Stream";
		return false;
	}
	
	error = Pa_StartStream(stream);
	if (error != paNoError)
	{
		*OutputMessage = "Problem starting Stream";
		return false;
	}

	while (Pa_IsStreamActive(stream))
	{
		if (monoBuffer != nullptr || stereoBuffer.rightChannel != nullptr)
		{
			*monoBufferOut = &monoBuffer;
			stereoBufferOut = &stereoBuffer;
		}
		Pa_Sleep(100);
	}

	sf_close(data->file);

	error = Pa_CloseStream(stream);
	if (error != paNoError)
	{
		*OutputMessage = "Problem Closing Stream";
		return false;
	}

	error = Pa_Terminate();
	if (error != paNoError)
	{
		"Problem Terminating Portaudio";
		return false;
	}
	return true;
}


static int Callback(const void* input,
	void* output,
	unsigned long frameCount,
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags,
	void* audioData)
{
	AudioData* data = (AudioData*)audioData;
	float* cursor = (float*)output;
	int frameSize = frameCount;
	int nFrames = frameCount;
	int currentRead;

	//set cursor to beginning

	while (frameSize > 0)
	{
		//seek out current position in file
		sf_seek(data->file, data->position, SEEK_SET);

		//Prevent reading past end of file
		if (frameSize > (data->info.frames - data->position))
		{
			//if thats gonna happen, shorten the read to the end
			currentRead = data->info.frames - data->position;

			//then loop to the beginning
			data->position = 0;
		}
		else
		{
			//Otherwise continue filling output buffer as normal
			currentRead = frameSize;
			//and increment the file position
			data->position += currentRead;
		}

		//Read straight into the buffer
		nFrames = sf_readf_float(data->file, cursor, currentRead);
		readSize = currentRead;
		
		int monoSize = FRAMES_PER_BUFFER / 2;
		int doubleIterator = 0;
		for (int i = 0; i < monoSize; i++)
		{
			leftChannelBuffer[i] = cursor[doubleIterator];
			rightChannelBuffer[i] = cursor[doubleIterator +1];
			monoBuffer[i] = leftChannelBuffer[i] + rightChannelBuffer[i];
			doubleIterator += 2;
		}

		if (nFrames < frameCount)
		{
			//Finish playing audio
			return paComplete;
		}
		//Increment the cursor
		cursor += currentRead;
		//Decrement the count of samples left
		frameSize -= currentRead;
	}
	//Continue playing audio
	return paContinue;
}