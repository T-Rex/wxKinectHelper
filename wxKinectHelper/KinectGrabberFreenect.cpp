#include "stdwx.h"
#include <libfreenect.h>
#include "KinectGrabberFreenect.h"

IMPLEMENT_DYNAMIC_CLASS(KinectGrabberFreenect, KinectGrabberBase)

KinectGrabberFreenect::FreenectDeviceMap KinectGrabberFreenect::DeviceHash;

KinectGrabberFreenect::KinectGrabberFreenect()
: m_Context(NULL)
, m_Device(NULL)
, m_DeviceIndex(0)
, m_VideoBuffer(NULL)
, m_DepthBufferLength(0)
, m_VideoBufferLength(0)
{
}

KinectGrabberFreenect::KinectGrabberFreenect(
    wxEvtHandler * handler,
    freenect_context * context,
    size_t deviceIndex)
: KinectGrabberBase(handler)
, m_Context(context)
, m_Device(NULL)
, m_DeviceIndex(deviceIndex)
{
    m_VideoMode = freenect_find_video_mode(
        FREENECT_RESOLUTION_MEDIUM,
        FREENECT_VIDEO_RGB);
   m_DepthMode = freenect_find_depth_mode(
        FREENECT_RESOLUTION_MEDIUM,
        FREENECT_DEPTH_11BIT);
    m_DepthFrameSize.Set(m_DepthMode.width, m_DepthMode.height);
}

KinectGrabberFreenect::~KinectGrabberFreenect()
{
    StopThread();
}

bool KinectGrabberFreenect::GrabDepthFrame(unsigned char * data)
{
	do
	{
	    wxMutexLocker lock(m_Mutex);
	    if(!m_Device || !m_GotDepth || !GetThread() || !GetThread()->IsAlive()) break;
	    int bufferIndex = m_DepthBufferIndex;
	    m_DepthBufferIndex = 1 - m_DepthBufferIndex;
	    memcpy(data, m_DepthBuffers[bufferIndex], m_DepthBufferLength);
	    m_GotDepth = false;
		return true;
	}
	while(false);
	return false;
}

bool KinectGrabberFreenect::GrabColorFrame(unsigned char * data)
{
	do
	{
	    if(!m_Device) break;
	    memcpy(data, m_VideoBuffer, m_VideoBufferLength);
		return true;
	}
	while(false);
	return false;
}

kinect_data::SkeletonFrame * KinectGrabberFreenect::GrabSkeletonFrame()
{
	return NULL;
}

void KinectGrabberFreenect::DepthCallback(freenect_device * device, void * data, uint32_t timestamp)
{
    do
    {
        KinectGrabberFreenect * grabber = wxDynamicCast(DeviceHash[device], KinectGrabberFreenect);
        if(!grabber) break;
        wxMutexLocker lock(grabber->m_Mutex);
        unsigned short * depth = (unsigned short *)data;
        unsigned char * buffer = grabber->m_DepthBuffers[grabber->m_DepthBufferIndex];
        int w = grabber->m_DepthFrameSize.GetWidth();
        int h = grabber->m_DepthFrameSize.GetHeight();
        for( int y = 0 ; y < h ; y++ )
		{
			for( int x = 0 ; x < w ; x++ )
			{
			    int offset = (w * y + x) * 3;
			    unsigned char depthValue = GetDepthValue(*depth);
			    buffer[offset + 0] = depthValue;
			    buffer[offset + 1] = depthValue;
			    buffer[offset + 2] = depthValue;
			    depth++;
			}
		}
		grabber->m_GotDepth = true;
        if(grabber->m_Handler)
		{
			wxCommandEvent e(KINECT_DEPTH_FRAME_RECEIVED, wxID_ANY);
			e.SetInt(grabber->m_DeviceIndex);
			grabber->m_Handler->AddPendingEvent(e);
		}
    }
    while(false);
}

unsigned char KinectGrabberFreenect::GetDepthValue(unsigned short data)
{
    // 0x7ff is 0000011111111111 in binary format - max value for 11bit depth;
    return (double)255 * (double)data / (double)0x7ff;
}

