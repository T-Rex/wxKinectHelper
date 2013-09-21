#pragma once
#include <vector>

class ObjectDefinition;
class ObjectDetector
{
public:
	ObjectDetector();
	~ObjectDetector();
	bool LocateObject(ObjectDefinition& object, IplImage* image, CvPoint corners[4]);
	bool LocateObject(ObjectDefinition& object, ObjectDefinition& imageDefinition, CvPoint corners[4]);
private:
	bool LocatePlanarObject(const CvSeq* objectKeypoints, const CvSeq* objectDescriptors,
                    const CvSeq* imageKeypoints, const CvSeq* imageDescriptors,
                    const CvPoint src_corners[4], CvPoint dst_corners[4]);
	void FlannFindPairs(const CvSeq*, const CvSeq* objectDescriptors,
           const CvSeq*, const CvSeq* imageDescriptors, std::vector<int>& ptpairs );
	void FindPairs(const CvSeq* objectKeypoints, const CvSeq* objectDescriptors,
           const CvSeq* imageKeypoints, const CvSeq* imageDescriptors, std::vector<int>& ptpairs);
	int NaiveNearestNeighbor(const float* vec, int laplacian,
                      const CvSeq* model_keypoints,
                      const CvSeq* model_descriptors);
	double CompareDescriptors(const float* d1, const float* d2, double best, int length);


};