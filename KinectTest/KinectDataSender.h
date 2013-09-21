#pragma once

#include <wx/wx.h>
#include "KinectTypes.h"
struct event_base;
struct evhttp_connection;
struct evhttp_request;
using namespace kinect_data;
class KinectDataSender : public wxThread
{
public:

	KinectDataSender(const wxString & hostName, unsigned short port);
	virtual ~KinectDataSender();

	virtual wxThread::ExitCode Entry();

	void SendKinectData(void * data, size_t length, KinectDataType type);
	void Stop();

private:
	void Send(void * data, size_t length, KinectDataType type);
	void CloseHTTPConnection();
	void _OnRequestDone(struct evhttp_request * request);

	bool m_bConnected;
	bool m_bDataPrepared;
	wxString m_HostName;
	unsigned short m_Port;
	event_base * _evbase;
	evhttp_connection * _evhttp_connection;
	wxCriticalSection m_cs;
	wxMutex* m_mutexSend;
	wxCondition* m_condSend;
	char* m_rawData;
	size_t m_dataLen;
	KinectDataType m_dataType;
	static void _OnRequestDone(struct evhttp_request * request, void * arg);
};