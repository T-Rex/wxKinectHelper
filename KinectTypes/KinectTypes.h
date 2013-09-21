#pragma once
#include <vector>
#include <stdint.h>

namespace kinect_data{
enum KinectDataType
{
	CONNECTION_INIT,
	FRAME_COLOR,
	FRAME_DEPTH,
	FRAME_SKELETON,

	KINECT_DATA_NONE
};

typedef enum _NUI_SKELETON_POSITION_INDEX
{
    NUI_SKELETON_POSITION_HIP_CENTER = 0,
    NUI_SKELETON_POSITION_SPINE,
    NUI_SKELETON_POSITION_SHOULDER_CENTER,
    NUI_SKELETON_POSITION_HEAD,
    NUI_SKELETON_POSITION_SHOULDER_LEFT,
    NUI_SKELETON_POSITION_ELBOW_LEFT,
    NUI_SKELETON_POSITION_WRIST_LEFT,
    NUI_SKELETON_POSITION_HAND_LEFT,
    NUI_SKELETON_POSITION_SHOULDER_RIGHT,
    NUI_SKELETON_POSITION_ELBOW_RIGHT,
    NUI_SKELETON_POSITION_WRIST_RIGHT,
    NUI_SKELETON_POSITION_HAND_RIGHT,
    NUI_SKELETON_POSITION_HIP_LEFT,
    NUI_SKELETON_POSITION_KNEE_LEFT,
    NUI_SKELETON_POSITION_ANKLE_LEFT,
    NUI_SKELETON_POSITION_FOOT_LEFT,
    NUI_SKELETON_POSITION_HIP_RIGHT,
    NUI_SKELETON_POSITION_KNEE_RIGHT,
    NUI_SKELETON_POSITION_ANKLE_RIGHT,
    NUI_SKELETON_POSITION_FOOT_RIGHT,
    NUI_SKELETON_POSITION_COUNT
} NUI_SKELETON_POSITION_INDEX;

typedef struct _NUI_TRANSFORM_SMOOTH_PARAMETERS {
    float fSmoothing;
    float fCorrection;
    float fPrediction;
    float fJitterRadius;
    float fMaxDeviationRadius;
} NUI_TRANSFORM_SMOOTH_PARAMETERS;

#define NUI_SKELETON_COUNT 6

#define NUI_SKELETON_MAX_TRACKED_COUNT 2

#define NUI_SKELETON_INVALID_TRACKING_ID 0

typedef enum _NUI_SKELETON_POSITION_TRACKING_STATE
{
    NUI_SKELETON_POSITION_NOT_TRACKED = 0,
    NUI_SKELETON_POSITION_INFERRED,
    NUI_SKELETON_POSITION_TRACKED
} NUI_SKELETON_POSITION_TRACKING_STATE;

typedef enum _NUI_SKELETON_TRACKING_STATE
{
    NUI_SKELETON_NOT_TRACKED = 0,
    NUI_SKELETON_POSITION_ONLY,
    NUI_SKELETON_TRACKED
} NUI_SKELETON_TRACKING_STATE;


typedef struct _KinectPointVector
{
    union {
        float vector4_f32[4];
        uint32_t vector4_u32[4];
        struct {
            float x;
            float y;
            float z;
            float w;
        };
        float v[4];
        uint32_t u[4];
    };
} KinectPointVector;

typedef struct _SkeletonData {
    NUI_SKELETON_TRACKING_STATE eTrackingState;
    uint32_t dwTrackingID;
    uint32_t dwEnrollmentIndex_NotUsed;
    uint32_t dwUserIndex;
    KinectPointVector Position;
    KinectPointVector SkeletonPositions[NUI_SKELETON_POSITION_COUNT];
    NUI_SKELETON_POSITION_TRACKING_STATE eSkeletonPositionTrackingState[NUI_SKELETON_POSITION_COUNT];
    uint32_t dwQualityFlags;
} SkeletonDataType;

struct SkeletonFrame
{	
	int64_t liTimeStamp;
	uint64_t dwFrameNumber;
	uint64_t dwFlags;
	KinectPointVector vFloorClipPlane;
	KinectPointVector vNormalToGravity;
    SkeletonDataType SkeletonData[NUI_SKELETON_COUNT];
};

class VideoFrame
{
public:
	VideoFrame(const unsigned char* buffer, uint32_t bufsize);
	VideoFrame(uint32_t width, uint32_t height, uint32_t nchannels, const unsigned char* data);
	~VideoFrame();
	unsigned char* GetRawData(){return m_rawData;}
	uint32_t GetHeight(){return m_height;}
	uint32_t GetWidth(){return m_width;}
	uint32_t GetChannelNum(){return m_nchannels;}
	uint32_t GetSize();
	uint32_t GetFrameSize();
	uint32_t GetHeaderSize(){return sizeof(m_header);}
	unsigned const char* GetFrame(){return m_data;}
	bool IsOK();
protected:
	union{
		struct{
			uint32_t m_height;
			uint32_t m_width;
			uint32_t m_nchannels;
		};
		unsigned char m_header[3 * sizeof(uint32_t)];
	};
	unsigned char* m_rawData;
	unsigned char* m_data;
};
};