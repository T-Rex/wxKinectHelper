#include "KinectGrabberMSW.h"

KinectGrabberMSW::KinectGrabberMSW(wxEvtHandler * handler, size_t deviceIndex)
	: KinectGrabberBase(handler)
	, m_DeviceIndex(deviceIndex)
	, m_Instance(NULL)
{
	m_DepthBuffer = CreateDepthDataBuffer();
	m_ColorBuffer = CreateColorDataBuffer();
	ResetEvents();
	do
	{
		if(FAILED(NuiCreateSensorByIndex(
			(int)m_DeviceIndex, &m_Instance))) break;
		if(FAILED(m_Instance->NuiInitialize(
			NUI_INITIALIZE_FLAG_USES_DEPTH_AND_PLAYER_INDEX | 
			NUI_INITIALIZE_FLAG_USES_COLOR |
			NUI_INITIALIZE_FLAG_USES_SKELETON))) break;
	}
	while(false);
}

KinectGrabberMSW::~KinectGrabberMSW()
{
	StopThread();
	FreeDataBuffer(m_DepthBuffer);
	FreeDataBuffer(m_ColorBuffer);
	if(m_Instance)
	{
		m_Instance->NuiShutdown();
		m_Instance->Release();
		m_Instance = NULL;
	}
}

void KinectGrabberMSW::ResetEvents()
{
	m_NewDepthFrameEvent = NULL;
	m_NewColorFrameEvent = NULL;
	m_NewSkeletonFrameEvent = NULL;
}

bool KinectGrabberMSW::CopyLocalBuffer(BYTE * src, BYTE * dst, size_t count)
{
	do
	{
		if(!src) break;
		memcpy(dst, src, count);
		return true;
	}
	while(false);
	return false;
}

bool KinectGrabberMSW::GrabDepthFrame(unsigned char * data)
{
	return CopyLocalBuffer(m_DepthBuffer, data, GetDepthDataBufferLength());
}

bool KinectGrabberMSW::GrabColorFrame(unsigned char * data)
{
	return CopyLocalBuffer(m_ColorBuffer, data, GetColorDataBufferLength());
}

kinect_data::SkeletonFrame * KinectGrabberMSW::GrabSkeletonFrame()
{
	do
	{
		if(!GetThread() || !GetThread()->IsAlive() || 
			!m_Instance || !m_NewSkeletonFrameEvent) break;
		return &m_SkeletonFrame;
	}
	while(false);
	return NULL;
}

bool KinectGrabberMSW::Start()
{
	do
	{
		if(!m_Instance) break;
		if(GetThread() && GetThread()->IsAlive()) break;

		if(CreateThread() != wxTHREAD_NO_ERROR) break;

		m_NewDepthFrameEvent = CreateEvent( NULL, TRUE, FALSE, NULL );
		m_NewColorFrameEvent = CreateEvent( NULL, TRUE, FALSE, NULL );
		m_NewSkeletonFrameEvent = CreateEvent( NULL, TRUE, FALSE, NULL );

		if(FAILED(m_Instance->NuiImageStreamOpen(
				NUI_IMAGE_TYPE_DEPTH_AND_PLAYER_INDEX, 
				NUI_IMAGE_RESOLUTION_320x240, 0, 
				//NUI_IMAGE_RESOLUTION_80x60, 0, 
				3, 
				m_NewDepthFrameEvent,
				&m_DepthStreamHandle))) break;
		if(FAILED(m_Instance->NuiImageStreamOpen(
				NUI_IMAGE_TYPE_COLOR, 
				NUI_IMAGE_RESOLUTION_640x480, 0, 
				4, 
				m_NewColorFrameEvent,
				&m_ColorStreamHandle)))  break;
		if(FAILED(m_Instance->NuiSkeletonTrackingEnable(
				m_NewSkeletonFrameEvent, 0))) break;

		GetThread()->Run();

		return true;
	}
	while(false);
	return false;
}

bool KinectGrabberMSW::Stop()
{
	do
	{
		StopThread();
		memset(m_DepthBuffer, 0, GetDepthDataBufferLength());
		memset(m_ColorBuffer, 0, GetColorDataBufferLength());
		CloseHandle(m_NewDepthFrameEvent);
		CloseHandle(m_NewColorFrameEvent);
		CloseHandle(m_NewSkeletonFrameEvent);
		ResetEvents();
		return true;
	}
	while(false);
	return false;
}

