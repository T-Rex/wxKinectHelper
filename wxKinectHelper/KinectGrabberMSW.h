#pragma once

#include "KinectGrabberBase.h"
#include "NuiApi.h"

class KinectGrabberMSW : public KinectGrabberBase, public wxThreadHelper
{
public:
	KinectGrabberMSW(wxEvtHandler * handler, size_t deviceIndex);
	~KinectGrabberMSW();

	virtual bool GrabDepthFrame(unsigned char * data);
	virtual bool GrabColorFrame(unsigned char * data);
	virtual kinect_data::SkeletonFrame * GrabSkeletonFrame();

	virtual bool Start();
	virtual bool Stop();
	virtual bool IsStarted();
private:
	virtual wxThread::ExitCode Entry();

	BYTE * CreateDepthDataBuffer();
	BYTE * CreateColorDataBuffer();
	size_t GetDepthDataBufferLength();
	size_t GetColorDataBufferLength();
	void FreeDataBuffer(BYTE * data);

	bool ReadDepthFrame();
	bool ReadColorFrame();
	bool ReadSkeletonFrame();

	void ReadDepthLockedRect(NUI_LOCKED_RECT & LockedRect, int w, int h, BYTE * data);
	void ReadColorLockedRect(NUI_LOCKED_RECT & LockedRect, int w, int h, BYTE * data);

	static RGBQUAD Nui_ShortToQuad_Depth( USHORT s );

	void ResetEvents();

	void StopThread();

	bool CopyLocalBuffer(BYTE * src, BYTE * dst, size_t count);

	HANDLE m_NewDepthFrameEvent;
	HANDLE m_NewColorFrameEvent;
	HANDLE m_NewSkeletonFrameEvent;

	HANDLE m_DepthStreamHandle;
	HANDLE m_ColorStreamHandle;

	BYTE * m_DepthBuffer;
	BYTE * m_ColorBuffer;
	INuiSensor * m_Instance;
	size_t m_DeviceIndex;
	kinect_data::SkeletonFrame m_SkeletonFrame;
	void ConvertFrame(NUI_SKELETON_FRAME& skeletonFrame); 
};