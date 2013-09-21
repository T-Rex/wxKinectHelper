#pragma once

class KinectDataProcessorBase
{
public:
	virtual bool ProcessColorFrame(void * data, int length) = 0;
	virtual bool ProcessDepthFrame(void * data, int length) = 0;
	virtual bool ProcessSkeletonFrame(void * data, int length) = 0;
	virtual void Dump(){}
};