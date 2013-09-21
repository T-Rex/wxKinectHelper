#pragma once
#include "KinectTypes.h"
#include "SkeletonPainterImpl.h"
#include "nuiapi.h"

class SkeletonPainterImplMSW : public SkeletonPainterImpl
{
public:
	~SkeletonPainterImplMSW();
	void DrawSkeleton(wxDC & dc, void * data);
private:
	void ConvertFrame(kinect_data::SkeletonFrame& skeletonFrame);
	void Nui_DrawSkeleton(wxDC & dc, NUI_SKELETON_DATA * data, size_t index);
	void Nui_DrawSkeletonSegment(wxDC & dc, wxPoint * points, int numJoints, ... );
	NUI_SKELETON_FRAME m_SkeletonFrame;
	static wxPen m_SkeletonPen[6];
};