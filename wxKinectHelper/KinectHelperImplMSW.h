#pragma once

#include "KinectHelperImpl.h"

class KinectHelperImplMSW : public KinectHelperImpl
{
public:
	~KinectHelperImplMSW();

	size_t GetDeviceCount();
	wxString GetDeviceName(size_t index);
	KinectGrabberBase * CreateGrabber(wxEvtHandler * handler, size_t index);
};