bool KinectGrabberMSW::IsStarted()
{
	return GetThread() != NULL && GetThread()->IsAlive();
}

wxThread::ExitCode KinectGrabberMSW::Entry()
{
	HANDLE eventHandles[3];
	eventHandles[0] = m_NewDepthFrameEvent;
	eventHandles[1] = m_NewColorFrameEvent;
	eventHandles[2] = m_NewSkeletonFrameEvent;
	while(!GetThread()->TestDestroy())
	{
		int mEventIndex = WaitForMultipleObjects(
			_countof(eventHandles), 
			eventHandles, FALSE, 100);
		switch(mEventIndex)
		{
		case 0: ReadDepthFrame(); break;
		case 1: ReadColorFrame(); break;
		case 2: ReadSkeletonFrame(); break;
		default: break;
		}
	}
	return NULL;
}

BYTE * KinectGrabberMSW::CreateDepthDataBuffer()
{
	size_t length = GetDepthDataBufferLength();
	BYTE * result = (BYTE*)CoTaskMemAlloc(length);
	memset(result, 0, length);
	return result;
}

BYTE * KinectGrabberMSW::CreateColorDataBuffer()
{
	size_t length = GetColorDataBufferLength();
	BYTE * result = (BYTE*)CoTaskMemAlloc(length);
	memset(result, 0, length);
	return result;
}

size_t KinectGrabberMSW::GetDepthDataBufferLength()
{
	return m_DepthFrameSize.GetWidth() * 
		m_DepthFrameSize.GetHeight() * 3;
}

size_t KinectGrabberMSW::GetColorDataBufferLength()
{
	return m_ColorFrameSize.GetWidth() * 
		m_ColorFrameSize.GetHeight() * 3;
}

void KinectGrabberMSW::FreeDataBuffer(BYTE * data)
{
	CoTaskMemFree((LPVOID)data);
}

