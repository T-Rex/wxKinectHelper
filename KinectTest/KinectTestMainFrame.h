/////////////////////////////////////////////////////////////////////////////
// Name:        KinectTestMainFrame.h
// Purpose:     
// Author:      Volodymir (T-Rex) Tryapichko
// Modified by: 
// Created:     30/06/2011 14:15:08
// RCS-ID:      
// Copyright:   Volodymir (T-Rex) Tryapichko, 2008-2011
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _KINECTTESTMAINFRAME_H_
#define _KINECTTESTMAINFRAME_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/frame.h"
#include "wx/gbsizer.h"
#include "wx/spinctrl.h"
////@end includes
#include "KinectHelper.h"

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxGridBagSizer;
class KinectCanvas;
class wxSpinCtrl;
////@end forward declarations
class KinectDataSender;

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_KINECTTESTMAINFRAME 10000
#define ID_DEVICE_LISTBOX 10001
#define ID_CHECKBOX 10007
#define ID_DEPTH_CANVAS 10003
#define ID_COLOR_CANVAS 10002
#define ID_SKELETON_CANVAS 10004
#define ID_GRABBER_START 10005
#define ID_GRABBER_STOP 10006
#define ID_TEXTCTRL 10008
#define ID_SPINCTRL 10009
#define SYMBOL_KINECTTESTMAINFRAME_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_KINECTTESTMAINFRAME_TITLE _("Kinect Test")
#define SYMBOL_KINECTTESTMAINFRAME_IDNAME ID_KINECTTESTMAINFRAME
#define SYMBOL_KINECTTESTMAINFRAME_SIZE wxSize(400, 300)
#define SYMBOL_KINECTTESTMAINFRAME_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * KinectTestMainFrame class declaration
 */

class KinectTestMainFrame: public wxFrame
{    
	DECLARE_CLASS( KinectTestMainFrame )
	DECLARE_EVENT_TABLE()

public:
	/// Constructors
	KinectTestMainFrame();
	KinectTestMainFrame( wxWindow* parent, wxWindowID id = SYMBOL_KINECTTESTMAINFRAME_IDNAME, const wxString& caption = SYMBOL_KINECTTESTMAINFRAME_TITLE, const wxPoint& pos = SYMBOL_KINECTTESTMAINFRAME_POSITION, const wxSize& size = SYMBOL_KINECTTESTMAINFRAME_SIZE, long style = SYMBOL_KINECTTESTMAINFRAME_STYLE );

	bool Create( wxWindow* parent, wxWindowID id = SYMBOL_KINECTTESTMAINFRAME_IDNAME, const wxString& caption = SYMBOL_KINECTTESTMAINFRAME_TITLE, const wxPoint& pos = SYMBOL_KINECTTESTMAINFRAME_POSITION, const wxSize& size = SYMBOL_KINECTTESTMAINFRAME_SIZE, long style = SYMBOL_KINECTTESTMAINFRAME_STYLE );

	/// Destructor
	~KinectTestMainFrame();

	/// Initialises member variables
	void Init();

	/// Creates the controls and sizers
	void CreateControls();

////@begin KinectTestMainFrame event handler declarations

    /// wxEVT_COMMAND_LISTBOX_SELECTED event handler for ID_DEVICE_LISTBOX
    void OnDEVICELISTBOXSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_REFRESH
    void OnREFRESHClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_GRABBER_START
    void OnGRABBERSTARTClick( wxCommandEvent& event );

    /// wxEVT_UPDATE_UI event handler for ID_GRABBER_START
    void OnGRABBERSTARTUpdate( wxUpdateUIEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_GRABBER_STOP
    void OnGRABBERSTOPClick( wxCommandEvent& event );

    /// wxEVT_UPDATE_UI event handler for ID_GRABBER_STOP
    void OnGRABBERSTOPUpdate( wxUpdateUIEvent& event );

    /// wxEVT_UPDATE_UI event handler for ID_TEXTCTRL
    void OnTEXTCTRLUpdate( wxUpdateUIEvent& event );

    /// wxEVT_UPDATE_UI event handler for ID_SPINCTRL
    void OnSPINCTRLUpdate( wxUpdateUIEvent& event );

////@end KinectTestMainFrame event handler declarations
	void OnDepthFrame(wxCommandEvent & event);
	void OnColorFrame(wxCommandEvent & event);
	void OnSkeletonFrame(wxCommandEvent & event);

////@begin KinectTestMainFrame member function declarations

    KinectHelper * GetKinectHelper() const { return m_KinectHelper ; }
    void SetKinectHelper(KinectHelper * value) { m_KinectHelper = value ; }

    wxImage GetCurrentImage() const { return m_CurrentImage ; }
    void SetCurrentImage(wxImage value) { m_CurrentImage = value ; }

    int GetSelectedDeviceInfo() const { return m_SelectedDeviceIndex ; }
    void SetSelectedDeviceInfo(int value) { m_SelectedDeviceIndex = value ; }

    wxImage GetColorImage() const { return m_ColorImage ; }
    void SetColorImage(wxImage value) { m_ColorImage = value ; }

    wxImage GetSkeletonImage() const { return m_SkeletonImage ; }
    void SetSkeletonImage(wxImage value) { m_SkeletonImage = value ; }

    KinectGrabberBase * GetGrabber() const { return m_Grabber ; }
    void SetGrabber(KinectGrabberBase * value) { m_Grabber = value ; }

    KinectDataSender * GetDataSender() const { return m_DataSender ; }
    void SetDataSender(KinectDataSender * value) { m_DataSender = value ; }

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end KinectTestMainFrame member function declarations

	void ShowDevices();
	void StartGrabbing();
	void StopGrabbing();

	/// Should we show tooltips?
	static bool ShowToolTips();

////@begin KinectTestMainFrame member variables
    wxGridBagSizer* m_MainSizer;
    wxListBox* m_DeviceListBox;
    wxCheckBox* m_SendToGatewayCheckBox;
    KinectCanvas* m_DepthCanvas;
    KinectCanvas* m_ColorCanvas;
    KinectCanvas* m_SkeletonCanvas;
    wxTextCtrl* m_GatewayNameTextCtrl;
    wxSpinCtrl* m_GatewayPortSpinCtrl;
    KinectHelper * m_KinectHelper;
    wxImage m_CurrentImage;
    int m_SelectedDeviceIndex;
    wxImage m_ColorImage;
    wxImage m_SkeletonImage;
    KinectGrabberBase * m_Grabber;
    KinectDataSender * m_DataSender;
////@end KinectTestMainFrame member variables
};

#endif
	// _KINECTTESTMAINFRAME_H_
