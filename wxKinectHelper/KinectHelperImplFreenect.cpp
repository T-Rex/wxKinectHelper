#include "stdwx.h"
#include "KinectHelperImplFreenect.h"
#include "KinectGrabberFreenect.h"

KinectHelperImplFreenect::KinectHelperImplFreenect()
: m_Context(NULL)
{
	if(freenect_init(&m_Context, NULL) < 0)
	{
		m_Context = NULL;
	}
	else
	{
	    freenect_set_log_level(m_Context, FREENECT_LOG_WARNING);
	}
}

KinectHelperImplFreenect::~KinectHelperImplFreenect()
{
	freenect_shutdown(m_Context);
}

size_t KinectHelperImplFreenect::GetDeviceCount()
{
	if(m_Context)
	{
		return (size_t)freenect_num_devices(m_Context);
	}
	return 0;
}

wxString KinectHelperImplFreenect::GetDeviceName(size_t index)
{
	wxString name = wxT("Unknown Kinect Sensor");
	if(m_Context)
	{
		name = wxString::Format(wxT("Kinect %u"), index);
	}
	return name;
}

KinectGrabberBase * KinectHelperImplFreenect::CreateGrabber(
	wxEvtHandler * handler, size_t index)
{
	return new KinectGrabberFreenect(handler, m_Context, index);
}

