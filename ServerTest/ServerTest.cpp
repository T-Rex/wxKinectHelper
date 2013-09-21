#include "stdwx.h"
#include <stdio.h>
#include "KinectDatasender.h"

class ServerTestApp : public wxAppConsole
{
	virtual bool OnInit()
	{
		wxLogDebug(wxT("Initialized"));
		return true;
	}
};

IMPLEMENT_APP_NO_MAIN(ServerTestApp);

int main(int & argc, char ** argv)
{
	int aargc = 0;
	char ** aargv = NULL;
	wxEntryStart(aargc, aargv);


	KinectDataSender * sender = new KinectDataSender(wxT("127.0.0.1"), 9500);
	if(sender->Create() != wxTHREAD_NO_ERROR)
	{
		wxDELETE(sender);
		return 1;
	}
	sender->Run();
	wxSleep(1);
	char * data = new char[10];
	sprintf(data, "12345");
	sender->SendKinectData(data, strlen(data), FRAME_COLOR);
	/*sprintf(data, "23456");
	sender->Send(data, strlen(data), FRAME_COLOR);
	sprintf(data, "34567");
	sender->Send(data, strlen(data), FRAME_COLOR);*/
	delete [] data;
	getchar();
	sender->Stop();
	sender->Delete();
	wxEntryCleanup();
	return 0;
}
