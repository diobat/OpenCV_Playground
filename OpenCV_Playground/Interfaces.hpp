#pragma once


class I_Callback
{

public :
	virtual ~I_Callback() {};
	virtual void callback() {};
};


class I_FrameSource
{
public:
	virtual cv::Mat getFrame() = 0;
};