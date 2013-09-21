extern "C" 
{
#include <event2/event.h>  
#include <event2/http.h>  
#include <event2/buffer.h>  
}
#include <wx/wx.h>
#include <wx/socket.h>
#include "KinectDataSender.h"
using namespace kinect_data;
KinectDataSender::KinectDataSender(const wxString & hostName, unsigned short port)
: wxThread(wxTHREAD_DETACHED)
, m_HostName(hostName)
, m_Port(port)
, m_rawData(NULL)
, m_bDataPrepared(false)
{
	m_mutexSend = new wxMutex();
	m_condSend = new wxCondition(*m_mutexSend);
}

KinectDataSender::~KinectDataSender()
{
	wxDELETE(m_condSend);
	wxDELETE(m_mutexSend);
	if(m_rawData != NULL) delete m_rawData;
}

wxThread::ExitCode KinectDataSender::Entry()
{
#if defined(__WXMSW__)
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2), &wsaData);
#endif
	do
	{
		_evbase = event_base_new();
		if(!_evbase) break;
		_evhttp_connection = evhttp_connection_base_new(_evbase, NULL, m_HostName.ToAscii(), m_Port);
		if(!_evhttp_connection) break;
		std::string s = "connect";
		m_bConnected = false;
		while(!TestDestroy())
		{
			if(!_evhttp_connection) break;
			if(m_bConnected)
			{
				if(m_condSend->Wait() == wxCOND_NO_ERROR)
				{
					if(m_bDataPrepared){
						wxCriticalSectionLocker locker(m_cs);
						Send(m_rawData, m_dataLen, m_dataType);
						m_bDataPrepared = false;
					}
				}
			}
			else
			{				
				Send((void*)s.c_str(), s.length(), CONNECTION_INIT);
			}

		}
	}
	while(false);
	//CloseHTTPConnection();
	if(_evbase)
	{
		event_base_free(_evbase);
	}
#if defined(__WXMSW__)
	WSACleanup();
#endif
	return NULL;
}

void KinectDataSender::Stop()
{
	CloseHTTPConnection();
	m_bDataPrepared = false;
	m_condSend->Broadcast();
}

void KinectDataSender::_OnRequestDone(struct evhttp_request * request)
{
	if(request) 
	{
		int responseCode = evhttp_request_get_response_code(request);
		//wxLogDebug(_("Request DONE: %d"), responseCode);
	}
	else
	{
		//wxLogDebug(_("Wrong request"));
	}
	m_bConnected = request != NULL;
	event_base_loopexit(_evbase, NULL);
}

void KinectDataSender::_OnRequestDone(struct evhttp_request * request, void * arg)
{
	if (KinectDataSender * self = reinterpret_cast<KinectDataSender*>(arg)) {
		self->_OnRequestDone(request);
	}
}

void KinectDataSender::SendKinectData(void * data, size_t length, KinectDataType type)
{
	if(!m_bConnected || m_bDataPrepared) return;
	wxCriticalSectionLocker locker(m_cs);
	if(m_rawData != NULL) delete m_rawData;
	m_rawData = new char[length]();
	memcpy(m_rawData, data, length);
	m_dataLen = length;
	m_dataType = type;
	m_bDataPrepared = true;
	m_condSend->Broadcast();
}

void KinectDataSender::Send(void * data, size_t length, KinectDataType type)
{
	do
	{
		if(!_evhttp_connection) break;
		evhttp_request * request = evhttp_request_new(KinectDataSender::_OnRequestDone, this);
		evkeyvalq * headers = evhttp_request_get_output_headers(request);
		//evhttp_add_header(headers, "Connection", "Keep-Alive");
		evhttp_add_header(headers, "Kinect-Data", wxString::Format(wxT("%d"), (int)type).ToAscii());
		evhttp_add_header(headers, "Connection", "Keep-Alive");
		//evhttp_add_header(headers, "Transfer-Encoding", "chunked");
		if(data && length)
		{
			evbuffer * buffer = evhttp_request_get_output_buffer(request);
			if(evbuffer_add(buffer, data, length)) break;
		}
		if(!_evhttp_connection) break;
		int retCode = evhttp_make_request(_evhttp_connection, request, EVHTTP_REQ_POST, "/");
		//event_base_dispatch(_evbase);
		//wxLogDebug(wxT("RetCode: %d"), retCode);
		event_base_dispatch(_evbase);
	}
	while(false);
}

void KinectDataSender::CloseHTTPConnection()
{
	if(_evhttp_connection)
	{
		evhttp_connection_free(_evhttp_connection);
		_evhttp_connection = NULL;
	}
}
