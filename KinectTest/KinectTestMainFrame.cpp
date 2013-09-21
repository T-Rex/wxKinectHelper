/////////////////////////////////////////////////////////////////////////////
// Name:        KinectTestMainFrame.cpp
// Purpose:     
// Author:      Volodymir (T-Rex) Tryapichko
// Modified by: 
// Created:     30/06/2011 14:15:08
// RCS-ID:      
// Copyright:   Volodymir (T-Rex) Tryapichko, 2008-2011
// Licence:     
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes
#include "KinectTypes.h"
#include "KinectTestMainFrame.h"
#include "KinectCanvas.h"
#include "SkeletonPainter.h"
#include "KinectGrabberBase.h"
#include "KinectDataSender.h"
using namespace kinect_data;
////@begin XPM images
////@end XPM images

#define SEND_VIDEO_FRAME
#define SEND_DEPTH_FRAME
#define SEND_SKELETON_FRAME
/*
 * KinectTestMainFrame type definition
 */

IMPLEMENT_CLASS( KinectTestMainFrame, wxFrame )


/*
 * KinectTestMainFrame event table definition
 */

BEGIN_EVENT_TABLE( KinectTestMainFrame, wxFrame )

////@begin KinectTestMainFrame event table entries
	EVT_LISTBOX( ID_DEVICE_LISTBOX, KinectTestMainFrame::OnDEVICELISTBOXSelected )

	EVT_BUTTON( wxID_REFRESH, KinectTestMainFrame::OnREFRESHClick )

	EVT_BUTTON( ID_GRABBER_START, KinectTestMainFrame::OnGRABBERSTARTClick )
	EVT_UPDATE_UI( ID_GRABBER_START, KinectTestMainFrame::OnGRABBERSTARTUpdate )

	EVT_BUTTON( ID_GRABBER_STOP, KinectTestMainFrame::OnGRABBERSTOPClick )
	EVT_UPDATE_UI( ID_GRABBER_STOP, KinectTestMainFrame::OnGRABBERSTOPUpdate )

	EVT_UPDATE_UI( ID_TEXTCTRL, KinectTestMainFrame::OnTEXTCTRLUpdate )

	EVT_UPDATE_UI( ID_SPINCTRL, KinectTestMainFrame::OnSPINCTRLUpdate )

////@end KinectTestMainFrame event table entries
	EVT_COMMAND (wxID_ANY, KINECT_DEPTH_FRAME_RECEIVED, KinectTestMainFrame::OnDepthFrame)
	EVT_COMMAND (wxID_ANY, KINECT_COLOR_FRAME_RECEIVED, KinectTestMainFrame::OnColorFrame)
	EVT_COMMAND (wxID_ANY, KINECT_SKELETON_FRAME_RECEIVED, KinectTestMainFrame::OnSkeletonFrame)

END_EVENT_TABLE()


/*
 * KinectTestMainFrame constructors
 */

KinectTestMainFrame::KinectTestMainFrame()
{
	Init();
}

KinectTestMainFrame::KinectTestMainFrame( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
	Init();
	Create( parent, id, caption, pos, size, style );
}


/*
 * KinectTestMainFrame creator
 */

bool KinectTestMainFrame::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin KinectTestMainFrame creation
	wxFrame::Create( parent, id, caption, pos, size, style );

	CreateControls();
	if (GetSizer())
	{
		GetSizer()->SetSizeHints(this);
	}
	Centre();
////@end KinectTestMainFrame creation
	return true;
}


/*
 * KinectTestMainFrame destructor
 */

KinectTestMainFrame::~KinectTestMainFrame()
{
////@begin KinectTestMainFrame destruction
////@end KinectTestMainFrame destruction
	StopGrabbing();
	wxDELETE(m_KinectHelper);
}


/*
 * Member initialisation
 */

