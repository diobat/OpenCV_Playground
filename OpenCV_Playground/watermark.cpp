#include "watermark.hpp"



void watermark(cv::Mat src)
{


	bool debug = false;

	
	cv::Mat watermark = cv::imread("images/d20.png", cv::IMREAD_UNCHANGED);  // the last argument keeps the alpha channel of the image

	uint width = 50;
	uint height = 50;

	cv::Size size(width, height);

	cv::resize(watermark, watermark, size, cv::INTER_LINEAR);

	int height_difference = src.rows - watermark.rows;
	int width_difference = src.cols - watermark.cols;


	//(last argument is (B, G, R, alpha) of the edge, since alpha is 0 colors dont really matter
	cv::copyMakeBorder(watermark, watermark, height_difference, 0, width_difference, 0, cv::BORDER_CONSTANT, (0, 0, 0, 0));


	if (debug)
	{
		std::cout << src.rows << std::endl;
		std::cout << watermark.rows << std::endl;
		//std::cout << dst.rows << std::endl;

		std::cout << src.channels() << std::endl;
		std::cout << watermark.channels() << std::endl;

		std::cout << watermark.type() << std::endl;

		std::cout << src.cols << std::endl;
		std::cout << watermark.cols << std::endl;
		//std::cout << dst.cols << std::endl;

	}

	cv::addWeighted(src, 1, watermark, 1, 0, src);



}



void watermarkREC(cv::Mat dst)
{


	time_t seconds = time(NULL);

	//std::cout << (seconds % 2 == 1) << std::endl;


	// Import both watermarks
	cv::Mat rec  = cv::imread("images/rec2.png", cv::IMREAD_UNCHANGED);
	cv::Mat rec2 = cv::imread("images/rec1.png", cv::IMREAD_UNCHANGED);


	// Import the watermark images
	// the last argument keeps the alpha channel of the image

	uint width = 100;
	uint height = 40;

	cv::Size size(width, height);


	// Based on current system time we apply one of the two watermarks
	if (seconds % 2)
	{
		cv::resize(rec, rec, size, cv::INTER_LINEAR);
	}
	else
	{
		cv::resize(rec2, rec, size, cv::INTER_LINEAR);
	}



	int height_difference = dst.rows - rec.rows;
	int width_difference = dst.cols - rec.cols;

	float positioning_ratio_h = 0.95f;
	float positioning_ratio_v = 0.05;

	int left_padding = height_difference * positioning_ratio_h;
	int right_padding = height_difference * (1.0f - positioning_ratio_h);
	int top_padding = width_difference * positioning_ratio_v;
	int bottom_padding = width_difference * (1.0f - positioning_ratio_v);


	// Add transparent border to the watermark to it matches the size of the frame
	//(last argument is (B, G, R, alpha) of the edge, since alpha is 0 colors dont really matter
	cv::copyMakeBorder(rec, rec, top_padding, bottom_padding, left_padding, right_padding, cv::BORDER_CONSTANT, (0, 0, 0, 0));

	
	// To prevent round error-caused mismatches in frame size 
	cv::resize(rec, rec, cv::Size(dst.cols, dst.rows), cv::INTER_LINEAR);


	if (false)
	{
		std::cout << dst.rows << std::endl;
		std::cout << rec.rows << std::endl;
		//std::cout << dst.rows << std::endl;

		std::cout << dst.channels() << std::endl;
		std::cout << rec.channels() << std::endl;

		std::cout << rec.type() << std::endl;

		std::cout << dst.cols << std::endl;
		std::cout << rec.cols << std::endl;
		//std::cout << dst.cols << std::endl;

	}


	// Add the two frames 
	cv::addWeighted(dst, 1, rec, 1, 0, dst);

}