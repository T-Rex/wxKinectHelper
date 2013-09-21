#include "SkeletonPainterImplMSW.h"

wxPen SkeletonPainterImplMSW::m_SkeletonPen[6] = 
{
	wxPen(wxColor(255, 0, 0), wxSOLID),
	wxPen(wxColor(255, 0, 0), wxSOLID),
	wxPen(wxColor(255, 0, 0), wxSOLID),
	wxPen(wxColor(255, 0, 0), wxSOLID),
	wxPen(wxColor(255, 0, 0), wxSOLID),
	wxPen(wxColor(255, 0, 0), wxSOLID)
};

SkeletonPainterImplMSW::~SkeletonPainterImplMSW()
{
}

void SkeletonPainterImplMSW::ConvertFrame(kinect_data::SkeletonFrame& skeletonFrame)
{
	m_SkeletonFrame.dwFlags = skeletonFrame.dwFlags;
	m_SkeletonFrame.dwFrameNumber = skeletonFrame.dwFrameNumber;
	m_SkeletonFrame.liTimeStamp.QuadPart = skeletonFrame.liTimeStamp;
	m_SkeletonFrame.vFloorClipPlane.x = skeletonFrame.vFloorClipPlane.x;
	m_SkeletonFrame.vNormalToGravity.x = skeletonFrame.vNormalToGravity.x;
	m_SkeletonFrame.vFloorClipPlane.y = skeletonFrame.vFloorClipPlane.y;
	m_SkeletonFrame.vNormalToGravity.y = skeletonFrame.vNormalToGravity.y;
	m_SkeletonFrame.vFloorClipPlane.z = skeletonFrame.vFloorClipPlane.z;
	m_SkeletonFrame.vNormalToGravity.z = skeletonFrame.vNormalToGravity.z;
	m_SkeletonFrame.vFloorClipPlane.w = skeletonFrame.vFloorClipPlane.w;
	m_SkeletonFrame.vNormalToGravity.w = skeletonFrame.vNormalToGravity.w;
	for(int i = 0; i < NUI_SKELETON_COUNT; ++i)
	{
		NUI_SKELETON_DATA& sd(m_SkeletonFrame.SkeletonData[i]);
		kinect_data::SkeletonDataType& sd_origin(skeletonFrame.SkeletonData[i]);
		sd.dwEnrollmentIndex = sd_origin.dwEnrollmentIndex_NotUsed;
		sd.dwQualityFlags = sd_origin.dwQualityFlags;
		sd.dwTrackingID = sd_origin.dwTrackingID;
		sd.dwUserIndex = sd_origin.dwUserIndex;
		sd.eTrackingState = (NUI_SKELETON_TRACKING_STATE)sd_origin.eTrackingState;
		for(int j = 0; j < NUI_SKELETON_POSITION_COUNT; ++j)
		{
			sd.eSkeletonPositionTrackingState[j] = 
				(NUI_SKELETON_POSITION_TRACKING_STATE)sd_origin.eSkeletonPositionTrackingState[j];
		}
		sd.Position.x = sd_origin.Position.x;
		sd.Position.y = sd_origin.Position.y;
		sd.Position.z = sd_origin.Position.z;
		sd.Position.w = sd_origin.Position.w;
		for(int k = 0; k < NUI_SKELETON_POSITION_COUNT; ++k){
			sd.SkeletonPositions[k].x = sd_origin.SkeletonPositions[k].x;
			sd.SkeletonPositions[k].y = sd_origin.SkeletonPositions[k].y;
			sd.SkeletonPositions[k].z = sd_origin.SkeletonPositions[k].z;
			sd.SkeletonPositions[k].w = sd_origin.SkeletonPositions[k].w;
		}
	}

}

