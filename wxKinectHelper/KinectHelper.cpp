#include <wx/wx.h>
#include "KinectHelper.h"

#if defined(__WXMSW__)
#include "KinectHelperImplMSW.h"
#elif defined(__WXGTK__)
#include "KinectHelperImplFreenect.h"
#else
#include "KinectHelperImpl.h"
#endif

KinectHelper::KinectHelper()
{
#if defined(__WXMSW__)
	m_Impl = new KinectHelperImplMSW;
#elif defined(__WXGTK__)
	m_Impl = new KinectHelperImplFreenect;
#else
	m_Impl = NULL;
#endif
}

KinectHelper::~KinectHelper()
{
	wxDELETE(m_Impl);
}

size_t KinectHelper::GetDeviceCount()
{
	if(m_Impl)
	{
		return m_Impl->GetDeviceCount();
	}
	return 0;
}

wxString KinectHelper::GetDeviceName(size_t index)
{
	if(m_Impl)
	{
		return m_Impl->GetDeviceName(index);
	}
	return wxEmptyString;
}

KinectGrabberBase * KinectHelper::CreateGrabber(wxEvtHandler * handler, size_t index)
{
	if(m_Impl)
	{
		return m_Impl->CreateGrabber(handler, index);
	}
	return NULL;
}

