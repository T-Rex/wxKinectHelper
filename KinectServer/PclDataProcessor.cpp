#ifdef WIN32
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <Shlwapi.h>
#else
#include <dirent.h>
#endif
#include "PclDataProcessor.h"
using namespace kinect_data;
using namespace std;

PclDataProcessor::PclDataProcessor(const char* objStorePath)
	: m_rgbFrame(NULL)
	, m_depthFrame(NULL)
	, m_jsonLocated(JSON_NODE)
	, m_jsonSkeleton(JSON_NODE)
{
}

bool PclDataProcessor::GetDirListing(const char* path, const char* ext, std::list<std::string>& listing)
{
#ifdef WIN32
	WIN32_FIND_DATA ffd;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError=0;
	char curPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, curPath);
	PathAppend(curPath, path);
	string dirName(curPath);
	dirName += "\\*.";
	dirName += ext;
	hFind = FindFirstFile(dirName.c_str(), &ffd);

	if (INVALID_HANDLE_VALUE == hFind) 
	{
		return false;
	} 
   	do
	{
		if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			string fn(curPath);
			fn += "\\";
			fn += ffd.cFileName;
			listing.push_back(fn);
		}
	}
	while (FindNextFile(hFind, &ffd) != 0);
 
	FindClose(hFind);
	return true;
#else
	DIR *dir;
    struct dirent *ent;
	int extLen = strlen(ext);
    dir = opendir(path);
	string fn;
    while ((ent=readdir(dir)) != false) {
        fn = ent->d_name;
		if(fn.substr(fn.length() - extLen) == ext)
		{
			listing.push_back(fn);
		}
    }
 
    closedir(dir);
#endif
	return false;
}

bool PclDataProcessor::ProcessColorFrame(void * data, int length)
{
	if(m_rgbFrame)
	{
		delete m_rgbFrame;
	}
	m_rgbFrame = new VideoFrame((unsigned char*) data, (uint32_t) length);

	return m_rgbFrame->IsOK();
}

bool PclDataProcessor::ProcessDepthFrame(void * data, int length)
{
	if(m_depthFrame)
	{
		delete m_depthFrame;
	}
	m_depthFrame = new VideoFrame((unsigned char*) data, (uint32_t) length);
	if(m_depthFrame->IsOK())
	{
		m_detector.Detect(m_depthFrame);
	}
	return m_depthFrame->IsOK();
}

void PclDataProcessor::Dump()
{
	//TODO print statistics
	//size_t nr = m_detector.GetNumberOfObjects();
	//if(nr > 0){
	//	std::vector<ObjectBounds> objects;
	//	m_detector.GetObjectVector(objects);
	//	printf("Found %u objects: \n", objects.size());
	//	for(size_t i = 0; i < objects.size(); ++i)
	//	{
	//		printf("P(%d, %d) Sz(%d, %d)\n", objects[i].x, objects[i].y, objects[i].width, objects[i].height);
	//	}
	//}
}
bool PclDataProcessor::ProcessSkeletonFrame(void * data, int length)
{
	if(length == sizeof(SkeletonFrame)){
		memcpy(&m_skeleton, data, (size_t)length);
		return true;		
	}
	else
	{
		return false;
	}
}

