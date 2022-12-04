#pragma once
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include "Interfaces.hpp"



class eventManager : public I_Callback
{


};



class webcamMode : public I_Callback, public I_FrameSource
{

private:
	

	cv::Size frameSize;					// Framesizes

	cv::Mat src;						// Material class for the frames

	cv::VideoCapture capture;			// Capture class	


public:

	webcamMode(int deviceID, int apiID, cv::Size inputFrameSize = cv::Size(640, 480), bool isRecording = false);
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
	bool isVideoOver;

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