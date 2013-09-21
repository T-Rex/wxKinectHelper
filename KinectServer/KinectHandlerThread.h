#pragma once

#include "ext_thread.h"

struct event_base;
struct evhttp;
class  HttpHeader;

class KinectHandlerThread : public ext::Thread
{
public:
	KinectHandlerThread();
private:
	virtual void Execute();
	void _OnRead(struct evhttp_request * request);
	static void _OnRead(struct evhttp_request * request, void * arg);

	event_base * _evbase;
	evhttp * _evhttp;
};
