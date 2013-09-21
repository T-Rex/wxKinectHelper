#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/imgproc/imgproc_c.h"
#include "ObjectDefinition.h"
//#include <wx/wx.h>
#include <memory.h>
const double hessianThreshold = 200; 

//#include <wx/listimpl.cpp>
//WX_DEFINE_LIST(VisualObjectList);

ObjectDefinition::ObjectDefinition(const char* imgFileName)
	: m_keypoints(NULL)
	, m_descriptor(NULL)
{	
    m_storage = cvCreateMemStorage(0);
	if(imgFileName != NULL)
	{
		IplImage* img = cvLoadImage(imgFileName, CV_LOAD_IMAGE_GRAYSCALE);
		if(img != NULL)
		{
			Init(img);
			m_name = imgFileName;
		}
	}
}

ObjectDefinition::ObjectDefinition(int width, int height, int depth, int nchannels, const unsigned char* data)
	: m_keypoints(NULL)
	, m_descriptor(NULL)
{
    m_storage = cvCreateMemStorage(0);
	if(data != NULL)
	{
		IplImage* img = cvCreateImage(cvSize(width, height), depth, nchannels);
		memcpy((void*)img->imageData, (void*)data, img->imageSize);
		Init(img);
	}
}


ObjectDefinition::~ObjectDefinition(void)
{
}

bool ObjectDefinition::Save()
{
	if(!m_filename.empty())
	{
		return Save(m_filename.c_str());
	}
	return false;
}

bool ObjectDefinition::Load()
{
	if(!m_filename.empty())
	{
		return Load(m_filename.c_str());
	}
	return false;
}

bool ObjectDefinition::Load(const char* filename)
{
	do{
		CvFileStorage* storage = cvOpenFileStorage(filename, 0, CV_STORAGE_READ);
		m_keypoints = (CvSeq*)cvRead(storage, cvGetFileNodeByName(storage, 0, "keypoints"));
		m_descriptor = (CvSeq*)cvRead(storage, cvGetFileNodeByName(storage, 0, "descriptor"));
		cvReadRawData(storage, cvGetFileNodeByName(storage, 0, "corners"), m_corners, "ii");
		cvReleaseFileStorage(&storage);
		m_filename = filename; 
		return true;
	}while(false);
	return false;
}


bool ObjectDefinition::Save(const char* filename)
{
	do{
		CvFileStorage* storage = cvOpenFileStorage(filename, 0, CV_STORAGE_WRITE);
		cvWrite(storage, "keypoints", m_keypoints);
		cvWrite(storage, "descriptor", m_descriptor);
		cvStartWriteStruct(storage, "corners", CV_NODE_SEQ, NULL, cvAttrList(0,0));
		cvWriteRawData(storage, m_corners, 4, "ii");
		cvEndWriteStruct(storage);
		cvReleaseFileStorage(&storage);
		m_filename = filename; 
		return true;
	}while(false);
	return false;
}


void ObjectDefinition::Init(IplImage* image)
{
	m_params = cvSURFParams(hessianThreshold, 1);
	m_corners[0] = cvPoint(0,0);
	m_corners[1] = cvPoint(image->width,0);
	m_corners[2] = cvPoint(image->width,image->height);
	m_corners[3] = cvPoint(0,image->height);
	if(image->nChannels == 3)
	{
		IplImage* grayImage = cvCreateImage(cvSize(image->width, image->height), 8, 1); 
		cvCvtColor(image, grayImage, CV_RGB2GRAY);
		cvExtractSURF(grayImage, 0, &m_keypoints, &m_descriptor, m_storage, m_params);
		cvReleaseImage(&grayImage);
	}
	else{
		cvExtractSURF(image, 0, &m_keypoints, &m_descriptor, m_storage, m_params);
	}
	cvReleaseImage(&image);
}

std::string ObjectDefinition::GetName()
{
	return m_name.empty() ? m_filename : m_name;
}