void SkeletonPainterImplMSW::DrawSkeleton(wxDC & dc, void * data)
{
	do
	{
		kinect_data::SkeletonFrame * frame = 
			reinterpret_cast<kinect_data::SkeletonFrame*>(data);
		if(!frame) break;
		ConvertFrame(*frame);
		for(size_t i = 0 ; i < NUI_SKELETON_COUNT ; i++ )
		{
			if(m_SkeletonFrame.SkeletonData[i].eTrackingState == NUI_SKELETON_TRACKED ||
				m_SkeletonFrame.SkeletonData[i].eTrackingState == NUI_SKELETON_POSITION_ONLY)
			{
				Nui_DrawSkeleton(dc, &m_SkeletonFrame.SkeletonData[i], i);
			}
		}
	}
	while(false);
}

void SkeletonPainterImplMSW::Nui_DrawSkeleton(wxDC & dc, 
		NUI_SKELETON_DATA * data, size_t index)
{
	wxPoint points[NUI_SKELETON_POSITION_COUNT];
	long fx(0), fy(0);
	wxSize imageSize = dc.GetSize();
    USHORT depth;
	for (size_t i = 0; i < NUI_SKELETON_POSITION_COUNT; i++)
	{
		NuiTransformSkeletonToDepthImage(
			data->SkeletonPositions[i], &fx, &fy, &depth);
		points[i].x = (int) ( fx * imageSize.GetWidth() / 320);
		points[i].y = (int) ( fy * imageSize.GetHeight() / 240);
	}

	Nui_DrawSkeletonSegment(dc,points,4,
		NUI_SKELETON_POSITION_HIP_CENTER, 
		NUI_SKELETON_POSITION_SPINE, 
		NUI_SKELETON_POSITION_SHOULDER_CENTER, 
		NUI_SKELETON_POSITION_HEAD);
	Nui_DrawSkeletonSegment(dc,points,5,
		NUI_SKELETON_POSITION_SHOULDER_CENTER, 
		NUI_SKELETON_POSITION_SHOULDER_LEFT, 
		NUI_SKELETON_POSITION_ELBOW_LEFT, 
		NUI_SKELETON_POSITION_WRIST_LEFT, 
		NUI_SKELETON_POSITION_HAND_LEFT);
	Nui_DrawSkeletonSegment(dc,points,5,
		NUI_SKELETON_POSITION_SHOULDER_CENTER, 
		NUI_SKELETON_POSITION_SHOULDER_RIGHT, 
		NUI_SKELETON_POSITION_ELBOW_RIGHT, 
		NUI_SKELETON_POSITION_WRIST_RIGHT, 
		NUI_SKELETON_POSITION_HAND_RIGHT);
	Nui_DrawSkeletonSegment(dc,points,5,
		NUI_SKELETON_POSITION_HIP_CENTER, 
		NUI_SKELETON_POSITION_HIP_LEFT, 
		NUI_SKELETON_POSITION_KNEE_LEFT, 
		NUI_SKELETON_POSITION_ANKLE_LEFT, 
		NUI_SKELETON_POSITION_FOOT_LEFT);
	Nui_DrawSkeletonSegment(dc,points,5,
		NUI_SKELETON_POSITION_HIP_CENTER, 
		NUI_SKELETON_POSITION_HIP_RIGHT, 
		NUI_SKELETON_POSITION_KNEE_RIGHT, 
		NUI_SKELETON_POSITION_ANKLE_RIGHT, 
		NUI_SKELETON_POSITION_FOOT_RIGHT);
}

void SkeletonPainterImplMSW::Nui_DrawSkeletonSegment(wxDC & dc, 
		wxPoint * points, int numJoints, ...)
{
	va_list vl;
	va_start(vl,numJoints);
	wxPoint segmentPositions[NUI_SKELETON_POSITION_COUNT];

	for (int iJoint = 0; iJoint < numJoints; iJoint++)
	{
		NUI_SKELETON_POSITION_INDEX jointIndex = va_arg(
			vl,NUI_SKELETON_POSITION_INDEX);
		segmentPositions[iJoint].x = points[jointIndex].x;
		segmentPositions[iJoint].y = points[jointIndex].y;
	}

	dc.SetPen(*wxBLUE_PEN);
	dc.DrawLines(numJoints, segmentPositions);

	va_end(vl);
}
