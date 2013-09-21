#pragma once

#include "ext_thread.h"

struct event_base;
struct evhttp;
class KinectDataProcessorBase;

class KinectServer : public ext::Thread
{
public:
	enum Status
	{
		Running,
		Stopped
	};
	KinectServer(int port);
	Status GetStatus() const;
	void Stop();
	void SetDataProcessor(KinectDataProcessorBase * processor);
private:
	virtual void Execute();
	void _OnRequest(struct evhttp_request * request);
	static void _OnRequest(struct evhttp_request * request, void * arg);

	int _port;
	event_base * _evbase;
	evhttp * _evhttp;
	Status _status;
	KinectDataProcessorBase * _dataProcessor;
};