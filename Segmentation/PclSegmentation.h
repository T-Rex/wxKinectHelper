#pragma once

#include <pcl/ModelCoefficients.h>
#include <pcl/point_types.h>
#include <pcl/sample_consensus/method_types.h>
#include <pcl/sample_consensus/model_types.h>
#include <pcl/segmentation/sac_segmentation.h>
#include "KinectTypes.h"
#include <string>

class PclSegmentation
{
public:
	PclSegmentation();
	void CreatePointCloud(kinect_data::VideoFrame* frame);
	void FindClusters();
	size_t WeightModel(pcl::SacModel model, pcl::PointCloud<pcl::PointXYZ>::Ptr cluster);
	size_t WeightNormalModel(pcl::SacModel model, pcl::PointCloud<pcl::PointXYZ>::Ptr cluster);
	std::string GetModelName(size_t index);
	void ApplyParameters();
	std::vector<pcl::PointCloud<pcl::PointXYZ>::Ptr > m_clusters; 
	std::vector<std::string> m_models;
	void ConvertPointToScreen(pcl::PointXYZ& pt, int& xi, int& yi);
private:
	pcl::SACSegmentation<pcl::PointXYZ> m_seg;
	std::vector<pcl::PointIndices> cluster_indices;
	pcl::PointCloud<pcl::PointXYZ>::Ptr m_cloud;
	size_t m_minPointOfPlane;
	float m_frameWidth;
	float m_frameHeight;
	float m_iterLimit;
	float m_leafSize;
	float m_clusterTolerance; 
	int m_minClusterSize;
	int m_maxClusterSize;
	int m_modelType;
	int m_methodType;
	bool m_bOptimization;
	double m_distanceThreshold;
};