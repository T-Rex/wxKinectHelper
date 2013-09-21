#pragma once
#include "KinectDataProcessorBase.h"
#include "KinectTypes.h"
#include <list>
#include <string>
#include "JSONNode.h"
#include "PclObjectDetector.h"

class PclDataProcessor: public KinectDataProcessorBase
{
public:
	PclDataProcessor(){}
	PclDataProcessor(const char* objStorePath);
	virtual bool ProcessColorFrame(void * data, int length);
	virtual bool ProcessDepthFrame(void * data, int length);
	virtual bool ProcessSkeletonFrame(void * data, int length);
	virtual void Dump();
protected:
	bool GetDirListing(const char* path, const char* ext, std::list<std::string>& listing);	
	kinect_data::VideoFrame* m_rgbFrame;
	kinect_data::VideoFrame* m_depthFrame;
	kinect_data::SkeletonFrame m_skeleton;
	PclObjectDetector m_detector;
	JSONNode m_jsonLocated;
	JSONNode m_jsonSkeleton;
};