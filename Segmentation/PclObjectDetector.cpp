#include "PclObjectDetector.h"

using namespace kinect_data;
PclObjectDetector::PclObjectDetector()
	: m_stopWorker(false)
{
	m_worker = boost::shared_ptr<boost::thread> (new boost::thread(&PclObjectDetector::DetectionWorker, this));
}

PclObjectDetector::~PclObjectDetector()
{
	m_stopWorker = true;
	m_worker->join();
}
void PclObjectDetector::Detect(kinect_data::VideoFrame* frame)
{
	if(m_mutexFill.try_lock())
	{
		//prepare dataset to segmetation
		m_seg.CreatePointCloud(frame);
		m_mutexFill.unlock();
		m_condWorkerIsFree.notify_one();
	}
}

void PclObjectDetector::GetObjectVector(std::vector<ObjectBounds>& objects)
{
	objects.clear();
	m_mutexFill.lock();
	objects.resize(m_objects.size());
	std::copy(m_objects.begin(), m_objects.end(), objects.begin());
	m_mutexFill.unlock();
}

void PclObjectDetector::DetectionWorker(PclObjectDetector* ptr)
{
	lock lk(ptr->m_mutexMonitorWorker);
	while (!ptr->m_stopWorker)
	{
		ptr->m_condWorkerIsFree.wait(lk);
		ptr->m_mutexFill.lock();
		//Finding clusters
		ptr->m_seg.FindClusters();
		//Lock vector of objects
		if(ptr->m_seg.m_clusters.size() > 0)
		{
			size_t nrClusters = ptr->m_seg.m_clusters.size();
			ptr->m_objects.clear();
			for(size_t i = 0; i < nrClusters; ++i)
			{
				pcl::PointCloud<pcl::PointXYZ>::Ptr cloud = ptr->m_seg.m_clusters[i];
				if(cloud->size() == 0) continue;
				int xcur(0), ycur(0);
				ptr->m_seg.ConvertPointToScreen(cloud->points[0], xcur, ycur);
				int xmax(xcur), ymax(ycur);
				int xmin(xcur), ymin(ycur);
				for(size_t j = 0; j < cloud->size(); ++j)
				{	
					ptr->m_seg.ConvertPointToScreen(cloud->points[j], xcur, ycur);
					xmax = std::max(xmax, xcur);
					xmin = std::min(xmin, xcur);
					ymax = std::max(ymax, ycur);
					ymin = std::min(ymin, ycur);
				}
				ObjectBounds ob;
				ob.x = xmin; ob.y = ymin;
				ob.width = xmax - xmin; ob.height = ymax - ymin;
				ptr->m_objects.push_back(ob);
			}
			printf("Found %u objects: \n", ptr->m_objects.size());
			for(size_t i = 0; i < ptr->m_objects.size(); ++i)
			{
				printf("P#%u: (%d, %d) Size(%d, %d) - %s\n", i, ptr->m_objects[i].x, ptr->m_objects[i].y, ptr->m_objects[i].width, ptr->m_objects[i].height, ptr->m_seg.m_models[i].c_str());
			}
		}
		else
			printf("The object's not found.\n");
		ptr->m_mutexFill.unlock();
	}
}