void KinectGrabberMSW::StopThread()
{
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

bool KinectGrabberMSW::ReadDepthFrame()
{
	do
	{
		if(m_DeviceIndex < 0 || !m_Instance) break;
		const NUI_IMAGE_FRAME * pImageFrame;
		if(FAILED(NuiImageStreamGetNextFrame(
				m_DepthStreamHandle, 200, 
				&pImageFrame))) break;
		INuiFrameTexture * pTexture = pImageFrame->pFrameTexture;
		NUI_LOCKED_RECT LockedRect;
		pTexture->LockRect( 0, &LockedRect, NULL, 0 );
		ReadDepthLockedRect(LockedRect, 
			m_DepthFrameSize.GetWidth(), 
			m_DepthFrameSize.GetHeight(), 
			m_DepthBuffer);
		NuiImageStreamReleaseFrame(m_DepthStreamHandle, pImageFrame);
		if(m_Handler)
		{
			wxCommandEvent e(KINECT_DEPTH_FRAME_RECEIVED, wxID_ANY);
			e.SetInt(m_DeviceIndex);
			m_Handler->AddPendingEvent(e);
		}
		return true;
	}
	while(false);
	return false;
}

bool KinectGrabberMSW::ReadColorFrame()
{
	do
	{
		if(m_DeviceIndex < 0 || !m_Instance) break;
		const NUI_IMAGE_FRAME * pImageFrame;
		if(FAILED(NuiImageStreamGetNextFrame(
				m_ColorStreamHandle, 200, &pImageFrame))) break;
		INuiFrameTexture * pTexture = pImageFrame->pFrameTexture;
		NUI_LOCKED_RECT LockedRect;
		pTexture->LockRect( 0, &LockedRect, NULL, 0 );
		ReadColorLockedRect(LockedRect, 
			m_ColorFrameSize.GetWidth(), 
			m_ColorFrameSize.GetHeight(), 
			m_ColorBuffer);
		NuiImageStreamReleaseFrame(m_ColorStreamHandle, pImageFrame);
		if(m_Handler)
		{
			wxCommandEvent e(KINECT_COLOR_FRAME_RECEIVED, wxID_ANY);
			e.SetInt(m_DeviceIndex);
			m_Handler->AddPendingEvent(e);
		}
		return true;
	}
	while(false);
	return false;
}

bool KinectGrabberMSW::ReadSkeletonFrame()
{
	do
	{
		if(m_DeviceIndex < 0 || !m_Instance) break;
		NUI_SKELETON_FRAME skeletonFrame;
		if(FAILED(m_Instance->NuiSkeletonGetNextFrame(
				200, &skeletonFrame))) break;
	    bool bFoundSkeleton = false;
        for ( int i = 0 ; i < NUI_SKELETON_COUNT ; i++ )
        {
            if( skeletonFrame.SkeletonData[i].eTrackingState == NUI_SKELETON_TRACKED ||
                (skeletonFrame.SkeletonData[i].eTrackingState == NUI_SKELETON_POSITION_ONLY))
            {
                bFoundSkeleton = true;
            }
        }
		// no skeletons!
		if( !bFoundSkeleton )
		{
			break;
		}

		// smooth out the skeleton data
		if ( FAILED(m_Instance->NuiTransformSmooth(&skeletonFrame,NULL)))
		{
			break;
		}

		ConvertFrame(skeletonFrame);
		if(m_Handler)
		{
			wxCommandEvent e(KINECT_SKELETON_FRAME_RECEIVED, wxID_ANY);
			e.SetInt(m_DeviceIndex);
			m_Handler->AddPendingEvent(e);
		}
		return true;
	}
	while(false);
	return false;
}

void KinectGrabberMSW::ConvertFrame(NUI_SKELETON_FRAME& skeletonFrame)
{
	m_SkeletonFrame.dwFlags = skeletonFrame.dwFlags;
	m_SkeletonFrame.dwFrameNumber = skeletonFrame.dwFrameNumber;
	m_SkeletonFrame.liTimeStamp = skeletonFrame.liTimeStamp.QuadPart;
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
		kinect_data::SkeletonDataType& sd(m_SkeletonFrame.SkeletonData[i]);
		NUI_SKELETON_DATA sd_origin(skeletonFrame.SkeletonData[i]);
		sd.dwEnrollmentIndex_NotUsed = sd_origin.dwEnrollmentIndex;
		sd.dwQualityFlags = sd_origin.dwQualityFlags;
		sd.dwTrackingID = sd_origin.dwTrackingID;
		sd.dwUserIndex = sd_origin.dwUserIndex;
		sd.eTrackingState = (kinect_data::NUI_SKELETON_TRACKING_STATE)sd_origin.eTrackingState;
		for(int j = 0; j < NUI_SKELETON_POSITION_COUNT; ++j)
		{
			sd.eSkeletonPositionTrackingState[j] = 
				(kinect_data::NUI_SKELETON_POSITION_TRACKING_STATE)sd_origin.eSkeletonPositionTrackingState[j];
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
void KinectGrabberMSW::ReadDepthLockedRect(NUI_LOCKED_RECT & LockedRect, 
	int w, int h, BYTE * data)
{
	if( LockedRect.Pitch != 0 )
	{
		BYTE * pBuffer = (BYTE*) LockedRect.pBits;
		USHORT * pBufferRun = (USHORT*) pBuffer;
		for( int y = 0 ; y < h ; y++ )
		{
			for( int x = 0 ; x < w ; x++ )
			{
				int offset = (w * y + x) * 3;
				data[offset + 0] = *pBufferRun >> 3 & 0xff;
				data[offset + 1] = *pBufferRun >> 11 & 0xf;
				data[offset + 2] = 0;
				pBufferRun++;
			}
		}
	}
}

void KinectGrabberMSW::ReadColorLockedRect(NUI_LOCKED_RECT & LockedRect, 
	int w, int h, BYTE * data)
{
	if( LockedRect.Pitch != 0 )
	{
		BYTE * pBuffer = (BYTE*) LockedRect.pBits;
		for( int y = 0 ; y < h ; y++ )
		{
			for( int x = 0 ; x < w ; x++ )
			{
				RGBQUAD * quad = ((RGBQUAD*)pBuffer) + x;
				int offset = (w * y + x) * 3;
				data[offset + 0] = quad->rgbRed;
				data[offset + 1] = quad->rgbGreen;
				data[offset + 2] = quad->rgbBlue;
			}
			pBuffer += LockedRect.Pitch;
		}
	}
}

RGBQUAD KinectGrabberMSW::Nui_ShortToQuad_Depth( USHORT s )
{
	USHORT RealDepth = (s & 0xfff8) >> 3;
	BYTE l = 255 - (BYTE)(256*RealDepth/0x0fff);
	l = RealDepth == 0 ? 0 : l;
	RGBQUAD q;
	q.rgbRed = q.rgbBlue = q.rgbGreen = l;
	return q;
}
