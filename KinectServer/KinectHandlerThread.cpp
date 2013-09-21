#include "KinectHandlerThread.h"
//#include "HttpHeader.h"

extern "C" 
{
#include "event.h"
}

KinectHandlerThread::KinectHandlerThread()
{

}

void KinectHandlerThread::Execute()
{
	_evbase = event_base_new();
	_evhttp = evhttp_new(_evbase);  
	evhttp_bind_socket(_evhttp, "0.0.0.0", 9500);
	evhttp_set_gencb(_evhttp, _OnRead, this);  
	event_base_dispatch(_evbase);
}

void KinectHandlerThread::_OnRead(struct evhttp_request * request, void * arg)
{
	if (KinectHandlerThread * self = reinterpret_cast<KinectHandlerThread *>(arg)) {
		self->_OnRead(fd);
	}
}

void KinectHandlerThread::_OnRead(struct evhttp_request *request)
{
	/*wxString buffer(ReadHttpHeader(_fd));
	HttpHeader header(buffer);
*/
	evkeyvalq * headers = evhttp_request_get_input_headers(request);
	char * packetTypeHeader = evhttp_find_header(headers, "Kinect-Data");
	int packetType = packetTypeHeader != NULL ? atoi(packetTypeHeader) : KINECT_DATA_NONE;
	switch(packetType)
	{
	case FRAME_COLOR:
		wxLogDebug(_("COLOR frame arrived");
		break;
	case FRAME_DEPTH:
		wxLogDebug(_("DEPTH frame arrived");
		break;
	case FRAME_SKELETON:
		wxLogDebug(_("SKELETON frame arrived");
		break;
	default:
		break;
	}
}
