#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/utils/logger.hpp>        // Control logging level into command line


#include <opencv2/imgproc.hpp>          // image processing



#include <opencv2/videoio.hpp>          // Video saving abilities

#include <iostream>     // Error messages
#include <list>         // Lists
#include <Windows.h>    // Keypress detection
#include <time.h>       // Generate timestamp for file names
#include <iostream>
#include <filesystem>


///// CUSTOM LIBS

#include "watermark.hpp"
#include "EventFlow.hpp"

/*
cv::VideoCapture capture;
int webcam_width = 640;
int webcam_height = 480;


cv::Mat src(webcam_width, webcam_height, CV_8UC4);
cv::Mat dst(webcam_width, webcam_height, CV_8UC4);
cv::Mat catFrame(webcam_width, webcam_height, CV_8UC4);
*/


cv::Mat src, dst, catFrame;

int morph_elem = 0;
int morph_size = 0;
int morph_operator = 0;
int const max_operator = 4;
int const max_elem = 2;
int const max_kernel_size = 21;

bool continueLoop = true,  isWebcam = false, isRecording = false, isReadingFile = false;

const char* window_name = "Morphology Transformations";

void Morphology_Operations();

int main(int argc, char* argv[])
{

    // First we disable all OpenCV logging
    cv::utils::logging::setLogLevel(cv::utils::logging::LogLevel::LOG_LEVEL_SILENT);


    // Add some arg logging into console for debug feedback purposes
    //std::cout << argc << std::endl;
    //std::cout << strlen(argv[1]) << std::endl;

    if (argc > 3)
    {
        std::cout << "Too many arguments" << std::endl;
        return -1;
    }

    if (argc < 2)
    {
        std::cout << "Too few arguments" << std::endl;
        return -1;
    }



    for (int i = 1; i < argc; i++)
    {
        //std::wcout << argv[i] << std::endl;


        if (strlen(argv[i]) > 2)
        {
            
            isReadingFile = true;

            if (!std::filesystem::exists((std::string)argv[i]))
            {
                std::cout << "No file with that name was found" << std::endl;
                return -1;
            }
            else
            {
                fileMode fileHandler((std::string)argv[i]);
            }

        }



        if (strcmp(argv[i],"w") == 0 && !isReadingFile)
        {
            isWebcam = true;

        }
        else if (strcmp(argv[i], "r") == 0)
        {
            isRecording = true;

        }
        else if (strcmp(argv[i], "h") == 0)
        {
            std::cout << "w             | Read from Webcam mode" << std::endl;
            std::cout << "r             | Recording mode, file will be named output.avi" << std::endl;
            std::cout << "filename      | Read from file mode" << std::endl;


            return 0;
        }
        else
        {
            if (!isReadingFile)
            {
                std::cout << "Unknown argument. Use \'h\' for help" << std::endl;
                return -1;
            }
        }
    }

    // Start the custom classes
    webcamMode webcamHandler(0, cv::CAP_ANY, isRecording);
    recordingMode recordingHandler("output.avi");
    fileMode fileHandler((std::string)argv[1]);


    
    cv::namedWindow(window_name, cv::WINDOW_AUTOSIZE); // Create window

    cv::createTrackbar("Operator:\n 0: Opening - 1: Closing  \n 2: Gradient - 3: Top Hat \n 4: Black Hat", window_name, &morph_operator, max_operator);
    cv::createTrackbar("Element:\n 0: Rect - 1: Cross - 2: Ellipse", window_name, &morph_elem, max_elem);
    cv::createTrackbar("Kernel size:\n 2n +1", window_name, &morph_size, max_kernel_size);
    





    do
    {

        if (isWebcam)
        {
            webcamHandler.callback();
            src = webcamHandler.getFrame();
        }
        else if(isReadingFile)
        {
            fileHandler.callback();
            src = fileHandler.getFrame();
        }

        if (src.empty())
        {
            break;
        }

        //////////////////////////////
        // DO STUFF WITH THE FRAME HERE
         
        

        //dst.release();
        //std::cout << "KICKFLIP" << std::endl;
        cv::flip(src, src, 1);
        dst = src.clone();
        
        // Intensity normalization
        cv::normalize(dst, dst, 255.0, 0.0, cv::NORM_INF);



        Morphology_Operations();




        // STOP DOING STUFF WITH THE FRAME HERE
        //////////////////////////////

        // Add a watermark to the edited frame
        watermark(dst);
        if (isRecording)
        {
            watermarkREC(dst);
        }


        if (isRecording)
        {
            recordingHandler.callback(dst);
        }



        // Horizontally concatenate the frames
        cv::hconcat(src, dst, catFrame);

       

        // Display the frame
        cv::imshow("Live!", catFrame);



        if (cv::waitKey(5) >= 0)
        {
            break;
        }

    } while (true);


    cv::destroyAllWindows();

    std::cout << "Program terminated."  << std::endl;
    return 0;
}



void Morphology_Operations()
{
    // Since MORPH_X : 2,3,4,5 and 6
    int operation = morph_operator + 2;
    cv::Mat element = cv::getStructuringElement(morph_elem, cv::Size(2 * morph_size + 1, 2 * morph_size + 1), cv::Point(morph_size, morph_size));
    morphologyEx(dst, dst, operation, element);
    //imshow(window_name, dst);
}

