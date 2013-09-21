/////////////////////////////////////////////////////////////////////////////
// Name:        KinectCanvas.cpp
// Purpose:     
// Author:      Volodymir (T-Rex) Tryapichko
// Modified by: 
// Created:     30/06/2011 14:16:35
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

#include "KinectCanvas.h"
#include <wx/dcbuffer.h>

////@begin XPM images
////@end XPM images


/*
 * KinectCanvas type definition
 */

IMPLEMENT_DYNAMIC_CLASS( KinectCanvas, wxWindow )


/*
 * KinectCanvas event table definition
 */

BEGIN_EVENT_TABLE( KinectCanvas, wxWindow )

////@begin KinectCanvas event table entries
	EVT_PAINT( KinectCanvas::OnPaint )

////@end KinectCanvas event table entries

END_EVENT_TABLE()


/*
 * KinectCanvas constructors
 */

KinectCanvas::KinectCanvas()
{
	Init();
}

KinectCanvas::KinectCanvas(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
{
	Init();
	Create(parent, id, pos, size, style);
}


/*
 * KinectCanvas creator
 */

bool KinectCanvas::Create(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
{
////@begin KinectCanvas creation
	wxWindow::Create(parent, id, pos, size, style);
	CreateControls();
////@end KinectCanvas creation
	return true;
}


/*
 * KinectCanvas destructor
 */

KinectCanvas::~KinectCanvas()
{
////@begin KinectCanvas destruction
////@end KinectCanvas destruction
}


/*
 * Member initialisation
 */

void KinectCanvas::Init()
{
////@begin KinectCanvas member initialisation
	m_CurrentImage = NULL;
////@end KinectCanvas member initialisation
}


/*
 * Control creation for KinectCanvas
 */

void KinectCanvas::CreateControls()
{    
////@begin KinectCanvas content construction
////@end KinectCanvas content construction
	SetBackgroundStyle(wxBG_STYLE_CUSTOM);
}


/*
 * Should we show tooltips?
 */

bool KinectCanvas::ShowToolTips()
{
	return true;
}

/*
 * Get bitmap resources
 */

wxBitmap KinectCanvas::GetBitmapResource( const wxString& name )
{
	// Bitmap retrieval
////@begin KinectCanvas bitmap retrieval
	wxUnusedVar(name);
	return wxNullBitmap;
////@end KinectCanvas bitmap retrieval
}

/*
 * Get icon resources
 */

wxIcon KinectCanvas::GetIconResource( const wxString& name )
{
	// Icon retrieval
////@begin KinectCanvas icon retrieval
	wxUnusedVar(name);
	return wxNullIcon;
////@end KinectCanvas icon retrieval
}


/*
 * wxEVT_PAINT event handler for ID_KINECTCANVAS
 */

void KinectCanvas::OnPaint( wxPaintEvent& event )
{
	wxAutoBufferedPaintDC dc(this);
	if(!m_CurrentImage)
	{
		dc.SetBackground(wxBrush(GetBackgroundColour(), wxSOLID));
		dc.Clear();
		dc.DrawLabel(_("No image"), wxRect(GetSize()), wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL);
	}
	else
	{
		wxBitmap bmp(*m_CurrentImage);
		dc.DrawBitmap(bmp, (GetSize().GetWidth()-bmp.GetWidth())/2, (GetSize().GetHeight()-bmp.GetHeight())/2);
	}
}

