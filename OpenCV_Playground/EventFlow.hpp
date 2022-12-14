#pragma once
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

// std
#include <filesystem>

// own libs
#include "Interfaces.hpp"



class webcamMode : public I_Callback, public I_FrameSource
{

private:
	

	cv::Size frameSize;					// Framesizes
	cv::Mat src;						// Material class for the frames
	cv::VideoCapture capture;			// Capture class	
	bool recordingMode;

public:

	webcamMode(int deviceID, int apiID, bool isRecording = false, cv::Size inputFrameSize = cv::Size(640, 480));
	void callback(void);
	cv::Mat getFrame(void);
	~webcamMode();
};



class fileMode : public I_Callback, public I_FrameSource
{
private:
	cv::Size frameSize;
	cv::Mat src;
	cv::VideoCapture capture;

	bool wasInitialized;

public:

	fileMode(std::string filePath);
	void callback();
	cv::Mat getFrame();
	~fileMode();
};


class recordingMode : public I_Callback
{
private:

	cv::Size frameSize;
	cv::Mat frameToRecord;
	cv::VideoWriter recorder;

public:

	recordingMode(std::string outputFilename, cv::Size inputFrameSize = cv::Size(640, 480));
	void callback(cv::Mat nextFrame);
	~recordingMode();
};