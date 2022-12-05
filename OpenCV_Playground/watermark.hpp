#pragma once
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/core/base.hpp>

#include <opencv2/highgui.hpp> // for debugging
#include <iostream> // for debugging

#include <time.h> // REC red light blink

void watermark(cv::Mat dst);

void watermarkREC(cv::Mat dst);
