#pragma once

#include "PclSegmentation.h"
#include <boost/thread.hpp>
#include <boost/thread/condition.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>
#include "KinectTypes.h"

struct ObjectBounds
{
	int x, y;
	int width, height;
};

typedef boost::mutex::scoped_lock lock;

class PclObjectDetector
{
public:
	PclObjectDetector();
	~PclObjectDetector();
	void Detect(kinect_data::VideoFrame* frame);
	static void DetectionWorker(PclObjectDetector* ptr); 
	void GetObjectVector(std::vector<ObjectBounds>& objects);
	size_t GetNumberOfObjects(){return m_objects.size();}
private:
	boost::shared_ptr<boost::thread> m_worker;
	boost::mutex m_mutexFill;
	PclSegmentation m_seg;
	std::vector<ObjectBounds> m_objects;
    boost::condition m_condWorkerIsFree;
    boost::mutex m_mutexMonitorWorker;
	bool m_stopWorker;
};