extern "C" 
{
#include <event2/event.h>  
#include <event2/http.h>  
#include <event2/buffer.h>  
}
#include "KinectTypes.h"
#include "KinectServer.h"
#include "KinectDataProcessorBase.h"

using namespace kinect_data;

KinectServer::KinectServer(int port)
: _port(port)
, _evbase(NULL)
, _evhttp(NULL)
, _status(Stopped)
, _dataProcessor(NULL)
{

}

void KinectServer::Execute()
{
#if defined(WIN32)
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2), &wsaData);
#endif
	do
	{
		_status = Running;
		_evbase = event_base_new();
		if(!_evbase) break;
		_evhttp = evhttp_new(_evbase);  
		if(!_evhttp) break;
		if(evhttp_bind_socket(_evhttp, "0.0.0.0", _port) != 0) break;
		evhttp_set_gencb(_evhttp, _OnRequest, this);
		event_base_dispatch(_evbase);
	}
	while(false);
	if(_evbase)
	{
		event_base_free(_evbase);
		_evbase = NULL;
	}
#if defined(WIN32)
	WSACleanup();
#endif
	_status = Stopped;
}

void KinectServer::_OnRequest(struct evhttp_request * request, void * arg)
{
	if (KinectServer * self = reinterpret_cast<KinectServer*>(arg)) {
		self->_OnRequest(request);
	}
}

void KinectServer::_OnRequest(struct evhttp_request *request)
{
	evkeyvalq * headers = evhttp_request_get_input_headers(request);
	const char * packetTypeHeader = evhttp_find_header(headers, "Kinect-Data");
	int packetType = packetTypeHeader != NULL ? atoi(packetTypeHeader) : KINECT_DATA_NONE;
	evbuffer * buffer = evhttp_request_get_input_buffer(request);
	if(buffer)
	{
		size_t bufferLength = evbuffer_get_length(buffer);
		unsigned char * bufferData = bufferLength > 0 ? new unsigned char[bufferLength] : NULL;
		memset(bufferData, 0, bufferLength);
		evbuffer_copyout(buffer, bufferData, bufferLength);
		bool processingResult = false;
		switch(packetType)
		{
		case CONNECTION_INIT:
			printf("CONNECTED\r\n");
			break;
		case FRAME_COLOR:
			//printf("COLOR\r\n");
			processingResult = _dataProcessor != NULL ? _dataProcessor->ProcessColorFrame(bufferData, bufferLength) : false;
			break;
		case FRAME_DEPTH:
			//printf("DEPTH\r\n");
			processingResult = _dataProcessor != NULL ? _dataProcessor->ProcessDepthFrame(bufferData, bufferLength) : false;
			break;
		case FRAME_SKELETON:
			//printf("SKELETON\r\n");
			processingResult = _dataProcessor != NULL ? _dataProcessor->ProcessSkeletonFrame(bufferData, bufferLength) : false;
			break;
		default:
			printf("UNSUPPORTED\r\n");
			break;
		}
		delete [] bufferData;
		bufferData = NULL;
		evhttp_send_reply(request, HTTP_OK, NULL, NULL);
	}
}

KinectServer::Status KinectServer::GetStatus() const
{
	return _status;
}

void KinectServer::Stop()
{
	if(_evbase)
	{
		if(_evhttp)
		{
			evhttp_free(_evhttp);
			_evhttp = NULL;
		}
		event_base_loopbreak(_evbase);
	}
}

void KinectServer::SetDataProcessor(KinectDataProcessorBase * processor)
{
	_dataProcessor = processor;
}