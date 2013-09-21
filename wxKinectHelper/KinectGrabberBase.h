#pragma once
#include "KinectTypes.h"
#include <wx/wx.h>

class KinectGrabberBase : public wxObject
{
	DECLARE_ABSTRACT_CLASS(KinectGrabberBase)
public:
	KinectGrabberBase();
	KinectGrabberBase(wxEvtHandler * handler);
	virtual ~KinectGrabberBase();

	virtual bool GrabDepthFrame(unsigned char * data) = 0;
	virtual bool GrabColorFrame(unsigned char * data) = 0;
	virtual kinect_data::SkeletonFrame * GrabSkeletonFrame() = 0;

	virtual bool Start() = 0;
	virtual bool Stop() = 0;
	virtual bool IsStarted() = 0;

	const wxSize & GetDepthFrameSize();
	const wxSize & GetColorFrameSize();
protected:
	wxSize m_DepthFrameSize;
	wxSize m_ColorFrameSize;
	wxEvtHandler * m_Handler;
};

BEGIN_DECLARE_EVENT_TYPES()

DECLARE_LOCAL_EVENT_TYPE(KINECT_DEPTH_FRAME_RECEIVED, -1)
DECLARE_LOCAL_EVENT_TYPE(KINECT_COLOR_FRAME_RECEIVED, -1)
DECLARE_LOCAL_EVENT_TYPE(KINECT_SKELETON_FRAME_RECEIVED, -1)

END_DECLARE_EVENT_TYPES()
