#include <iostream>
#include <pcl/ModelCoefficients.h>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl/features/normal_3d.h>
#include <pcl/filters/extract_indices.h>
#include <pcl/filters/passthrough.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/kdtree/kdtree.h>
#include <pcl/sample_consensus/method_types.h>
#include <pcl/sample_consensus/model_types.h>
#include <pcl/segmentation/sac_segmentation.h>
#include <pcl/segmentation/extract_clusters.h>
#include "PclSegmentation.h"
//from Kinect SDK:
#define NUI_CAMERA_DEPTH_IMAGE_TO_SKELETON_MULTIPLIER_320x240 (3.501e-3f)

using namespace kinect_data;
PclSegmentation::PclSegmentation()
	: m_modelType(pcl::SACMODEL_PLANE)
	, m_methodType(pcl::SAC_RANSAC)
	, m_bOptimization(true)
	, m_distanceThreshold(0.02)
	, m_iterLimit(0.3f)
	, m_leafSize(0.005f)
	, m_clusterTolerance(0.02f)
	, m_minClusterSize(100)
	, m_maxClusterSize(25000)
{
	ApplyParameters();
}

void PclSegmentation::ApplyParameters()
{
	m_seg.setOptimizeCoefficients(m_bOptimization);
	m_seg.setModelType(m_modelType);
	m_seg.setMethodType(m_methodType);
	m_seg.setDistanceThreshold(m_distanceThreshold);
	m_seg.setMaxIterations(100);
}

void PclSegmentation::CreatePointCloud(VideoFrame* frame)
{
	//if(m_cloud) delete m_cloud;
	m_cloud = pcl::PointCloud<pcl::PointXYZ>::Ptr(new pcl::PointCloud<pcl::PointXYZ>());
	m_frameWidth = (float)frame->GetWidth();
	m_frameHeight = (float)frame->GetHeight();
	int nchannels = frame->GetChannelNum(); 
	size_t i(0);
	const unsigned char* data = frame->GetFrame();
	float cnt(0.0f);
	float step(1.0f);
	for(float yi = 0; yi < m_frameHeight; yi += step)
	{
		for(float xi = 0; xi < m_frameWidth; xi += step)
		{
			pcl::PointXYZ point;
			point.z = (float)*((unsigned short*)(data));
			if(point.z != 0 ){
				point.z /= 1000.0f;
				point.x = (xi - m_frameWidth / 2.0f) * NUI_CAMERA_DEPTH_IMAGE_TO_SKELETON_MULTIPLIER_320x240 * point.z;
				point.y = -(yi - m_frameHeight / 2.0f) * NUI_CAMERA_DEPTH_IMAGE_TO_SKELETON_MULTIPLIER_320x240 * point.z;
				m_cloud->points.push_back(point);
			}
			data += ((int)step * nchannels);
		}
	}
	m_cloud->width = m_cloud->points.size();
	m_cloud->height = 1;
	m_cloud->is_dense = true;
	//pcl::PCDWriter writer;
	//writer.writeBinaryCompressed("depth.pcd", *m_cloud);
	m_minPointOfPlane = 4;
}

void PclSegmentation::ConvertPointToScreen(pcl::PointXYZ& pt, int& xi, int& yi)
{
	xi = (pt.x / (NUI_CAMERA_DEPTH_IMAGE_TO_SKELETON_MULTIPLIER_320x240 * pt.z)) + m_frameWidth / 2.0f; 
	yi = m_frameHeight / 2.0f - (pt.y / (NUI_CAMERA_DEPTH_IMAGE_TO_SKELETON_MULTIPLIER_320x240 * pt.z)); 
}

void PclSegmentation::FindClusters()
{
	//Downsampling points
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_origin(m_cloud);
	pcl::VoxelGrid<pcl::PointXYZ> vg;
	pcl::PassThrough<pcl::PointXYZ> ps;
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_filtered (new pcl::PointCloud<pcl::PointXYZ>);
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_passed (new pcl::PointCloud<pcl::PointXYZ>);
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_filtered2 (new pcl::PointCloud<pcl::PointXYZ>);
	vg.setInputCloud(cloud_origin);
	//1 cm per point
	vg.setLeafSize(m_leafSize, m_leafSize, m_leafSize);
	vg.filter(*cloud_passed);
	//limit distance
	ps.setInputCloud(cloud_passed);
	ps.setFilterFieldName("z");
	ps.setFilterLimits(0.0, 2.0);
	ps.filter(*cloud_filtered);
	//Finding all big plane and extract they from cloud
	pcl::PointIndices::Ptr inliers (new pcl::PointIndices);
	pcl::ModelCoefficients::Ptr coefficients (new pcl::ModelCoefficients);
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_plane (new pcl::PointCloud<pcl::PointXYZ> ());

	int i=0, nr_points = (int) cloud_filtered->points.size ();
	while (cloud_filtered->points.size () > m_iterLimit * nr_points)
	{
		// Segment the largest planar component from the remaining cloud
		m_seg.setInputCloud(cloud_filtered);
		m_seg.segment (*inliers, *coefficients); 
		if (inliers->indices.size () == 0)
		{
			//Could not estimate a planar model for the given dataset
			break;
		}

		// Extract the planar inliers from the input cloud
		pcl::ExtractIndices<pcl::PointXYZ> extract;
		extract.setInputCloud (cloud_filtered);
		extract.setIndices (inliers);
		extract.setNegative (false);
		extract.filter (*cloud_plane); 

		// Remove the planar inliers, extract the rest
		extract.setNegative (true);
		extract.filter (*cloud_filtered2); 
		cloud_filtered.swap(cloud_filtered2);
	}
	m_clusters.clear();
	if(cloud_filtered->size() == 0) return;
	// Creating the KdTree object for the search method of the extraction
	pcl::search::KdTree<pcl::PointXYZ>::Ptr tree (new pcl::search::KdTree<pcl::PointXYZ>);
	tree->setInputCloud (cloud_filtered);

	std::vector<pcl::PointIndices> cluster_indices;
	pcl::EuclideanClusterExtraction<pcl::PointXYZ> ec;
	ec.setClusterTolerance(m_clusterTolerance); // 2cm
	ec.setMinClusterSize(m_minClusterSize);
	ec.setMaxClusterSize(m_maxClusterSize);
	ec.setSearchMethod(tree);
	ec.setInputCloud(cloud_filtered);
	ec.extract(cluster_indices);
	
	//Add cluster point clouds to vector of clusters
	int j = 0;
	m_clusters.resize(cluster_indices.size());
	m_models.resize(cluster_indices.size());
	for (std::vector<pcl::PointIndices>::const_iterator it = cluster_indices.begin (); it != cluster_indices.end (); ++it)
	{
		pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_cluster (new pcl::PointCloud<pcl::PointXYZ>);
		for (std::vector<int>::const_iterator pit = it->indices.begin (); pit != it->indices.end (); pit++)
			cloud_cluster->points.push_back (cloud_filtered->points[*pit]); 
		m_clusters[j] = cloud_cluster;
		m_models[j] = GetModelName(j);
		j++;
	}
}

