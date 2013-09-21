#include "stdwx.h"
#include <NuiApi.h>
#include "KinectHelperImplMSW.h"
#include "KinectGrabberMSW.h"

KinectHelperImplMSW::~KinectHelperImplMSW()
{
}

size_t KinectHelperImplMSW::GetDeviceCount()
{
	int result(0);
	if(FAILED(NuiGetSensorCount(&result))) return 0;
	return (size_t)result;
}

wxString KinectHelperImplMSW::GetDeviceName(size_t index)
{
	BSTR result;
	DWORD size;
	INuiSensor * instance(NULL);
	wxString name = wxT("Unknown Kinect Sensor");
	if(!FAILED(NuiCreateSensorByIndex(index, &instance)))
	{
		if(instance != NULL)
		{
			
			//if(instance->NuiGetPropsBlob(
			//	MsrNui::INDEX_UNIQUE_DEVICE_NAME, 
			//	&result, &size))
			//{
				//name = result;
				//SysFreeString(result);
			//}
			//NuiDestroyInstance(instance);
			name = wxString::Format(wxT("Kinect#%u"), index); 
		}
	}
	return name;
}

KinectGrabberBase * KinectHelperImplMSW::CreateGrabber(
	wxEvtHandler * handler, size_t index)
{
	return new KinectGrabberMSW(handler, index);
}