void KinectTestMainFrame::Init()
{
////@begin KinectTestMainFrame member initialisation
	m_KinectHelper = new KinectHelper;
	m_SelectedDeviceIndex = -1;
	m_SkeletonImage = wxImage(320, 240);
	m_Grabber = NULL;
	m_DataSender = NULL;
	m_MainSizer = NULL;
	m_DeviceListBox = NULL;
	m_SendToGatewayCheckBox = NULL;
	m_DepthCanvas = NULL;
	m_ColorCanvas = NULL;
	m_SkeletonCanvas = NULL;
	m_GatewayNameTextCtrl = NULL;
	m_GatewayPortSpinCtrl = NULL;
////@end KinectTestMainFrame member initialisation
}


/*
 * Control creation for KinectTestMainFrame
 */

void KinectTestMainFrame::CreateControls()
{    
////@begin KinectTestMainFrame content construction
	KinectTestMainFrame* itemFrame1 = this;

	m_MainSizer = new wxGridBagSizer(0, 0);
	m_MainSizer->SetEmptyCellSize(wxSize(10, 20));
	itemFrame1->SetSizer(m_MainSizer);

	wxArrayString m_DeviceListBoxStrings;
	m_DeviceListBox = new wxListBox( itemFrame1, ID_DEVICE_LISTBOX, wxDefaultPosition, wxSize(-1, 230), m_DeviceListBoxStrings, wxLB_SINGLE );
	m_MainSizer->Add(m_DeviceListBox, wxGBPosition(3, 0), wxGBSpan(2, 1), wxGROW|wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);

	m_SendToGatewayCheckBox = new wxCheckBox( itemFrame1, ID_CHECKBOX, _("Send to gateway"), wxDefaultPosition, wxDefaultSize, 0 );
	m_SendToGatewayCheckBox->SetValue(false);
	m_MainSizer->Add(m_SendToGatewayCheckBox, wxGBPosition(0, 0), wxGBSpan(1, 1), wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_DepthCanvas = new KinectCanvas( itemFrame1, ID_DEPTH_CANVAS, wxDefaultPosition, wxSize(320, 240), wxSIMPLE_BORDER );
	m_MainSizer->Add(m_DepthCanvas, wxGBPosition(0, 1), wxGBSpan(4, 1), wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxTOP|wxBOTTOM, 5);

	m_ColorCanvas = new KinectCanvas( itemFrame1, ID_COLOR_CANVAS, wxDefaultPosition, wxSize(640, 480), wxSIMPLE_BORDER );
	m_MainSizer->Add(m_ColorCanvas, wxGBPosition(4, 1), wxGBSpan(1, 2), wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxRIGHT, 5);

	m_SkeletonCanvas = new KinectCanvas( itemFrame1, ID_SKELETON_CANVAS, wxDefaultPosition, wxSize(320, 240), wxSIMPLE_BORDER );
	m_MainSizer->Add(m_SkeletonCanvas, wxGBPosition(0, 2), wxGBSpan(4, 1), wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxRIGHT|wxTOP|wxBOTTOM, 5);

	wxBoxSizer* itemBoxSizer8 = new wxBoxSizer(wxHORIZONTAL);
	m_MainSizer->Add(itemBoxSizer8, wxGBPosition(5, 0), wxGBSpan(1, 3), wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 0);

	wxButton* itemButton9 = new wxButton( itemFrame1, wxID_REFRESH, _("Refresh"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer8->Add(itemButton9, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

	wxButton* itemButton10 = new wxButton( itemFrame1, ID_GRABBER_START, _("Start Grabber"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer8->Add(itemButton10, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

	wxButton* itemButton11 = new wxButton( itemFrame1, ID_GRABBER_STOP, _("Stop Grabber"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer8->Add(itemButton11, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_GatewayNameTextCtrl = new wxTextCtrl( itemFrame1, ID_TEXTCTRL, _("localhost"), wxDefaultPosition, wxDefaultSize, 0 );
	m_MainSizer->Add(m_GatewayNameTextCtrl, wxGBPosition(1, 0), wxGBSpan(1, 1), wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_GatewayPortSpinCtrl = new wxSpinCtrl( itemFrame1, ID_SPINCTRL, _T("0"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0 );
	m_MainSizer->Add(m_GatewayPortSpinCtrl, wxGBPosition(2, 0), wxGBSpan(1, 1), wxGROW|wxALIGN_TOP|wxALL, 5);

////@end KinectTestMainFrame content construction
	//m_MainSizer->AddGrowableCol(1);
	//m_MainSizer->AddGrowableRow(0);
	ShowDevices();
	//m_Canvas->SetCurrentImage(m_CurrentImage);
}


/*
 * Should we show tooltips?
 */

bool KinectTestMainFrame::ShowToolTips()
{
	return true;
}

/*
 * Get bitmap resources
 */

wxBitmap KinectTestMainFrame::GetBitmapResource( const wxString& name )
{
	// Bitmap retrieval
////@begin KinectTestMainFrame bitmap retrieval
	wxUnusedVar(name);
	return wxNullBitmap;
////@end KinectTestMainFrame bitmap retrieval
}

/*
 * Get icon resources
 */

wxIcon KinectTestMainFrame::GetIconResource( const wxString& name )
{
	// Icon retrieval
////@begin KinectTestMainFrame icon retrieval
	wxUnusedVar(name);
	return wxNullIcon;
////@end KinectTestMainFrame icon retrieval
}

void KinectTestMainFrame::ShowDevices()
{
	StopGrabbing();
	size_t count = m_KinectHelper->GetDeviceCount();
	m_DeviceListBox->Clear();
	for(size_t i = 0; i < count; ++i)
	{
		int item = m_DeviceListBox->Append(m_KinectHelper->GetDeviceName(i));
		m_DeviceListBox->SetClientData(item, (void*)i);
	}
}


/*
 * wxEVT_COMMAND_LISTBOX_SELECTED event handler for ID_DEVICE_LISTBOX
 */

void KinectTestMainFrame::OnDEVICELISTBOXSelected( wxCommandEvent& event )
{
	do
	{
		size_t deviceIndex = (size_t)m_DeviceListBox->GetClientData(event.GetInt());
		if(deviceIndex < 0 || deviceIndex > m_KinectHelper->GetDeviceCount()) break;
		m_SelectedDeviceIndex = deviceIndex;
		StartGrabbing();
	}
	while(false);
}

void KinectTestMainFrame::StartGrabbing()
{
	StopGrabbing();
	m_DataSender = new KinectDataSender(wxT("127.0.0.1"), 9500);
	if(m_DataSender->Create() != wxTHREAD_NO_ERROR)
	{
		m_DataSender->Delete();
		m_DataSender = NULL;
	}
	m_DataSender->Run();
	m_Grabber = m_KinectHelper->CreateGrabber(this, m_SelectedDeviceIndex);
	m_CurrentImage = wxImage(m_Grabber->GetDepthFrameSize().GetWidth(), m_Grabber->GetDepthFrameSize().GetHeight());
	m_ColorImage = wxImage(m_Grabber->GetColorFrameSize().GetWidth(), m_Grabber->GetColorFrameSize().GetHeight());
	m_SkeletonImage = wxImage(m_Grabber->GetDepthFrameSize().GetWidth(), m_Grabber->GetDepthFrameSize().GetHeight());
	m_DepthCanvas->SetCurrentImage(&m_CurrentImage);
	m_ColorCanvas->SetCurrentImage(&m_ColorImage);
	m_SkeletonCanvas->SetCurrentImage(&m_SkeletonImage);
	if(!m_Grabber->Start())
	{
		StopGrabbing();
	}
}

void KinectTestMainFrame::StopGrabbing()
{
	if(m_Grabber)
	{
		if(m_Grabber->IsStarted())
		{
			m_Grabber->Stop();
		}
		wxDELETE(m_Grabber);
	}
	m_DepthCanvas->SetCurrentImage(NULL);
	m_ColorCanvas->SetCurrentImage(NULL);
	m_SkeletonCanvas->SetCurrentImage(NULL);
	m_DepthCanvas->Refresh();
	m_ColorCanvas->Refresh();
	m_SkeletonCanvas->Refresh();

	if(m_DataSender)
	{
		if(m_DataSender->IsRunning())
		{
			m_DataSender->Stop();
			m_DataSender->Delete();
		}
		m_DataSender = NULL;
	}
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_GRABBER_START
 */

void KinectTestMainFrame::OnGRABBERSTARTClick( wxCommandEvent& event )
{
	StartGrabbing();
}


/*
 * wxEVT_UPDATE_UI event handler for ID_GRABBER_START
 */

void KinectTestMainFrame::OnGRABBERSTARTUpdate( wxUpdateUIEvent& event )
{
	event.Enable(m_SelectedDeviceIndex >= 0 && (!m_Grabber || !m_Grabber->IsStarted()));
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_GRABBER_STOP
 */

void KinectTestMainFrame::OnGRABBERSTOPClick( wxCommandEvent& event )
{
	StopGrabbing();
}


/*
 * wxEVT_UPDATE_UI event handler for ID_GRABBER_STOP
 */

void KinectTestMainFrame::OnGRABBERSTOPUpdate( wxUpdateUIEvent& event )
{
	event.Enable(m_Grabber && m_Grabber->IsStarted());
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_REFRESH
 */

void KinectTestMainFrame::OnREFRESHClick( wxCommandEvent& event )
{
	ShowDevices();
}

void KinectTestMainFrame::OnDepthFrame(wxCommandEvent & event)
{
	do
	{
		if(!m_Grabber) break;
		m_Grabber->GrabDepthFrame(m_CurrentImage.GetData());
		m_DepthCanvas->Refresh();
#ifdef SEND_DEPTH_FRAME
		if(m_DataSender)
		{
			VideoFrame frame((size_t) m_CurrentImage.GetWidth(), 
						     (size_t) m_CurrentImage.GetHeight(),
							 3,
							 m_CurrentImage.GetData());
			if(frame.IsOK()) 
				m_DataSender->SendKinectData(frame.GetRawData(), frame.GetSize(), FRAME_DEPTH);
		}
#endif
	}
	while(false);
}

void KinectTestMainFrame::OnColorFrame(wxCommandEvent & event)
{
	do
	{
		if(!m_Grabber) break;
		m_Grabber->GrabColorFrame(m_ColorImage.GetData());
		m_ColorCanvas->Refresh();
#ifdef SEND_VIDEO_FRAME
		if(m_DataSender)
		{
			VideoFrame frame((size_t) m_ColorImage.GetWidth(), 
						     (size_t) m_ColorImage.GetHeight(),
							 3,
							 m_ColorImage.GetData());
			if(frame.IsOK()) 
				m_DataSender->SendKinectData(frame.GetRawData(), frame.GetSize(), FRAME_COLOR);
		}
#endif
	}
	while(false);
}

void KinectTestMainFrame::OnSkeletonFrame(wxCommandEvent & event)
{
	do
	{
		if(!m_Grabber) break;
		SkeletonPainter painter;
		wxBitmap bmp(m_SkeletonImage.GetWidth(), m_SkeletonImage.GetHeight());
		wxMemoryDC mdc(bmp);
		painter.DrawSkeleton(mdc, m_Grabber->GrabSkeletonFrame());
		mdc.SelectObject(wxNullBitmap);
		m_SkeletonImage = bmp.ConvertToImage();
		m_SkeletonCanvas->Refresh();
#ifdef SEND_SKELETON_FRAME
		if(m_DataSender)
		{
			kinect_data::SkeletonFrame* skeleton = m_Grabber->GrabSkeletonFrame();
			unsigned char* buffer = new unsigned char[sizeof(kinect_data::SkeletonFrame)];
			memcpy(buffer, skeleton, sizeof(kinect_data::SkeletonFrame));
			m_DataSender->SendKinectData(buffer, sizeof(kinect_data::SkeletonFrame), FRAME_SKELETON);
			delete buffer;
		}
#endif
	}
	while(false);
}


/*
 * wxEVT_UPDATE_UI event handler for ID_TEXTCTRL
 */

void KinectTestMainFrame::OnTEXTCTRLUpdate( wxUpdateUIEvent& event )
{
	event.Enable(m_SendToGatewayCheckBox->IsChecked());
}


/*
 * wxEVT_UPDATE_UI event handler for ID_SPINCTRL
 */

void KinectTestMainFrame::OnSPINCTRLUpdate( wxUpdateUIEvent& event )
{
	event.Enable(m_SendToGatewayCheckBox->IsChecked());
}

