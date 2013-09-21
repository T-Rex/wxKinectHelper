#pragma once
#include "KinectDataProcessorBase.h"
#include "KinectTypes.h"
#include "ObjectDefinition.h"
#include "ObjectDetector.h"
#include <list>
#include <string>
#include "JSONNode.h"

class SimpleDataProcessor: public KinectDataProcessorBase
{
public:
	SimpleDataProcessor(){}
	SimpleDataProcessor(const char* objStorePath);
	virtual bool ProcessColorFrame(void * data, int length);
	virtual bool ProcessDepthFrame(void * data, int length);
	virtual bool ProcessSkeletonFrame(void * data, int length);
	void AddVisualObject(ObjectDefinition& objDef);
	void TrainImages(const char* imgPath);
	
protected:
	bool GetDirListing(const char* path, const char* ext, std::list<std::string>& listing);	
	void DetectObject();
	VisualObjectList m_objects;
	kinect_data::VideoFrame* m_rgbFrame;
	kinect_data::VideoFrame* m_depthFrame;
	kinect_data::SkeletonFrame m_skeleton;
	ObjectDetector m_detector;
	JSONNode m_jsonLocated;
	JSONNode m_jsonSkeleton;
};