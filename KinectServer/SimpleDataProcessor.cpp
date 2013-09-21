#ifdef WIN32
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <Shlwapi.h>
#else
#include <dirent.h>
#endif
#include "SimpleDataProcessor.h"
using namespace kinect_data;
using namespace std;

SimpleDataProcessor::SimpleDataProcessor(const char* objStorePath)
	: m_rgbFrame(NULL)
	, m_depthFrame(NULL)
	, m_jsonLocated(JSON_NODE)
	, m_jsonSkeleton(JSON_NODE)
{
	list<string> files;
	if(GetDirListing(objStorePath, "xml", files))
	{
		for(list<string>::iterator it = files.begin(); it != files.end(); ++it)
		{
			printf("Load image features: %s\n", it->c_str());
			ObjectDefinition* obj = new ObjectDefinition(NULL);
			obj->Load(it->c_str());
			AddVisualObject(*obj);
		}
	}
}

void SimpleDataProcessor::TrainImages(const char* imgPath)
{
	list<string> files;
	if(GetDirListing(imgPath, "png", files))
	{
		for(list<string>::iterator it = files.begin(); it != files.end(); ++it)
		{
			printf("Load image: %s\n", it->c_str());
			ObjectDefinition* obj = new ObjectDefinition(it->c_str());
			AddVisualObject(*obj);
		}
	}
}

bool SimpleDataProcessor::GetDirListing(const char* path, const char* ext, std::list<std::string>& listing)
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

bool SimpleDataProcessor::ProcessColorFrame(void * data, int length)
{
	if(m_rgbFrame)
	{
		delete m_rgbFrame;
	}
	m_rgbFrame = new VideoFrame((unsigned char*) data, (uint32_t) length);
	if(m_rgbFrame->IsOK())
		DetectObject();
	return m_rgbFrame->IsOK();
}

void SimpleDataProcessor::DetectObject()
{
	m_jsonLocated.clear();
	ObjectDefinition featImg(m_rgbFrame->GetWidth(), m_rgbFrame->GetHeight(), 8, m_rgbFrame->GetChannelNum(), m_rgbFrame->GetFrame());
	for(VisualObjectList::iterator it = m_objects.begin(); it != m_objects.end(); ++it)
	{
		CvPoint corners[4];
		if(m_detector.LocateObject(featImg, *it, corners))
		{
			m_jsonLocated.push_back(JSONNode("name", it->GetName().c_str()));
			JSONNode jsonCorners(JSON_ARRAY);
			jsonCorners.set_name("corners");
			for(int i = 0; i < 4; ++i){
				JSONNode jsonPoint(JSON_ARRAY);
				jsonCorners.push_back(JSONNode("", corners[i].x));
				jsonCorners.push_back(JSONNode("", corners[i].y));
				jsonCorners.push_back(jsonPoint);
			}
			m_jsonLocated.push_back(jsonCorners);
			printf("Object found (%d, %d) (%d, %d) (%d, %d) (%d, %d)\n",
					corners[0].x, corners[0].y,
					corners[1].x, corners[1].y,
					corners[2].x, corners[2].y,
					corners[3].x, corners[3].y);
		}
	}
}

bool SimpleDataProcessor::ProcessDepthFrame(void * data, int length)
{
	if(m_depthFrame)
	{
		delete m_depthFrame;
	}
	m_depthFrame = new VideoFrame((unsigned char*) data, (uint32_t) length);
	return m_depthFrame->IsOK();
}

bool SimpleDataProcessor::ProcessSkeletonFrame(void * data, int length)
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

void SimpleDataProcessor::AddVisualObject(ObjectDefinition& objDef)
{
	m_objects.push_back(objDef);
}

