#pragma once
#include <string>
#include <cv.h>
#include <list>

class ObjectDefinition
{
public:
	ObjectDefinition(const char* imgFileName);
	ObjectDefinition(int width, int height, int depth, int nchannels, const unsigned char* data);
	virtual ~ObjectDefinition(void);
	const CvSeq* GetKeyPoints(){return m_keypoints;}
	const CvSeq* GetDescriptor(){return m_descriptor;}
	const CvPoint* GetCorners(){return m_corners;}
	bool Save(const char* filename);
	bool Save();
	bool Load(const char* filename);
	bool Load();
	bool IsOK(){return m_keypoints != NULL && m_descriptor != NULL;}
	std::string GetName();
private:
	void Init(IplImage* image);
	CvSeq* m_keypoints;
	CvSeq* m_descriptor;
	CvMemStorage* m_storage;
	CvSURFParams m_params;
	CvPoint m_corners[4];
	std::string m_filename;
	std::string m_name;
};

typedef std::list<ObjectDefinition> VisualObjectList;
//WX_DECLARE_LIST(ObjectDefinition, VisualObjectList); 
