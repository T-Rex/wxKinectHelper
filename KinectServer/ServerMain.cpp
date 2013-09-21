#include <stdio.h>
#include "KinectServer.h"
#include "JSONNode.h"
#ifdef USE_OPENCV
#include "cv.h"
#include "ObjectDefinition.h"
#include "SimpleDataProcessor.h"
#endif
#ifdef USE_PCL
#include "PclDataProcessor.h"
#endif
#define KINECT_PORT 9500

#ifdef FIRST_TEST
#define FEAT_PATH ""
#define IMG_PATH ""
#else
#endif
int main(int argc, char** argv)
{
	KinectServer * server = new KinectServer(KINECT_PORT);
	KinectDataProcessorBase* dataProc = NULL; 
#ifdef USE_OPENCV
	dataProc = new SimpleDataProcessor(FEAT_PATH);
	dataProc->TrainImages(IMG_PATH);
#endif //USE_OPENCV
#ifdef USE_PCL
	dataProc = new PclDataProcessor(FEAT_PATH);
#endif //USE_PCL
	server->SetDataProcessor(dataProc);
	server->Start();
	printf("Server started.\r\n");
	getchar();
	printf("Stopping server");
	server->Stop();
	while (server->GetStatus() != KinectServer::Stopped)
	{
		printf(".");
	}
	delete server;
	delete dataProc;
	return 0;
}