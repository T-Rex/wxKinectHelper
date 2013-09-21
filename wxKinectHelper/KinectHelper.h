#pragma once

class KinectGrabberBase;
class KinectHelperImpl;

class KinectHelper
{
public:
	KinectHelper();
	~KinectHelper();

	size_t GetDeviceCount();
	wxString GetDeviceName(size_t index);
	KinectGrabberBase * CreateGrabber(wxEvtHandler * handler, size_t index);
protected:
	KinectHelperImpl * m_Impl;
};
