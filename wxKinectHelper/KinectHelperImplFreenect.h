#pragma once

#include "KinectHelperImpl.h"
#include <libfreenect.h>

class KinectHelperImplFreenect : public KinectHelperImpl
{
public:
	KinectHelperImplFreenect();
	~KinectHelperImplFreenect();

	size_t GetDeviceCount();
	wxString GetDeviceName(size_t index);
	KinectGrabberBase * CreateGrabber(wxEvtHandler * handler, size_t index);
private:
	freenect_context * m_Context;
};
