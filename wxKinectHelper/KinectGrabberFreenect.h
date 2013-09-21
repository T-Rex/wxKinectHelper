#pragma once
#include "KinectTypes.h"
#include "KinectGrabberBase.h"

class KinectGrabberFreenect : public KinectGrabberBase, public wxThreadHelper
{
    DECLARE_DYNAMIC_CLASS(KinectGrabberFreenect)
    WX_DECLARE_VOIDPTR_HASH_MAP(KinectGrabberBase *, FreenectDeviceMap);
public:
    KinectGrabberFreenect();
	KinectGrabberFreenect(wxEvtHandler * handler,
                       freenect_context * context,
                       size_t deviceIndex);
	virtual ~KinectGrabberFreenect();

	virtual bool GrabDepthFrame(unsigned char * data);
	virtual bool GrabColorFrame(unsigned char * data);
	virtual kinect_data::SkeletonFrame * GrabSkeletonFrame();

	virtual bool Start();
	virtual bool Stop();
	virtual bool IsStarted();
private:
    freenect_context * m_Context;
    freenect_device * m_Device;
    freenect_frame_mode m_DepthMode;
    freenect_frame_mode m_VideoMode;
    size_t m_DeviceIndex;
    unsigned char * m_DepthBuffers[2];
    unsigned char * m_VideoBuffer;
    int m_VideoBufferLength;
    int m_DepthBufferLength;
    int m_DepthBufferIndex;
    bool m_GotDepth;

    wxThread::ExitCode Entry();
    void StopThread();

    wxMutex m_Mutex;

	static FreenectDeviceMap DeviceHash;
	static void DepthCallback(freenect_device * device, void * data, uint32_t timestamp);
	static void VideoCallback(freenect_device * device, void * data, uint32_t timestamp);
	static unsigned char GetDepthValue(unsigned short data);
};

