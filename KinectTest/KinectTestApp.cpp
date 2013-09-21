/////////////////////////////////////////////////////////////////////////////
// Name:        KinectTestApp.cpp
// Purpose:     
// Author:      Volodymir (T-Rex) Tryapichko
// Modified by: 
// Created:     30/06/2011 14:14:49
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

#include "KinectTestApp.h"

////@begin XPM images
////@end XPM images


/*
 * Application instance implementation
 */

////@begin implement app
IMPLEMENT_APP( KinectTestApp )
////@end implement app


/*
 * KinectTestApp type definition
 */

IMPLEMENT_CLASS( KinectTestApp, wxApp )


/*
 * KinectTestApp event table definition
 */

BEGIN_EVENT_TABLE( KinectTestApp, wxApp )

////@begin KinectTestApp event table entries
////@end KinectTestApp event table entries

END_EVENT_TABLE()


/*
 * Constructor for KinectTestApp
 */

KinectTestApp::KinectTestApp()
{
	Init();
}


/*
 * Member initialisation
 */

void KinectTestApp::Init()
{
////@begin KinectTestApp member initialisation
////@end KinectTestApp member initialisation
}

/*
 * Initialisation for KinectTestApp
 */

bool KinectTestApp::OnInit()
{    
////@begin KinectTestApp initialisation
	// Remove the comment markers above and below this block
	// to make permanent changes to the code.

#if wxUSE_XPM
	wxImage::AddHandler(new wxXPMHandler);
#endif
#if wxUSE_LIBPNG
	wxImage::AddHandler(new wxPNGHandler);
#endif
#if wxUSE_LIBJPEG
	wxImage::AddHandler(new wxJPEGHandler);
#endif
#if wxUSE_GIF
	wxImage::AddHandler(new wxGIFHandler);
#endif
	KinectTestMainFrame* mainWindow = new KinectTestMainFrame( NULL );
	mainWindow->Show(true);
////@end KinectTestApp initialisation

	return true;
}


/*
 * Cleanup for KinectTestApp
 */

int KinectTestApp::OnExit()
{    
////@begin KinectTestApp cleanup
	return wxApp::OnExit();
////@end KinectTestApp cleanup
}

