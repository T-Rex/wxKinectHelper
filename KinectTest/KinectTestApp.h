/////////////////////////////////////////////////////////////////////////////
// Name:        KinectTestApp.h
// Purpose:     
// Author:      Volodymir (T-Rex) Tryapichko
// Modified by: 
// Created:     30/06/2011 14:14:49
// RCS-ID:      
// Copyright:   Volodymir (T-Rex) Tryapichko, 2008-2011
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _KINECTTESTAPP_H_
#define _KINECTTESTAPP_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/image.h"
#include "KinectTestMainFrame.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
////@end control identifiers

/*!
 * KinectTestApp class declaration
 */

class KinectTestApp: public wxApp
{    
    DECLARE_CLASS( KinectTestApp )
    DECLARE_EVENT_TABLE()

public:
    /// Constructor
    KinectTestApp();

    void Init();

    /// Initialises the application
    virtual bool OnInit();

    /// Called on exit
    virtual int OnExit();

////@begin KinectTestApp event handler declarations

////@end KinectTestApp event handler declarations

////@begin KinectTestApp member function declarations

////@end KinectTestApp member function declarations

////@begin KinectTestApp member variables
////@end KinectTestApp member variables
};

/*!
 * Application instance declaration 
 */

////@begin declare app
DECLARE_APP(KinectTestApp)
////@end declare app

#endif
    // _KINECTTESTAPP_H_
