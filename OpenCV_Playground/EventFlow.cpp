#include "EventFlow.hpp"
#include <opencv2/imgproc.hpp>          // image processing
#include "watermark.hpp"




///////////////
// Webcam class definitions
///////////////


webcamMode::webcamMode(int deviceID, int apiID, cv::Size inputFrameSize, bool isRecording)
{


	// Initialize the image frames
	cv::Mat src(inputFrameSize, CV_8UC4);
	cv::Mat dst(inputFrameSize, CV_8UC4);
	cv::Mat catFrame(inputFrameSize, CV_8UC4);


	// Disable auto white balance (Currently not working)
	capture.set(cv::CAP_PROP_AUTO_WB, 0);
	capture.set(cv::CAP_PROP_WB_TEMPERATURE, 7200);


	// Intialize the capture of webcam frames
	capture.open(deviceID, apiID);




	if (!capture.isOpened())
	{
		std::cerr << "ERROR: CAMERA IS NOT WORKING" << std::endl;
	}


}

// Gets called once per loop in the main function
void webcamMode::callback()
{
	// Capture the frame
	capture.read(src);
	// Convert the captured frame into a 4 channel, by adding alpha channel
	cv::cvtColor(src, src, cv::COLOR_BGR2BGRA);

}

cv::Mat webcamMode::getFrame()
{
	return src;
}

webcamMode::~webcamMode()
{
	// Release the capture resources
	std::cout << "Releasing webcam" << std::endl;
	capture.release();
}




///////////////
// File class definitions
///////////////


fileMode::fileMode(std::string inputFileLocation)
{
	// Iniitate the capture class
	capture = cv::VideoCapture(inputFileLocation);
	isVideoOver = false;

	// Check if camera opened successfully
	if (!capture.isOpened()) {
		std::cout << "Error opening video stream or file" << std::endl;
	}


	// Save the frame size in an easier to acess location
	frameSize = cv::Size(capture.get(cv::CAP_PROP_FRAME_WIDTH), capture.get(cv::CAP_PROP_FRAME_HEIGHT));

}

void fileMode::callback()
{
	// Capture the frame
	capture.read(src);
	// Convert the captured frame into a 4 channel, by adding alpha channel
	cv::cvtColor(src, src, cv::COLOR_BGR2BGRA);
}

cv::Mat fileMode::getFrame()
{
	return src;
}

fileMode::~fileMode()
{
	// Release the capture resources
	std::cout << "Releasing videoreader" << std::endl;
	capture.release();
}






///////////////
// Recorder class definitions
///////////////

recordingMode::recordingMode(std::string outputFilename, cv::Size inputFrameSize)
{
	// Store the frame size
	frameSize = inputFrameSize;


	// Create the recorder object
	recorder = cv::VideoWriter(outputFilename, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 10, inputFrameSize, true);


}


void recordingMode::callback(cv::Mat nextFrame)
{
	// check if sizes match
	if ((nextFrame.rows == frameSize.height) && (nextFrame.cols == frameSize.width))
	{
		// Remove the alpha channel otherwise the recorder wont work
		cv::cvtColor(nextFrame, frameToRecord, cv::COLOR_BGRA2BGR); 
		// Save the frame
		recorder.write(frameToRecord);
	}
	else
	{
		std::cerr << "Size mismatch between frame and recorder" << std::endl;
	}

}


recordingMode::~recordingMode()
{
	// Release the recorder resources
	std::cout << "Releasing videorecorder" << std::endl;
	recorder.release();
}