std::string PclSegmentation::GetModelName(size_t index)
{
	size_t np = 4;
	std::string modelName("UNKNOWN MODEL");
	std::map<int, std::string> models;
	models[-1] = std::string("UNKNOWN MODEL");
	models[pcl::SACMODEL_PLANE] = std::string("PLANE");
	models[pcl::SACMODEL_CYLINDER] = std::string("CYLINDER");
	models[pcl::SACMODEL_SPHERE] = std::string("SPHERE");
//	models[pcl::SACMODEL_REGISTRATION] = std::string("REGISTRATION");
	models[pcl::SACMODEL_CIRCLE2D] = std::string("CIRCLE2D");
	std::map<int, std::string>::iterator itMax = models.begin();
	std::map<int, std::string>::iterator it = models.begin();
	for(++it; it != models.end(); ++it)
	{
		printf("For %s ", it->second.c_str());
		size_t weight = WeightNormalModel((pcl::SacModel)(it->first), m_clusters[index]); 
		if(np < weight) {
			np = weight;
			itMax = it;
		}
	}
	return itMax->second;
}

size_t PclSegmentation::WeightNormalModel(pcl::SacModel model, pcl::PointCloud<pcl::PointXYZ>::Ptr cluster)
{
  pcl::NormalEstimation<pcl::PointXYZ, pcl::Normal> ne;
  pcl::SACSegmentationFromNormals<pcl::PointXYZ, pcl::Normal> seg; 
  pcl::PointCloud<pcl::Normal>::Ptr cloud_normals (new pcl::PointCloud<pcl::Normal>);
  pcl::search::KdTree<pcl::PointXYZ>::Ptr tree (new pcl::search::KdTree<pcl::PointXYZ> ());
  pcl::ModelCoefficients::Ptr coefficients (new pcl::ModelCoefficients);
  pcl::PointIndices::Ptr inliers (new pcl::PointIndices);
  tree->setInputCloud (cluster);
  // Estimate point normals
  ne.setSearchMethod (tree);
  ne.setInputCloud (cluster);
  ne.setKSearch (100);
  ne.compute (*cloud_normals);

// Create the segmentation object for cylinder segmentation and set all the parameters
  seg.setOptimizeCoefficients (false);
  seg.setModelType (model);
  seg.setMethodType (pcl::SAC_RANSAC);
  seg.setNormalDistanceWeight (0.01);
  seg.setMaxIterations (100);
  seg.setProbability(1.0);
  seg.setDistanceThreshold (0.005);
  seg.setRadiusLimits (0.05, 0.2);
  seg.setInputCloud (cluster);
  seg.setInputNormals (cloud_normals);

  // Obtain the cylinder inliers and coefficients
  seg.segment (*inliers, *coefficients);
  printf("%u points found\n", inliers->indices.size());
  return inliers->indices.size();
} 

size_t PclSegmentation::WeightModel(pcl::SacModel model, pcl::PointCloud<pcl::PointXYZ>::Ptr cluster)
{
  //pcl::NormalEstimation<pcl::PointXYZ, pcl::Normal> ne;
	pcl::SACSegmentation<pcl::PointXYZ> seg;
	pcl::ModelCoefficients::Ptr coefficients (new pcl::ModelCoefficients);
    pcl::PointIndices::Ptr inliers (new pcl::PointIndices);
    pcl::search::KdTree<pcl::PointXYZ>::Ptr tree (new pcl::search::KdTree<pcl::PointXYZ> ());
  //// Estimate point normals
  //ne.setSearchMethod (tree);
  //ne.setInputCloud (cluster);
  //ne.setKSearch (50);
  //ne.compute (*cloud_normals);

// Create the segmentation object for cylinder segmentation and set all the parameters
  seg.setOptimizeCoefficients (false);
  seg.setModelType (model);
  seg.setMethodType (pcl::SAC_RANSAC);
  seg.setMaxIterations (100);
  //seg.setProbability(1.0);
  seg.setDistanceThreshold (0.01);
  seg.setRadiusLimits (0.05, 0.2);
  seg.setInputCloud (cluster);
  //seg.setInputNormals (cluster);

  // Obtain the cylinder inliers and coefficients
  seg.segment (*inliers, *coefficients);
  printf("%u points found\n", inliers->indices.size());
  return inliers->indices.size();
} 