void KinectGrabberFreenect::VideoCallback(freenect_device * device, void * data, uint32_t timestamp)
{
    do
    {
        KinectGrabberFreenect * grabber = wxDynamicCast(DeviceHash[device], KinectGrabberFreenect);
        if(!grabber) break;
        if(grabber->m_Handler)
		{
			wxCommandEvent e(KINECT_COLOR_FRAME_RECEIVED, wxID_ANY);
			e.SetInt(grabber->m_DeviceIndex);
			grabber->m_Handler->AddPendingEvent(e);
		}
    }
    while(false);
}

bool KinectGrabberFreenect::Start()
{
	do
	{
	    if(!m_Context) break;
#if wxVERSION_NUMBER >= 2900
	    if(CreateThread() != wxTHREAD_NO_ERROR) break;
#else
        if(Create() != wxTHREAD_NO_ERROR) break;
#endif
	    if(freenect_open_device(m_Context, &m_Device,
                             (int)m_DeviceIndex) < 0) break;

        freenect_set_depth_callback(m_Device, KinectGrabberFreenect::DepthCallback);
        freenect_set_video_callback(m_Device, KinectGrabberFreenect::VideoCallback);
        freenect_set_video_mode(m_Device, m_VideoMode);
        if(freenect_set_depth_mode(m_Device, m_DepthMode) < 0)
        {
            printf("DEPTH MODE ERROR\n");
        }
        m_VideoBufferLength = m_VideoMode.bytes;
        m_DepthBufferLength = m_DepthFrameSize.GetWidth() * m_DepthFrameSize.GetHeight() * 3;
        m_VideoBuffer = new unsigned char[m_VideoBufferLength];
        m_DepthBufferIndex = 0;
        m_GotDepth = false;
        m_DepthBuffers[0] = new unsigned char[
            m_DepthFrameSize.GetWidth() *
            m_DepthFrameSize.GetHeight() * 3];
        m_DepthBuffers[1] = new unsigned char[
            m_DepthFrameSize.GetWidth() *
            m_DepthFrameSize.GetHeight() * 3];
        freenect_set_video_buffer(m_Device, m_VideoBuffer);
        DeviceHash[m_Device] = this;
        freenect_start_video(m_Device);
        if(freenect_start_depth(m_Device) < 0)
        {
            printf("START DEPTH ERROR\n");
        }
        GetThread()->Run();
		return true;
	}
	while(false);
	return false;
}

bool KinectGrabberFreenect::Stop()
{
	do
	{
	    StopThread();
	    DeviceHash.erase(m_Device);
	    freenect_stop_depth(m_Device);
        freenect_stop_video(m_Device);
        freenect_close_device(m_Device);
	    wxDELETEA(m_DepthBuffers[0]);
	    wxDELETEA(m_DepthBuffers[1]);
	    wxDELETEA(m_VideoBuffer);
	    m_VideoBufferLength = m_DepthBufferLength = 0;
	    m_Device = NULL;
		return true;
	}
	while(false);
	return false;
}

void KinectGrabberFreenect::StopThread()
{
#if wxVERSION_NUMBER < 2900
    wxThreadKind m_kind = wxTHREAD_DETACHED;
    if(GetThread() && !GetThread()->IsDetached())
    {
        m_kind = wxTHREAD_JOINABLE;
    }
#endif
	if(GetThread())
	{
		if(GetThread()->IsAlive())
		{
			GetThread()->Delete();
		}
		if(m_kind == wxTHREAD_JOINABLE)
		{
			if(GetThread()->IsAlive())
			{
				GetThread()->Wait();
			}
			wxDELETE(m_thread);
		}
		else
		{
			m_thread = NULL;
		}
	}
	wxYield();
}

wxThread::ExitCode KinectGrabberFreenect::Entry()
{
    int status(0);
    while(!GetThread()->TestDestroy() && status >= 0)
    {
        status = freenect_process_events(m_Context);
    }
    return NULL;
}

bool KinectGrabberFreenect::IsStarted()
{
	return m_Device != NULL && GetThread() && GetThread()->IsAlive();
}

