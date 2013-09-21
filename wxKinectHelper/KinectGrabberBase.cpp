#include "KinectGrabberBase.h"

DEFINE_EVENT_TYPE(KINECT_DEPTH_FRAME_RECEIVED)
DEFINE_EVENT_TYPE(KINECT_COLOR_FRAME_RECEIVED)
DEFINE_EVENT_TYPE(KINECT_SKELETON_FRAME_RECEIVED)

IMPLEMENT_ABSTRACT_CLASS(KinectGrabberBase, wxObject)

KinectGrabberBase::KinectGrabberBase()
: m_Handler(NULL)
, m_DepthFrameSize(320,240)
//, m_DepthFrameSize(80,60)
, m_ColorFrameSize(640,480)
{
}

KinectGrabberBase::KinectGrabberBase(wxEvtHandler * handler)
: m_Handler(handler)
, m_DepthFrameSize(320,240)
//, m_DepthFrameSize(80,60)
, m_ColorFrameSize(640,480)
{
}

KinectGrabberBase::~KinectGrabberBase()
{
}

const wxSize & KinectGrabberBase::GetDepthFrameSize()
{
	return m_DepthFrameSize;
}

const wxSize & KinectGrabberBase::GetColorFrameSize()
{
	return m_ColorFrameSize;
}
