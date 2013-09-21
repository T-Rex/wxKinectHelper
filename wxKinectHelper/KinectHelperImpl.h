#pragma once

class KinectGrabberBase;

class KinectHelperImpl
{
public:
	virtual ~KinectHelperImpl();

	virtual size_t GetDeviceCount() = 0;
	virtual wxString GetDeviceName(size_t index) = 0;
	virtual KinectGrabberBase * CreateGrabber(wxEvtHandler * handler, size_t index) = 0;
};
