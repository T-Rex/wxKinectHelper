/////////////////////////////////////////////////////////////////////////////
// Name:        KinectCanvas.h
// Purpose:     
// Author:      Volodymir (T-Rex) Tryapichko
// Modified by: 
// Created:     30/06/2011 14:16:35
// RCS-ID:      
// Copyright:   Volodymir (T-Rex) Tryapichko, 2008-2011
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _KINECTCANVAS_H_
#define _KINECTCANVAS_H_


/*!
 * Includes
 */

////@begin includes
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
class KinectCanvas;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_KINECTCANVAS 10002
#define SYMBOL_KINECTCANVAS_STYLE wxSIMPLE_BORDER
#define SYMBOL_KINECTCANVAS_IDNAME ID_KINECTCANVAS
#define SYMBOL_KINECTCANVAS_SIZE wxSize(320, 240)
#define SYMBOL_KINECTCANVAS_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * KinectCanvas class declaration
 */

class KinectCanvas: public wxWindow
{    
    DECLARE_DYNAMIC_CLASS( KinectCanvas )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    KinectCanvas();
    KinectCanvas(wxWindow* parent, wxWindowID id = ID_KINECTCANVAS, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(100, 100), long style = wxSIMPLE_BORDER);

    /// Creation
    bool Create(wxWindow* parent, wxWindowID id = ID_KINECTCANVAS, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(100, 100), long style = wxSIMPLE_BORDER);

    /// Destructor
    ~KinectCanvas();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin KinectCanvas event handler declarations

    /// wxEVT_PAINT event handler for ID_KINECTCANVAS
    void OnPaint( wxPaintEvent& event );

////@end KinectCanvas event handler declarations

////@begin KinectCanvas member function declarations

    wxImage * GetCurrentImage() const { return m_CurrentImage ; }
    void SetCurrentImage(wxImage * value) { m_CurrentImage = value ; }

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end KinectCanvas member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin KinectCanvas member variables
    wxImage * m_CurrentImage;
////@end KinectCanvas member variables
};

#endif
    // _KINECTCANVAS_H_
