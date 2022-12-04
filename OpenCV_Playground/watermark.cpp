#include "watermark.hpp"



void watermark(cv::Mat src, cv::Mat dst)
{

	bool debug = false;


	cv::Mat watermark = cv::imread("images/d20.png", cv::IMREAD_UNCHANGED);  // the last argument keeps the alpha channel of the image

	uint width = 50;
	uint height = 50;

	cv::Size size(width, height);

	cv::resize(watermark, watermark, size, cv::INTER_LINEAR);

	int height_difference = src.rows - watermark.rows;
	int width_difference = src.cols - watermark.cols;


	cv::copyMakeBorder(watermark, watermark, height_difference, 0, width_difference, 0, cv::BORDER_CONSTANT, (0,0,0,0) );   //(last argument is (B, G, R, alpha) of the edge, since alpha is 0 colors dont really matter


	if (debug)
	{
		std::cout << src.rows << std::endl;
		std::cout << watermark.rows << std::endl;
		std::cout << dst.rows << std::endl;

		std::cout << src.channels() << std::endl;
		std::cout << watermark.channels() << std::endl;

		std::cout << watermark.type() << std::endl;

		std::cout << src.cols << std::endl;
		std::cout << watermark.cols << std::endl;
		std::cout << dst.cols << std::endl;

	}

	cv::addWeighted(src, 1, watermark, 1, 0, dst);




}